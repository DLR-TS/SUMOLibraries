/********************************************************************************
*                                                                               *
*                     T h e   A d i e   T e x t   E d i t o r                   *
*                                                                               *
*********************************************************************************
* Copyright (C) 1998,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This program is free software: you can redistribute it and/or modify          *
* it under the terms of the GNU General Public License as published by          *
* the Free Software Foundation, either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* This program is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU General Public License for more details.                                  *
*                                                                               *
* You should have received a copy of the GNU General Public License             *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.         *
********************************************************************************/
#include "fx.h"
#include "fxkeys.h"
#include "FX88591Codec.h"
#include "FXCP1252Codec.h"
#include "FXUTF16Codec.h"
#include "HelpWindow.h"
#include "Preferences.h"
#include "Commands.h"
#include "Modeline.h"
#include "Syntax.h"
#include "TextWindow.h"
#include "Adie.h"
#include "FindInFiles.h"
#include "ShellCommand.h"
#include "ShellDialog.h"
#include "icons.h"


/*
  Notes:

  - Each style has optional parent; colors with value FXRGBA(0,0,0,0) are
    inherited from the parent; this way, sub-styles are possible.
  - Have an option to beep when the search wraps back to the top of the file.
  - When entire lines are highlighted through triple click and then dragged, a
    drop at the start of the destination line would seem more natural.
  - A more serious problem occurs when you undo a drag and drop.  If you undo,
    (using ctrl-Z for example), the paste is reversed, but not the cut.  You have
    to hit undo again to reverse the cut.  It would be far more natural to have
    the "combination" type operations, such as a move, (cut then paste), be
    somehow recorded as a single operation in the undo system.
  - Would be nice if we could remember not only bookmarks, but also window
    size/position based on file name (see idea about sessions below).
  - Close last window just saves text, then starts new document in last window;
    in other words, only quit will terminate app.
  - Maybe FXText should have its own accelerator table so that key bindings
    may be changed.
  - Would be nice to save as HTML.
  - Sessions. When reloading session, restore all windows (& positions)
    that were open last time when in that session.
    Info of sessions is in registry
  - Command line option --sesssion <name> to open session instead of single
    file.
  - Master syntax rule should be explicitly created.  Simplifies parser.
  - Master syntax rule NOT style index 0.  So you can set normal colors
    on a per-language basis...
  - Need option for tabbed interface.
  - Ability to open multiple files at once in open-panel.
  - Need default set of styles built-in to code, we can now parse this
    due to new SyntaxParser being able to parse everything from a big
    string!
  - Some way to inherit style attributes, and a way to edit them.
  - Perhaps change registry representation of style colors.
  - We can (temporarily) colorize result of find/replace search pattern,
    simply by writing the style buffer with some value.  This would
    require a small tweak: add one extra style entry at the end for
    this purpose, use only temporarily (save old style buffer temporarily).
  - For this, we should really remove search stuff from FXText widget
    proper.
  - Option to read/write BOM at start when load/save from disk.
  - Comment/uncomment regex in Syntax object.  This would make this
    language-independent. (1) snap selection, (2) execute regex, (3)
    replace selection with result.
*/

#define CLOCKTIMER      1000000000      // Blink rate for corner clock
#define RESTYLEJUMP     80              // Restyling back-off
#define MAXFILESIZE     1000000000      // Limit files to this when loading

/*******************************************************************************/

// Map
FXDEFMAP(TextWindow) TextWindowMap[]={
  FXMAPFUNC(SEL_UPDATE,0,TextWindow::onUpdate),
  FXMAPFUNC(SEL_FOCUSIN,0,TextWindow::onFocusIn),
  FXMAPFUNC(SEL_TIMEOUT,TextWindow::ID_CLOCKTIME,TextWindow::onClock),
  FXMAPFUNC(SEL_FOCUSIN,TextWindow::ID_TEXT,TextWindow::onTextFocus),
  FXMAPFUNC(SEL_INSERTED,TextWindow::ID_TEXT,TextWindow::onTextInserted),
  FXMAPFUNC(SEL_REPLACED,TextWindow::ID_TEXT,TextWindow::onTextReplaced),
  FXMAPFUNC(SEL_DELETED,TextWindow::ID_TEXT,TextWindow::onTextDeleted),
  FXMAPFUNC(SEL_DND_DROP,TextWindow::ID_TEXT,TextWindow::onTextDNDDrop),
  FXMAPFUNC(SEL_DND_MOTION,TextWindow::ID_TEXT,TextWindow::onTextDNDMotion),
  FXMAPFUNC(SEL_QUERY_TIP,TextWindow::ID_TEXT,TextWindow::onQueryTextTip),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,TextWindow::ID_TEXT,TextWindow::onTextRightMouse),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,TextWindow::ID_LOGGER,TextWindow::onLoggerRightMouse),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ABOUT,TextWindow::onCmdAbout),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_HELP,TextWindow::onCmdHelp),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_NEW,TextWindow::onCmdNew),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_NEW_FILE,TextWindow::onCmdNewFile),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_OPEN,TextWindow::onCmdOpen),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_OPEN_SELECTED,TextWindow::onCmdOpenSelected),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_OPEN_TREE,TextWindow::onCmdOpenTree),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_OPEN_RECENT,TextWindow::onCmdOpenRecent),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SWITCH,TextWindow::onCmdSwitch),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_RELOAD,TextWindow::onCmdReload),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_RELOAD,TextWindow::onUpdReload),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SAVE,TextWindow::onCmdSave),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SAVE,TextWindow::onUpdSave),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SAVEAS,TextWindow::onCmdSaveAs),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SAVETO,TextWindow::onCmdSaveTo),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_INSERT_FILE,TextWindow::onUpdIsEditable),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_INSERT_FILE,TextWindow::onCmdInsertFile),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_EXTRACT_FILE,TextWindow::onUpdHasSelection),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_EXTRACT_FILE,TextWindow::onCmdExtractFile),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_FONT,TextWindow::onCmdFont),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_PRINT,TextWindow::onCmdPrint),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_BACK,TextWindow::onCmdTextBackColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_BACK,TextWindow::onCmdTextBackColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_BACK,TextWindow::onUpdTextBackColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_FORE,TextWindow::onCmdTextForeColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_FORE,TextWindow::onCmdTextForeColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_FORE,TextWindow::onUpdTextForeColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_SELBACK,TextWindow::onCmdTextSelBackColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_SELBACK,TextWindow::onCmdTextSelBackColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_SELBACK,TextWindow::onUpdTextSelBackColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_SELFORE,TextWindow::onCmdTextSelForeColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_SELFORE,TextWindow::onCmdTextSelForeColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_SELFORE,TextWindow::onUpdTextSelForeColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_HILITEBACK,TextWindow::onCmdTextHiliteBackColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_HILITEBACK,TextWindow::onCmdTextHiliteBackColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_HILITEBACK,TextWindow::onUpdTextHiliteBackColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_HILITEFORE,TextWindow::onCmdTextHiliteForeColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_HILITEFORE,TextWindow::onCmdTextHiliteForeColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_HILITEFORE,TextWindow::onUpdTextHiliteForeColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_CURSOR,TextWindow::onCmdTextCursorColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_CURSOR,TextWindow::onCmdTextCursorColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_CURSOR,TextWindow::onUpdTextCursorColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_ACTIVEBACK,TextWindow::onCmdTextActBackColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_ACTIVEBACK,TextWindow::onCmdTextActBackColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_ACTIVEBACK,TextWindow::onUpdTextActBackColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_NUMBACK,TextWindow::onCmdTextBarColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_NUMBACK,TextWindow::onCmdTextBarColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_NUMBACK,TextWindow::onUpdTextBarColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_NUMFORE,TextWindow::onCmdTextNumberColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_TEXT_NUMFORE,TextWindow::onCmdTextNumberColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_NUMFORE,TextWindow::onUpdTextNumberColor),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_DIR_BACK,TextWindow::onCmdDirBackColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_DIR_BACK,TextWindow::onCmdDirBackColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_DIR_BACK,TextWindow::onUpdDirBackColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_DIR_FORE,TextWindow::onCmdDirForeColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_DIR_FORE,TextWindow::onCmdDirForeColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_DIR_FORE,TextWindow::onUpdDirForeColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_DIR_SELBACK,TextWindow::onCmdDirSelBackColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_DIR_SELBACK,TextWindow::onCmdDirSelBackColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_DIR_SELBACK,TextWindow::onUpdDirSelBackColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_DIR_SELFORE,TextWindow::onCmdDirSelForeColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_DIR_SELFORE,TextWindow::onCmdDirSelForeColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_DIR_SELFORE,TextWindow::onUpdDirSelForeColor),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_DIR_LINES,TextWindow::onCmdDirLineColor),
  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_DIR_LINES,TextWindow::onCmdDirLineColor),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_DIR_LINES,TextWindow::onUpdDirLineColor),

  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TOGGLE_WRAP,TextWindow::onUpdWrap),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TOGGLE_WRAP,TextWindow::onCmdWrap),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_FIXED_WRAP,TextWindow::onUpdWrapFixed),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_FIXED_WRAP,TextWindow::onCmdWrapFixed),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_STRIP_CR,TextWindow::onUpdStripReturns),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_STRIP_CR,TextWindow::onCmdStripReturns),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_STRIP_SP,TextWindow::onUpdStripSpaces),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_STRIP_SP,TextWindow::onCmdStripSpaces),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_APPEND_CR,TextWindow::onUpdAppendCarriageReturn),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_APPEND_CR,TextWindow::onCmdAppendCarriageReturn),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_APPEND_NL,TextWindow::onUpdAppendNewline),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_APPEND_NL,TextWindow::onCmdAppendNewline),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_FILEFILTER,TextWindow::onCmdFilter),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_OVERSTRIKE,TextWindow::onUpdOverstrike),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_READONLY,TextWindow::onUpdReadOnly),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TABMODE,TextWindow::onUpdTabMode),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_PREFERENCES,TextWindow::onCmdPreferences),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TABCOLUMNS,TextWindow::onCmdTabColumns),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TABCOLUMNS,TextWindow::onUpdTabColumns),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_DELIMITERS,TextWindow::onCmdDelimiters),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_DELIMITERS,TextWindow::onUpdDelimiters),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_WRAPCOLUMNS,TextWindow::onCmdWrapColumns),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_WRAPCOLUMNS,TextWindow::onUpdWrapColumns),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_MODELINE,TextWindow::onCmdModeline),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_MODELINE,TextWindow::onUpdModeline),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_AUTOINDENT,TextWindow::onCmdAutoIndent),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_AUTOINDENT,TextWindow::onUpdAutoIndent),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_INSERTTABS,TextWindow::onCmdInsertTabs),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_INSERTTABS,TextWindow::onUpdInsertTabs),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_BRACEMATCH,TextWindow::onCmdBraceMatch),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_BRACEMATCH,TextWindow::onUpdBraceMatch),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_BRACEMATCHTIME,TextWindow::onCmdBraceMatchTime),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_BRACEMATCHTIME,TextWindow::onUpdBraceMatchTime),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_BRACEMATCHSTAY,TextWindow::onCmdBraceMatchStay),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_BRACEMATCHSTAY,TextWindow::onUpdBraceMatchStay),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_WHEELADJUST,TextWindow::onUpdWheelAdjust),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_WHEELADJUST,TextWindow::onCmdWheelAdjust),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SAVEVIEWS,TextWindow::onUpdSaveViews),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SAVEVIEWS,TextWindow::onCmdSaveViews),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SHOWACTIVE,TextWindow::onUpdShowActive),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SHOWACTIVE,TextWindow::onCmdShowActive),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TEXT_LINENUMS,TextWindow::onUpdLineNumbers),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TEXT_LINENUMS,TextWindow::onCmdLineNumbers),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_WARNCHANGED,TextWindow::onUpdWarnChanged),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_WARNCHANGED,TextWindow::onCmdWarnChanged),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TOGGLE_BROWSER,TextWindow::onCmdToggleBrowser),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TOGGLE_BROWSER,TextWindow::onUpdToggleBrowser),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_TOGGLE_DOTFILES,TextWindow::onCmdToggleHidden),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_TOGGLE_DOTFILES,TextWindow::onUpdToggleHidden),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SEARCHPATHS,TextWindow::onCmdSearchPaths),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SEARCHPATHS,TextWindow::onUpdSearchPaths),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SAVE_SETTINGS,TextWindow::onCmdSaveSettings),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_FINDFILES,TextWindow::onCmdFindInFiles),

  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_INSERTPOINT,TextWindow::onUpdInsertPoint),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_INSERTPOINT,TextWindow::onCmdInsertPoint),

  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SET_MARK,TextWindow::onUpdSetMark),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SET_MARK,TextWindow::onCmdSetMark),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_NEXT_MARK,TextWindow::onUpdNextMark),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_NEXT_MARK,TextWindow::onCmdNextMark),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_PREV_MARK,TextWindow::onUpdPrevMark),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_PREV_MARK,TextWindow::onCmdPrevMark),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_MARK_0,TextWindow::ID_MARK_9,TextWindow::onUpdGotoMark),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_MARK_0,TextWindow::ID_MARK_9,TextWindow::onCmdGotoMark),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_DEL_MARK,TextWindow::onUpdDelMark),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_DEL_MARK,TextWindow::onCmdDelMark),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_CLEAR_MARKS,TextWindow::onUpdClearMarks),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_CLEAR_MARKS,TextWindow::onCmdClearMarks),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SAVEMARKS,TextWindow::onUpdSaveMarks),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SAVEMARKS,TextWindow::onCmdSaveMarks),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SHELL_DIALOG,TextWindow::onCmdShellDialog),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SHELL_DIALOG,TextWindow::onUpdShellDialog),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SHELL_COMMAND,TextWindow::onCmdShellCommand),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SHELL_COMMAND,TextWindow::onUpdShellCommand),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SHELL_FILTER,TextWindow::onCmdShellFilter),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SHELL_FILTER,TextWindow::onUpdShellFilter),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SHELL_CANCEL,TextWindow::onCmdShellCancel),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SHELL_CANCEL,TextWindow::onUpdShellCancel),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SHELL_OUTPUT,TextWindow::onCmdShellOutput),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SHELL_ERROR,TextWindow::onCmdShellError),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SHELL_DONE,TextWindow::onCmdShellDone),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_CUST_CMD0,TextWindow::ID_CUST_CMDF,TextWindow::onUpdShellMenu),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_CUST_CMD0,TextWindow::ID_CUST_CMDF,TextWindow::onCmdShellMenu),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_EXPRESSION,TextWindow::onCmdExpression),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_EXPRESSION,TextWindow::onUpdExpression),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_URL_ENCODE,TextWindow::onCmdURLEncode),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_URL_ENCODE,TextWindow::onUpdURLCoding),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_URL_DECODE,TextWindow::onCmdURLDecode),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_URL_DECODE,TextWindow::onUpdURLCoding),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_GOTO_LINE,TextWindow::onCmdGotoLine),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_GOTO_SELECTED,TextWindow::onCmdGotoSelected),

  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SEARCH,TextWindow::onCmdSearch),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_REPLACE,TextWindow::onCmdReplace),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_REPLACE,TextWindow::onUpdIsEditable),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SEARCH_SEL_FORW,TextWindow::onCmdSearchSel),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SEARCH_SEL_BACK,TextWindow::onCmdSearchSel),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SEARCH_NXT_FORW,TextWindow::onCmdSearchNext),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SEARCH_NXT_BACK,TextWindow::onCmdSearchNext),

  FXMAPFUNC(SEL_CHANGED,TextWindow::ID_ISEARCH_TEXT,TextWindow::onChgISearchText),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_TEXT,TextWindow::onCmdISearchText),
  FXMAPFUNC(SEL_KEYPRESS,TextWindow::ID_ISEARCH_TEXT,TextWindow::onKeyISearchText),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_PREV,TextWindow::onCmdISearchPrev),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_NEXT,TextWindow::onCmdISearchNext),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_START,TextWindow::onCmdISearchStart),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_FINISH,TextWindow::onCmdISearchFinish),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_HIST_UP,TextWindow::onCmdISearchHistUp),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_HIST_DN,TextWindow::onCmdISearchHistDn),

  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_ISEARCH_IGNCASE,TextWindow::onUpdISearchCase),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_IGNCASE,TextWindow::onCmdISearchCase),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_ISEARCH_WORDS,TextWindow::onUpdISearchWords),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_WORDS,TextWindow::onCmdISearchWords),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_ISEARCH_REVERSE,TextWindow::onUpdISearchDir),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_REVERSE,TextWindow::onCmdISearchDir),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_ISEARCH_REGEX,TextWindow::onUpdISearchRegex),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_ISEARCH_REGEX,TextWindow::onCmdISearchRegex),

  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_USE_INITIAL_SIZE,TextWindow::onUpdUseInitialSize),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_USE_INITIAL_SIZE,TextWindow::onCmdUseInitialSize),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SET_INITIAL_SIZE,TextWindow::onCmdSetInitialSize),

  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_SYNTAX,TextWindow::onUpdSyntax),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_SYNTAX,TextWindow::onCmdSyntax),
  FXMAPFUNC(SEL_UPDATE,TextWindow::ID_RESTYLE,TextWindow::onUpdRestyle),
  FXMAPFUNC(SEL_COMMAND,TextWindow::ID_RESTYLE,TextWindow::onCmdRestyle),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_WINDOW_1,TextWindow::ID_WINDOW_15,TextWindow::onUpdWindow),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_WINDOW_1,TextWindow::ID_WINDOW_15,TextWindow::onCmdWindow),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_SYNTAX_FIRST,TextWindow::ID_SYNTAX_LAST,TextWindow::onUpdSyntaxSwitch),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_SYNTAX_FIRST,TextWindow::ID_SYNTAX_LAST,TextWindow::onCmdSyntaxSwitch),

  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_TABSELECT_0,TextWindow::ID_TABSELECT_8,TextWindow::onUpdTabSelect),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_TABSELECT_0,TextWindow::ID_TABSELECT_8,TextWindow::onCmdTabSelect),

  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_NORMAL_FG_FIRST,TextWindow::ID_STYLE_NORMAL_FG_LAST,TextWindow::onCmdStyleNormalFG),
  FXMAPFUNCS(SEL_CHANGED,TextWindow::ID_STYLE_NORMAL_FG_FIRST,TextWindow::ID_STYLE_NORMAL_FG_LAST,TextWindow::onCmdStyleNormalFG),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_NORMAL_FG_FIRST,TextWindow::ID_STYLE_NORMAL_FG_LAST,TextWindow::onUpdStyleNormalFG),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_NORMAL_BG_FIRST,TextWindow::ID_STYLE_NORMAL_BG_LAST,TextWindow::onCmdStyleNormalBG),
  FXMAPFUNCS(SEL_CHANGED,TextWindow::ID_STYLE_NORMAL_BG_FIRST,TextWindow::ID_STYLE_NORMAL_BG_LAST,TextWindow::onCmdStyleNormalBG),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_NORMAL_BG_FIRST,TextWindow::ID_STYLE_NORMAL_BG_LAST,TextWindow::onUpdStyleNormalBG),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_SELECT_FG_FIRST,TextWindow::ID_STYLE_SELECT_FG_LAST,TextWindow::onCmdStyleSelectFG),
  FXMAPFUNCS(SEL_CHANGED,TextWindow::ID_STYLE_SELECT_FG_FIRST,TextWindow::ID_STYLE_SELECT_FG_LAST,TextWindow::onCmdStyleSelectFG),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_SELECT_FG_FIRST,TextWindow::ID_STYLE_SELECT_FG_LAST,TextWindow::onUpdStyleSelectFG),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_SELECT_BG_FIRST,TextWindow::ID_STYLE_SELECT_BG_LAST,TextWindow::onCmdStyleSelectBG),
  FXMAPFUNCS(SEL_CHANGED,TextWindow::ID_STYLE_SELECT_BG_FIRST,TextWindow::ID_STYLE_SELECT_BG_LAST,TextWindow::onCmdStyleSelectBG),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_SELECT_BG_FIRST,TextWindow::ID_STYLE_SELECT_BG_LAST,TextWindow::onUpdStyleSelectBG),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_HILITE_FG_FIRST,TextWindow::ID_STYLE_HILITE_FG_LAST,TextWindow::onCmdStyleHiliteFG),
  FXMAPFUNCS(SEL_CHANGED,TextWindow::ID_STYLE_HILITE_FG_FIRST,TextWindow::ID_STYLE_HILITE_FG_LAST,TextWindow::onCmdStyleHiliteFG),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_HILITE_FG_FIRST,TextWindow::ID_STYLE_HILITE_FG_LAST,TextWindow::onUpdStyleHiliteFG),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_HILITE_BG_FIRST,TextWindow::ID_STYLE_HILITE_BG_LAST,TextWindow::onCmdStyleHiliteBG),
  FXMAPFUNCS(SEL_CHANGED,TextWindow::ID_STYLE_HILITE_BG_FIRST,TextWindow::ID_STYLE_HILITE_BG_LAST,TextWindow::onCmdStyleHiliteBG),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_HILITE_BG_FIRST,TextWindow::ID_STYLE_HILITE_BG_LAST,TextWindow::onUpdStyleHiliteBG),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_ACTIVE_BG_FIRST,TextWindow::ID_STYLE_ACTIVE_BG_LAST,TextWindow::onCmdStyleActiveBG),
  FXMAPFUNCS(SEL_CHANGED,TextWindow::ID_STYLE_ACTIVE_BG_FIRST,TextWindow::ID_STYLE_ACTIVE_BG_LAST,TextWindow::onCmdStyleActiveBG),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_ACTIVE_BG_FIRST,TextWindow::ID_STYLE_ACTIVE_BG_LAST,TextWindow::onUpdStyleActiveBG),

  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_UNDERLINE_FIRST,TextWindow::ID_STYLE_UNDERLINE_LAST,TextWindow::onCmdStyleUnderline),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_UNDERLINE_FIRST,TextWindow::ID_STYLE_UNDERLINE_LAST,TextWindow::onUpdStyleUnderline),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_STRIKEOUT_FIRST,TextWindow::ID_STYLE_STRIKEOUT_LAST,TextWindow::onCmdStyleStrikeout),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_STRIKEOUT_FIRST,TextWindow::ID_STYLE_STRIKEOUT_LAST,TextWindow::onUpdStyleStrikeout),
  FXMAPFUNCS(SEL_COMMAND,TextWindow::ID_STYLE_BOLD_FIRST,TextWindow::ID_STYLE_BOLD_LAST,TextWindow::onCmdStyleBold),
  FXMAPFUNCS(SEL_UPDATE,TextWindow::ID_STYLE_BOLD_FIRST,TextWindow::ID_STYLE_BOLD_LAST,TextWindow::onUpdStyleBold),
  };


// Object implementation
FXIMPLEMENT(TextWindow,FXMainWindow,TextWindowMap,ARRAYNUMBER(TextWindowMap))

/*******************************************************************************/

// Make some windows
TextWindow::TextWindow(Adie* a):FXMainWindow(a,"Adie",nullptr,nullptr,DECOR_ALL,0,0,850,600,0,0),mrufiles(a){

  // Add to list of windows
  getApp()->appendWindow(this);

  // Default font
  font=nullptr;

  // Application icons
  setIcon(getApp()->bigicon);
  setMiniIcon(getApp()->smallicon);

  // Status bar
  statusbar=new FXStatusBar(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|STATUSBAR_WITH_DRAGCORNER|FRAME_RAISED);

  // Sites where to dock
  topdock=new FXDockSite(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
  bottomdock=new FXDockSite(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
  leftdock=new FXDockSite(this,LAYOUT_SIDE_LEFT|LAYOUT_FILL_Y);
  rightdock=new FXDockSite(this,LAYOUT_SIDE_RIGHT|LAYOUT_FILL_Y);

  // Make menu bar
  dragshell1=new FXToolBarShell(this,FRAME_RAISED);
  menubar=new FXMenuBar(topdock,dragshell1,LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
  new FXToolBarGrip(menubar,menubar,FXMenuBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);

  // Tool bar
  dragshell2=new FXToolBarShell(this,FRAME_RAISED);
  toolbar=new FXToolBar(topdock,dragshell2,LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
  new FXToolBarGrip(toolbar,toolbar,FXToolBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);

  // Search bar
  dragshell3=new FXToolBarShell(this,FRAME_RAISED);
  searchbar=new FXToolBar(bottomdock,dragshell3,LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
  searchbar->allowedSides(FXDockBar::ALLOW_HORIZONTAL);
  new FXToolBarGrip(searchbar,searchbar,FXToolBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);

  // Splitter
  FXSplitter* splitter=new FXSplitter(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y|SPLITTER_TRACKING);

  // Sunken border for tree
  treebox=new FXVerticalFrame(splitter,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

  // Make tree
  FXHorizontalFrame* treeframe=new FXHorizontalFrame(treebox,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  dirlist=new FXDirList(treeframe,this,ID_OPEN_TREE,DIRLIST_SHOWFILES|DIRLIST_NO_OWN_ASSOC|TREELIST_BROWSESELECT|TREELIST_SHOWS_LINES|TREELIST_SHOWS_BOXES|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  dirlist->setAssociations(getApp()->getFileAssociations(),false);
  dirlist->setDraggableFiles(false);
  FXHorizontalFrame* filterframe=new FXHorizontalFrame(treebox,LAYOUT_FILL_X,0,0,0,0, 4,0,0,4);
  new FXLabel(filterframe,tr("Filter:"),nullptr,LAYOUT_CENTER_Y);
  filter=new FXComboBox(filterframe,25,this,ID_FILEFILTER,COMBOBOX_STATIC|LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK);
  filter->setNumVisible(4);
  new FXToggleButton(filterframe,tr("\tShow hidden files\tShow hidden files and directories."),tr("\tHide Hidden Files\tHide hidden files and directories."),getApp()->hiddenicon,getApp()->shownicon,this,ID_TOGGLE_DOTFILES,TOGGLEBUTTON_TOOLBAR|FRAME_RAISED,0,0,0,0, 3,3,3,3);

  FXSplitter* subsplitter=new FXSplitter(splitter,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|LAYOUT_FILL_Y|SPLITTER_VERTICAL|SPLITTER_REVERSED|SPLITTER_TRACKING);

  // Editor frame and text widgets
  editorframe=new FXHorizontalFrame(subsplitter,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  editor=new FXText(editorframe,this,ID_TEXT,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  editor->setHiliteMatchTime(2000000000);
  editor->setBarColumns(6);

  // Logger frame and logger widget
  loggerframe=new FXHorizontalFrame(subsplitter,LAYOUT_SIDE_BOTTOM|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
  logger=new FXText(loggerframe,this,ID_LOGGER,LAYOUT_FILL_X|LAYOUT_FILL_Y|TEXT_READONLY);
  logger->setVisibleRows(6);

  // Create status bar
  createStatusbar();

  // Create menu bar
  createMenubar();

  // Create tool bar
  createToolbar();

  // Create search bar
  createSearchbar();

  // Initialize bookmarks
  clearBookmarks();

  // Set status
  setStatusMessage(tr("Ready."));

  // Initial setting
  syntax=nullptr;

  // Recent files
  mrufiles.setTarget(this);
  mrufiles.setSelector(ID_OPEN_RECENT);
  mrufiles.setMaxFiles(15);

  // Initialize file name
  filename="untitled";
  filenameset=false;
  filetime=0;

  // Update insert point
  insertpoint=0;

  // Initialize other stuff
  searchpaths="/usr/include";
  setPatternList("All Files (*)");
  setCurrentPattern(0);
  shellCommand=nullptr;
  replaceStart=0;
  replaceEnd=0;
  initialwidth=640;
  initialheight=480;
  isearchReplace=false;
  isearchIndex=-1;
  isearchpos=-1;
  searchflags=SEARCH_FORWARD;
  searching=false;
  showsearchbar=false;
  showlogger=false;
  initialsize=true;
  colorize=false;
  stripcr=true;
  stripsp=false;
  appendcr=false;
  appendnl=true;
  saveviews=false;
  savemarks=false;
  warnchanged=false;
  modeline=false;
  undolist.mark();
  }


// Create main menu bar
void TextWindow::createMenubar(){

  // File menu
  filemenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&File"),nullptr,filemenu);

  // File Menu entries
  new FXMenuCommand(filemenu,tr("&New...\tCtl-N\tCreate new file."),getApp()->newicon,this,ID_NEW);
  new FXMenuCommand(filemenu,tr("New &File...\tShift-Ctl-N\tCreate new file on disk."),getApp()->newfileicon,this,ID_NEW_FILE);
  new FXMenuCommand(filemenu,tr("&Open...\tCtl-O\tOpen a file."),getApp()->openicon,this,ID_OPEN);
  new FXMenuCommand(filemenu,tr("S&witch...\t\tSwitch to other file."),getApp()->switchicon,this,ID_SWITCH);
  new FXMenuCommand(filemenu,tr("&Reload...\t\tReload file from disk."),getApp()->reloadicon,this,ID_RELOAD);
  new FXMenuCommand(filemenu,tr("Open Selected...  \tCtl-Y\tOpen selected file."),getApp()->openselicon,this,ID_OPEN_SELECTED);
  new FXMenuCommand(filemenu,tr("&Save\tCtl-S\tSave file to disk."),getApp()->saveicon,this,ID_SAVE);
  new FXMenuCommand(filemenu,tr("Save &As...\tShift-Ctl-S\tSave under a different file name."),getApp()->saveasicon,this,ID_SAVEAS);
  new FXMenuCommand(filemenu,tr("Save &To...\tShift-Ctl-T\tSave copy to a different file name."),getApp()->savetoicon,this,ID_SAVETO);
  new FXMenuCommand(filemenu,tr("&Close\tCtl-W\tClose document."),getApp()->closeicon,this,ID_CLOSE);
  new FXMenuSeparator(filemenu);
  new FXMenuCommand(filemenu,tr("Insert from file...\t\tInsert text from file."),getApp()->importicon,this,ID_INSERT_FILE);
  new FXMenuCommand(filemenu,tr("Extract to file...\t\tExtract text to file."),getApp()->exporticon,this,ID_EXTRACT_FILE);
  new FXMenuCommand(filemenu,tr("&Print...\tCtl-P\tPrint document."),getApp()->printicon,this,ID_PRINT);
  new FXMenuCheck(filemenu,tr("&Editable\t\tFile is editable."),editor,FXText::ID_TOGGLE_EDITABLE);

  // Recent file menu; this automatically hides if there are no files
  new FXMenuSeparator(filemenu,&mrufiles,FXRecentFiles::ID_ANYFILES);
  new FXMenuCommand(filemenu,"&1",nullptr,&mrufiles,FXRecentFiles::ID_FILE_1);
  new FXMenuCommand(filemenu,"&2",nullptr,&mrufiles,FXRecentFiles::ID_FILE_2);
  new FXMenuCommand(filemenu,"&3",nullptr,&mrufiles,FXRecentFiles::ID_FILE_3);
  new FXMenuCommand(filemenu,"&4",nullptr,&mrufiles,FXRecentFiles::ID_FILE_4);
  new FXMenuCommand(filemenu,"&5",nullptr,&mrufiles,FXRecentFiles::ID_FILE_5);
  new FXMenuCommand(filemenu,"&6",nullptr,&mrufiles,FXRecentFiles::ID_FILE_6);
  new FXMenuCommand(filemenu,"&7",nullptr,&mrufiles,FXRecentFiles::ID_FILE_7);
  new FXMenuCommand(filemenu,"&8",nullptr,&mrufiles,FXRecentFiles::ID_FILE_8);
  new FXMenuCommand(filemenu,"&9",nullptr,&mrufiles,FXRecentFiles::ID_FILE_9);
  new FXMenuCommand(filemenu,"1&0",nullptr,&mrufiles,FXRecentFiles::ID_FILE_10);
  new FXMenuCommand(filemenu,"11",nullptr,&mrufiles,FXRecentFiles::ID_FILE_11);
  new FXMenuCommand(filemenu,"12",nullptr,&mrufiles,FXRecentFiles::ID_FILE_12);
  new FXMenuCommand(filemenu,"13",nullptr,&mrufiles,FXRecentFiles::ID_FILE_13);
  new FXMenuCommand(filemenu,"14",nullptr,&mrufiles,FXRecentFiles::ID_FILE_14);
  new FXMenuCommand(filemenu,"15",nullptr,&mrufiles,FXRecentFiles::ID_FILE_15);
  new FXMenuCommand(filemenu,tr("&Clear Recent Files"),nullptr,&mrufiles,FXRecentFiles::ID_CLEAR);
  new FXMenuSeparator(filemenu,&mrufiles,FXRecentFiles::ID_ANYFILES);
  new FXMenuCommand(filemenu,tr("&Quit\tCtl-Q\tQuit program."),getApp()->quiticon,getApp(),Adie::ID_CLOSEALL);

  // Edit Menu
  editmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Edit"),nullptr,editmenu);

  // Edit Menu entries
  new FXMenuCommand(editmenu,tr("&Undo\tCtl-Z\tUndo last change."),getApp()->undoicon,&undolist,FXUndoList::ID_UNDO);
  new FXMenuCommand(editmenu,tr("&Redo\tCtl-Shift-Z\tRedo last undo."),getApp()->redoicon,&undolist,FXUndoList::ID_REDO);
  new FXMenuCommand(editmenu,tr("Undo all\tAlt-Z\tUndo all."),nullptr,&undolist,FXUndoList::ID_UNDO_ALL);
  new FXMenuCommand(editmenu,tr("Redo all\tShift-Alt-Z\tRedo all."),nullptr,&undolist,FXUndoList::ID_REDO_ALL);
  new FXMenuCommand(editmenu,tr("Revert\tAlt-R\tRevert to last-saved."),nullptr,&undolist,FXUndoList::ID_REVERT);
  new FXMenuSeparator(editmenu);
  new FXMenuCommand(editmenu,tr("&Copy\tCtl-C\tCopy selection to clipboard."),getApp()->copyicon,editor,FXText::ID_COPY_SEL);
  new FXMenuCommand(editmenu,tr("Cu&t\tCtl-X\tCut selection to clipboard."),getApp()->cuticon,editor,FXText::ID_CUT_SEL);
  new FXMenuCommand(editmenu,tr("&Paste\tCtl-V\tPaste from clipboard."),getApp()->pasteicon,editor,FXText::ID_PASTE_SEL);
  new FXMenuCommand(editmenu,tr("&Delete\t\tDelete selection."),getApp()->deleteicon,editor,FXText::ID_DELETE_SEL);
  new FXMenuSeparator(editmenu);
  new FXMenuCommand(editmenu,tr("Expression\t\tEvaluate selected expression."),getApp()->expressicon,this,ID_EXPRESSION);
  new FXMenuCommand(editmenu,tr("URL Encode\t\tEncode url special characters."),nullptr,this,ID_URL_ENCODE);
  new FXMenuCommand(editmenu,tr("URL Decode\t\tDecode url special characters."),nullptr,this,ID_URL_DECODE);
  new FXMenuCommand(editmenu,tr("Duplicate Line\tCtl-D\tDuplicate current line."),nullptr,editor,FXText::ID_COPY_LINE);
  new FXMenuCommand(editmenu,tr("Move line up\tCtl-Shift-<\tMove current line up."),nullptr,editor,FXText::ID_MOVE_LINE_UP);
  new FXMenuCommand(editmenu,tr("Move line down\tCtl-Shift->\tMove current line down."),nullptr,editor,FXText::ID_MOVE_LINE_DOWN);
  new FXMenuCommand(editmenu,tr("Lo&wer-case\tCtl-U\tChange to lower case."),getApp()->lowercaseicon,editor,FXText::ID_LOWER_CASE);
  new FXMenuCommand(editmenu,tr("Upp&er-case\tCtl-Shift-U\tChange to upper case."),getApp()->uppercaseicon,editor,FXText::ID_UPPER_CASE);
  new FXMenuCommand(editmenu,tr("Clean indent\t\tClean indentation to either all tabs or all spaces."),nullptr,editor,FXText::ID_CLEAN_INDENT);
  new FXMenuCommand(editmenu,tr("Shift left\tCtl-[\tShift text left."),getApp()->shiftlefticon,editor,FXText::ID_SHIFT_LEFT);
  new FXMenuCommand(editmenu,tr("Shift right\tCtl-]\tShift text right."),getApp()->shiftrighticon,editor,FXText::ID_SHIFT_RIGHT);
  new FXMenuCommand(editmenu,tr("Shift tab left\tAlt-[\tShift text left one tab position."),getApp()->shiftlefticon,editor,FXText::ID_SHIFT_TABLEFT);
  new FXMenuCommand(editmenu,tr("Shift tab right\tAlt-]\tShift text right one tab position."),getApp()->shiftrighticon,editor,FXText::ID_SHIFT_TABRIGHT);

  // These were the old bindings; keeping them for now...
  getAccelTable()->addAccel("Ctl-9",editor,FXSEL(SEL_COMMAND,FXText::ID_SHIFT_LEFT));
  getAccelTable()->addAccel("Ctl-0",editor,FXSEL(SEL_COMMAND,FXText::ID_SHIFT_RIGHT));

  // Goto Menu
  gotomenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Goto"),nullptr,gotomenu);

  // Goto Menu entries
  new FXMenuCommand(gotomenu,tr("&Goto...\tCtl-L\tGoto line number."),nullptr,this,ID_GOTO_LINE);
  new FXMenuCommand(gotomenu,tr("Goto selected...\tCtl-E\tGoto selected line number."),nullptr,this,ID_GOTO_SELECTED);
  new FXMenuSeparator(gotomenu);
  new FXMenuCommand(gotomenu,tr("Goto {..\tShift-Ctl-{\tGoto start of enclosing block."),nullptr,editor,FXText::ID_LEFT_BRACE);
  new FXMenuCommand(gotomenu,tr("Goto ..}\tShift-Ctl-}\tGoto end of enclosing block."),nullptr,editor,FXText::ID_RIGHT_BRACE);
  new FXMenuCommand(gotomenu,tr("Goto (..\tShift-Ctl-(\tGoto start of enclosing expression."),nullptr,editor,FXText::ID_LEFT_PAREN);
  new FXMenuCommand(gotomenu,tr("Goto ..)\tShift-Ctl-)\tGoto end of enclosing expression."),nullptr,editor,FXText::ID_RIGHT_PAREN);
  new FXMenuSeparator(gotomenu);
  new FXMenuCommand(gotomenu,tr("Goto matching (..)\tCtl-M\tGoto matching brace or parenthesis."),nullptr,editor,FXText::ID_GOTO_MATCHING);
  new FXMenuSeparator(gotomenu);
  new FXMenuCommand(gotomenu,tr("&Insert point\t\tGo back to position of last edit."),getApp()->pointicon,this,ID_INSERTPOINT);
  new FXMenuCommand(gotomenu,tr("&Set bookmark\tAlt-B\tSet bookmark at cursor location."),getApp()->bookseticon,this,ID_SET_MARK);
  new FXMenuCommand(gotomenu,tr("&Next bookmark\tAlt-N\tMove cursor to next bookmark."),getApp()->booknexticon,this,ID_NEXT_MARK);
  new FXMenuCommand(gotomenu,tr("&Previous bookmark\tAlt-P\tMove cursor to previous bookmark."),getApp()->bookprevicon,this,ID_PREV_MARK);
  new FXMenuCommand(gotomenu,tr("&Delete bookmark\tAlt-D\tDelete bookmark at cursor."),getApp()->bookdelicon,this,ID_DEL_MARK);
  new FXMenuCommand(gotomenu,tr("&Clear all bookmarks\tAlt-C\tClear all bookmarks."),getApp()->bookclricon,this,ID_CLEAR_MARKS);

  // Search Menu
  searchmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Search"),nullptr,searchmenu);

  // Search Menu entries
  new FXMenuCommand(searchmenu,tr("Select matching (..)\tShift-Ctl-M\tSelect matching brace or parenthesis."),nullptr,editor,FXText::ID_SELECT_MATCHING);
  new FXMenuCommand(searchmenu,tr("Select block {..}\tShift-Alt-{\tSelect enclosing block."),nullptr,editor,FXText::ID_SELECT_BRACE);
  new FXMenuCommand(searchmenu,tr("Select block {..}\tShift-Alt-}\tSelect enclosing block."),nullptr,editor,FXText::ID_SELECT_BRACE);
  new FXMenuCommand(searchmenu,tr("Select expression (..)\tShift-Alt-(\tSelect enclosing parentheses."),nullptr,editor,FXText::ID_SELECT_PAREN);
  new FXMenuCommand(searchmenu,tr("Select expression (..)\tShift-Alt-)\tSelect enclosing parentheses."),nullptr,editor,FXText::ID_SELECT_PAREN);
  new FXMenuSeparator(searchmenu);
  new FXMenuCommand(searchmenu,tr("Incremental search\tCtl-I\tSearch for a string."),nullptr,this,ID_ISEARCH_START);
  new FXMenuCommand(searchmenu,tr("Search &Files\tShift-Ctl-F\tSearch files for a string."),getApp()->searchfilesicon,this,ID_FINDFILES);
  new FXMenuCommand(searchmenu,tr("Find Backward\tShift-Ctl-G\tSearch backward for another occurrence."),getApp()->searchprevicon,this,ID_SEARCH_NXT_BACK);
  new FXMenuCommand(searchmenu,tr("Find Forward\tCtl-G\tSearch forward for another occurrence."),getApp()->searchnexticon,this,ID_SEARCH_NXT_FORW);
  new FXMenuCommand(searchmenu,tr("Find Backward Selected\tShift-Ctl-H\tSearch backward for selected text."),getApp()->searchprevicon,this,ID_SEARCH_SEL_BACK);
  new FXMenuCommand(searchmenu,tr("Find Forward Selected\tCtl-H\tSearch forward for selected text."),getApp()->searchnexticon,this,ID_SEARCH_SEL_FORW);

  new FXMenuCommand(searchmenu,tr("&Search...\tCtl-F\tSearch with a string pattern."),getApp()->searchicon,this,ID_SEARCH);
  new FXMenuCommand(searchmenu,tr("R&eplace...\tCtl-R\tSearch and replace with a string pattern."),getApp()->replaceicon,this,ID_REPLACE);

  // Accelerators for search
  getAccelTable()->addAccel("F3",this,FXSEL(SEL_COMMAND,ID_SEARCH_NXT_FORW));
  getAccelTable()->addAccel("Shift-F3",this,FXSEL(SEL_COMMAND,ID_SEARCH_NXT_BACK));

  // Syntax menu; it scrolls if we get too many
  syntaxmenu=new FXScrollPane(this,25);
  new FXMenuRadio(syntaxmenu,tr("Plain\t\tNo syntax for this file."),this,ID_SYNTAX_FIRST);
  for(int syn=0; syn<getApp()->numSyntaxes() && syn<100; syn++){
    new FXMenuRadio(syntaxmenu,getApp()->getSyntax(syn)->getName(),this,ID_SYNTAX_FIRST+1+syn);
    }

  // Tabs menu
  tabsmenu=new FXMenuPane(this);
  new FXMenuRadio(tabsmenu,"1",this,ID_TABSELECT_1);
  new FXMenuRadio(tabsmenu,"2",this,ID_TABSELECT_2);
  new FXMenuRadio(tabsmenu,"3",this,ID_TABSELECT_3);
  new FXMenuRadio(tabsmenu,"4",this,ID_TABSELECT_4);
  new FXMenuRadio(tabsmenu,"5",this,ID_TABSELECT_5);
  new FXMenuRadio(tabsmenu,"6",this,ID_TABSELECT_6);
  new FXMenuRadio(tabsmenu,"7",this,ID_TABSELECT_7);
  new FXMenuRadio(tabsmenu,"8",this,ID_TABSELECT_8);

  // Shell Menu
  shellmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Command"),nullptr,shellmenu);

  // Options menu
  new FXMenuCommand(shellmenu,tr("Shell &Command...\t\tExecute a shell command."),getApp()->commandicon,this,ID_SHELL_DIALOG);
  new FXMenuCommand(shellmenu,tr("&Filter Command...\t\tFilter text through shell command."),getApp()->filtericon,this,ID_SHELL_FILTER);
  new FXMenuCommand(shellmenu,tr("Command &Line...\t\tExecute current line as shell command."),getApp()->scripticon,this,ID_SHELL_COMMAND);
  new FXMenuCommand(shellmenu,tr("C&ancel Command\t\tCancel shell command."),getApp()->deleteicon,this,ID_SHELL_CANCEL);
  new FXMenuSeparator(shellmenu);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD1);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD2);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD3);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD4);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD5);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD6);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD7);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD8);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMD9);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMDA);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMDB);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMDC);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMDD);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMDE);
  new FXMenuCommand(shellmenu,FXString::null,nullptr,this,ID_CUST_CMDF);

  // Options Menu
  optionmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Options"),nullptr,optionmenu);

  // Options menu
  new FXMenuCommand(optionmenu,tr("&Preferences...\t\tChange preferences."),getApp()->configicon,this,ID_PREFERENCES);
  new FXMenuCommand(optionmenu,tr("&Font...\t\tChange text font."),getApp()->fontsicon,this,ID_FONT);
  new FXMenuCheck(optionmenu,tr("Insert &tabs\t\tToggle insert tabs."),this,ID_INSERTTABS);
  new FXMenuCheck(optionmenu,tr("&Word wrap\t\tToggle word wrap mode."),this,ID_TOGGLE_WRAP);
  new FXMenuCheck(optionmenu,tr("&Overstrike\t\tToggle overstrike mode."),editor,FXText::ID_TOGGLE_OVERSTRIKE);
  new FXMenuCheck(optionmenu,tr("&Syntax coloring\t\tToggle syntax coloring."),this,ID_SYNTAX);
  new FXMenuCheck(optionmenu,tr("Use initial size\t\tToggle initial window size mode."),this,ID_USE_INITIAL_SIZE);
  new FXMenuCommand(optionmenu,tr("Set initial size\t\tSet current window size as the initial window size."),nullptr,this,ID_SET_INITIAL_SIZE);
  new FXMenuCommand(optionmenu,tr("&Restyle\t\tToggle syntax coloring."),nullptr,this,ID_RESTYLE);
  new FXMenuCascade(optionmenu,tr("Tab stops"),nullptr,tabsmenu);
  new FXMenuCascade(optionmenu,tr("Syntax patterns\t\tSelect syntax for this file."),nullptr,syntaxmenu);
  new FXMenuSeparator(optionmenu);
  new FXMenuCommand(optionmenu,tr("Save Settings\t\tSave settings now."),nullptr,this,ID_SAVE_SETTINGS);

  // View menu
  viewmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&View"),nullptr,viewmenu);

  // View Menu entries
  new FXMenuCheck(viewmenu,tr("&File Browser\t\tDisplay file list."),this,ID_TOGGLE_BROWSER);
  new FXMenuCheck(viewmenu,tr("&Error Logger\t\tDisplay error logger."),loggerframe,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(viewmenu,tr("&Toolbar\t\tDisplay toolbar."),toolbar,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(viewmenu,tr("&Searchbar\t\tDisplay search bar."),searchbar,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(viewmenu,tr("Status &line\t\tDisplay status line."),statusbar,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(viewmenu,tr("Undo Counters\t\tShow undo/redo counters on status line."),undoredoblock,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(viewmenu,tr("Clock\t\tShow clock on status line."),clock,FXWindow::ID_TOGGLESHOWN);

  // Window menu
  windowmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Window"),nullptr,windowmenu);

  // Window menu
  new FXMenuRadio(windowmenu,"&1",this,ID_WINDOW_1);
  new FXMenuRadio(windowmenu,"&2",this,ID_WINDOW_2);
  new FXMenuRadio(windowmenu,"&3",this,ID_WINDOW_3);
  new FXMenuRadio(windowmenu,"&4",this,ID_WINDOW_4);
  new FXMenuRadio(windowmenu,"&5",this,ID_WINDOW_5);
  new FXMenuRadio(windowmenu,"&6",this,ID_WINDOW_6);
  new FXMenuRadio(windowmenu,"&7",this,ID_WINDOW_7);
  new FXMenuRadio(windowmenu,"&8",this,ID_WINDOW_8);
  new FXMenuRadio(windowmenu,"&9",this,ID_WINDOW_9);
  new FXMenuRadio(windowmenu,"1&0",this,ID_WINDOW_10);
  new FXMenuRadio(windowmenu,"11",this,ID_WINDOW_11);
  new FXMenuRadio(windowmenu,"12",this,ID_WINDOW_12);
  new FXMenuRadio(windowmenu,"13",this,ID_WINDOW_13);
  new FXMenuRadio(windowmenu,"14",this,ID_WINDOW_14);
  new FXMenuRadio(windowmenu,"15",this,ID_WINDOW_15);

  // Help menu
  helpmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Help"),nullptr,helpmenu);

  // Help Menu entries
  new FXMenuCommand(helpmenu,tr("&Help...\t\tDisplay help information."),getApp()->helpicon,this,ID_HELP,0);
#if defined(DEBUG)
  new FXMenuCommand(helpmenu,tr("&Dump Widgets...\t\tDump widget tree."),nullptr,getApp(),FXApp::ID_DUMP);
#endif
  new FXMenuSeparator(helpmenu);
  new FXMenuCommand(helpmenu,tr("&About Adie...\t\tDisplay about panel."),getApp()->smallicon,this,ID_ABOUT,0);
  }


// Create tool bar
void TextWindow::createToolbar(){

  // Toobar buttons: File manipulation
  new FXButton(toolbar,tr("\tNew\tCreate new document."),getApp()->newicon,this,ID_NEW,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tNew File\tCreate new file."),getApp()->newfileicon,this,ID_NEW_FILE,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tOpen Selected\tOpen selected file."),getApp()->openselicon,this,ID_OPEN_SELECTED,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tOpen\tOpen a file."),getApp()->openicon,this,ID_OPEN,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tSave\tSave file to disk."),getApp()->saveicon,this,ID_SAVE,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tSave As\tSave under a different file name."),getApp()->saveasicon,this,ID_SAVEAS,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tSave To\tSave copy to a different file name."),getApp()->savetoicon,this,ID_SAVETO,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // Spacer
  new FXSeparator(toolbar,SEPARATOR_GROOVE);

  // Toobar buttons: Print
  new FXButton(toolbar,"\tPrint\tPrint document.",getApp()->printicon,this,ID_PRINT,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // Spacer
  new FXSeparator(toolbar,SEPARATOR_GROOVE);

  // Toobar buttons: Editing
  new FXButton(toolbar,tr("\tCut\tCut selection to clipboard."),getApp()->cuticon,editor,FXText::ID_CUT_SEL,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tCopy\tCopy selection to clipboard."),getApp()->copyicon,editor,FXText::ID_COPY_SEL,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tPaste\tPaste clipboard."),getApp()->pasteicon,editor,FXText::ID_PASTE_SEL,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tDelete\t\tDelete selection."),getApp()->deleteicon,editor,FXText::ID_DELETE_SEL,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // Spacer
  new FXSeparator(toolbar,SEPARATOR_GROOVE);

  // Undo/redo
  new FXButton(toolbar,tr("\tUndo\tUndo last change."),getApp()->undoicon,&undolist,FXUndoList::ID_UNDO,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tRedo\tRedo last undo."),getApp()->redoicon,&undolist,FXUndoList::ID_REDO,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // Spacer
  new FXSeparator(toolbar,SEPARATOR_GROOVE);

  // Search
  new FXButton(toolbar,tr("\tSearch...\tSearch with a string pattern."),getApp()->searchicon,this,ID_SEARCH,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tReplace...\tSearch and replace with a string pattern."),getApp()->replaceicon,this,ID_REPLACE,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tFind previous selected\tSearch previous occurrence of selected text."),getApp()->searchprevicon,this,ID_SEARCH_SEL_BACK,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tFind next selected\tSearch next occurrence of selected text."),getApp()->searchnexticon,this,ID_SEARCH_SEL_FORW,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // Spacer
  new FXSeparator(toolbar,SEPARATOR_GROOVE);

  // Bookmarks
  new FXButton(toolbar,tr("\tSet bookmark\tSet bookmark at cursor location."),getApp()->bookseticon,this,ID_SET_MARK,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tPrevious Bookmark\tMove cursor to previous bookmark."),getApp()->bookprevicon,this,ID_PREV_MARK,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tNext Bookmark\tMove cursor to next bookmark."),getApp()->booknexticon,this,ID_NEXT_MARK,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tDelete bookmark\tDelete bookmark at cursor."),getApp()->bookdelicon,this,ID_DEL_MARK,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // Spacer
  new FXSeparator(toolbar,SEPARATOR_GROOVE);

  new FXButton(toolbar,tr("\tShift left\tShift text left by one."),getApp()->shiftlefticon,editor,FXText::ID_SHIFT_LEFT,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tShift right\tShift text right by one."),getApp()->shiftrighticon,editor,FXText::ID_SHIFT_RIGHT,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // Spacer
  new FXSeparator(toolbar,SEPARATOR_GROOVE);
  new FXButton(toolbar,tr("\tExpression\tEvaluate selected expression."),getApp()->expressicon,this,ID_EXPRESSION,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXToggleButton(toolbar,tr("\tShow Browser\t\tShow file browser."),tr("\tHide Browser\t\tHide file browser."),getApp()->nobrowsericon,getApp()->browsericon,this,ID_TOGGLE_BROWSER,ICON_ABOVE_TEXT|TOGGLEBUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXToggleButton(toolbar,tr("\tShow Logger\t\tShow error logger."),tr("\tHide Logger\t\tHide error logger."),getApp()->nologgericon,getApp()->loggericon,loggerframe,FXWindow::ID_TOGGLESHOWN,ICON_ABOVE_TEXT|TOGGLEBUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXToggleButton(toolbar,tr("\tNo wrap mode."),tr("\tWord wrap mode."),getApp()->nowrapicon,getApp()->wordwrapicon,this,ID_TOGGLE_WRAP,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tPreferences\tDisplay preferences dialog."),getApp()->configicon,this,ID_PREFERENCES,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tFonts\tDisplay font dialog."),getApp()->fontsicon,this,ID_FONT,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

  // Help
  new FXButton(toolbar,tr("\tDisplay help\tDisplay online help information."),getApp()->helpicon,this,ID_HELP,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT);
  }


// Create search bar
void TextWindow::createSearchbar(){
  new FXLabel(searchbar,tr("Search:"),nullptr,LAYOUT_CENTER_Y);
  FXHorizontalFrame* searchbox=new FXHorizontalFrame(searchbar,FRAME_LINE|LAYOUT_FILL_X|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0, 0,0);
  searchtext=new FXTextField(searchbox,50,this,ID_ISEARCH_TEXT,TEXTFIELD_ENTER_ONLY|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 4,4,1,1);
  searchtext->setTipText(tr("Incremental Search (Ctl-I)"));
  searchtext->setHelpText(tr("Incremental search for a string."));
  FXVerticalFrame* searcharrows=new FXVerticalFrame(searchbox,LAYOUT_RIGHT|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);
  FXArrowButton* ar1=new FXArrowButton(searcharrows,this,ID_ISEARCH_HIST_UP,ARROW_UP|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 3,3,2,2);
  FXArrowButton* ar2=new FXArrowButton(searcharrows,this,ID_ISEARCH_HIST_DN,ARROW_DOWN|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 3,3,2,2);
  ar1->setArrowSize(3);
  ar2->setArrowSize(3);
  ar1->setBackColor(searchtext->getBackColor());
  ar2->setBackColor(searchtext->getBackColor());
  new FXButton(searchbar,tr("\tSearch Previous (PgUp)\tSearch previous occurrence."),getApp()->backwardicon,this,ID_ISEARCH_PREV,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXButton(searchbar,tr("\tSearch Next (PgDn)\tSearch next occurrence."),getApp()->forwardicon,this,ID_ISEARCH_NEXT,ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
  new FXFrame(searchbar,FRAME_NONE|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,4,4);

  new FXToggleButton(searchbar,tr("\tLiteral String\tLiteral string search."),tr("\tRegular Expression\tRegular expression search."),getApp()->searchnorexicon,getApp()->searchrexicon,this,ID_ISEARCH_REGEX,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXToggleButton(searchbar,tr("\tPartial Word\tSearch for partial words."),tr("\tWhole Word\tSearch for whole words."),getApp()->searchnowordicon,getApp()->searchwordicon,this,ID_ISEARCH_WORDS,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXToggleButton(searchbar,tr("\tCase Sensitive\tCase sensitive search."),tr("\tCase Insensitive\tCase insensitive search."),getApp()->searchnocaseicon,getApp()->searchcaseicon,this,ID_ISEARCH_IGNCASE,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXToggleButton(searchbar,tr("\tSearch Forward\tSearch Backward through the text."),tr("\tSearch Up\tSearch upward through the text."),getApp()->searchdnicon,getApp()->searchupicon,this,ID_ISEARCH_REVERSE,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  }


// Create status bar
void TextWindow::createStatusbar(){

  // Info about the editor
  new FXButton(statusbar,tr("\tAbout Adie\tAbout the Adie text editor."),getApp()->smallicon,this,ID_ABOUT,LAYOUT_FILL_Y|LAYOUT_RIGHT);

  // Show clock on status bar
  clock=new FXLabel(statusbar,"88:88:88",nullptr,FRAME_SUNKEN|JUSTIFY_RIGHT|LAYOUT_RIGHT|LAYOUT_CENTER_Y|TEXTFIELD_READONLY,0,0,0,0,2,2,1,1);

  // Undo/redo block
  undoredoblock=new FXHorizontalFrame(statusbar,LAYOUT_RIGHT|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0);
  new FXLabel(undoredoblock,tr("  Undo:"),nullptr,LAYOUT_CENTER_Y);
  FXTextField* undocount=new FXTextField(undoredoblock,5,&undolist,FXUndoList::ID_UNDO_COUNT,TEXTFIELD_READONLY|FRAME_SUNKEN|JUSTIFY_RIGHT|LAYOUT_CENTER_Y,0,0,0,0,2,2,1,1);
  undocount->setBackColor(statusbar->getBackColor());
  new FXLabel(undoredoblock,tr("  Redo:"),nullptr,LAYOUT_CENTER_Y);
  FXTextField* redocount=new FXTextField(undoredoblock,5,&undolist,FXUndoList::ID_REDO_COUNT,TEXTFIELD_READONLY|FRAME_SUNKEN|JUSTIFY_RIGHT|LAYOUT_CENTER_Y,0,0,0,0,2,2,1,1);
  redocount->setBackColor(statusbar->getBackColor());

  // Show readonly state in status bar
  FXLabel* readonly=new FXLabel(statusbar,FXString::null,nullptr,FRAME_SUNKEN|JUSTIFY_RIGHT|LAYOUT_RIGHT|LAYOUT_CENTER_Y,0,0,0,0,2,2,1,1);
  readonly->setTarget(this);
  readonly->setSelector(ID_READONLY);
  readonly->setTipText(tr("Editable"));

  // Show insert mode in status bar
  FXLabel* overstrike=new FXLabel(statusbar,FXString::null,nullptr,FRAME_SUNKEN|LAYOUT_RIGHT|LAYOUT_CENTER_Y,0,0,0,0,2,2,1,1);
  overstrike->setTarget(this);
  overstrike->setSelector(ID_OVERSTRIKE);
  overstrike->setTipText(tr("Overstrike mode"));

  // Show tab mode in status bar
  FXLabel* tabmode=new FXLabel(statusbar,FXString::null,nullptr,FRAME_SUNKEN|LAYOUT_RIGHT|LAYOUT_CENTER_Y,0,0,0,0,2,2,1,1);
  tabmode->setTarget(this);
  tabmode->setSelector(ID_TABMODE);
  tabmode->setTipText(tr("Tab mode"));

  // Show tab columns
  FXTextField* numchars=new FXTextField(statusbar,2,this,ID_TABCOLUMNS,FRAME_SUNKEN|JUSTIFY_RIGHT|LAYOUT_RIGHT|LAYOUT_CENTER_Y,0,0,0,0,2,2,1,1);
  numchars->setBackColor(statusbar->getBackColor());
  numchars->setTipText(tr("Tab columns"));

  // Caption before tab columns
  new FXLabel(statusbar,tr("  Tab:"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y);

  // Show column number in status bar
  FXTextField* columnno=new FXTextField(statusbar,5,editor,FXText::ID_CURSOR_COLUMN,FRAME_SUNKEN|JUSTIFY_RIGHT|LAYOUT_RIGHT|LAYOUT_CENTER_Y,0,0,0,0,2,2,1,1);
  columnno->setBackColor(statusbar->getBackColor());
  columnno->setTipText(tr("Current column"));

  // Caption before number
  new FXLabel(statusbar,tr("  Col:"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y);

  // Show line number in status bar
  FXTextField* rowno=new FXTextField(statusbar,5,editor,FXText::ID_CURSOR_ROW,FRAME_SUNKEN|JUSTIFY_RIGHT|LAYOUT_RIGHT|LAYOUT_CENTER_Y,0,0,0,0,2,2,1,1);
  rowno->setBackColor(statusbar->getBackColor());
  rowno->setTipText(tr("Current line"));

  // Caption before number
  new FXLabel(statusbar,tr("  Line:"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  }


// Create and show window
void TextWindow::create(){
  readRegistry();
  FXMainWindow::create();
  dragshell1->create();
  dragshell2->create();
  dragshell3->create();
  filemenu->create();
  editmenu->create();
  gotomenu->create();
  searchmenu->create();
  optionmenu->create();
  viewmenu->create();
  windowmenu->create();
  helpmenu->create();
  if(!urilistType){urilistType=getApp()->registerDragType(urilistTypeName);}
  getApp()->addTimeout(this,ID_CLOCKTIME,CLOCKTIMER);
  editor->setFocus();
  }

/*******************************************************************************/

// Is it modified
FXbool TextWindow::isModified() const {
  return !undolist.marked();
  }


// Set current file of directory browser
void TextWindow::setBrowserCurrentFile(const FXString& file){
  dirlist->setCurrentFile(file);
  }


// Get current file of directory browser
FXString TextWindow::getBrowserCurrentFile() const {
  return dirlist->getCurrentFile();
  }


// Set current directory
void TextWindow::setBrowserCurrentDirectory(const FXString& dir){
  dirlist->setDirectory(dir);
  }


// Get current directory
FXString TextWindow::getBrowserCurrentDirectory() const {
  return dirlist->getDirectory();
  }


// Set editable flag
void TextWindow::setEditable(FXbool edit){
  editor->setEditable(edit);
  }


// Is it editable
FXbool TextWindow::isEditable() const {
  return editor->isEditable();
  }


// Set current pattern
void TextWindow::setCurrentPattern(FXint n){
  filter->setCurrentItem(FXCLAMP(0,n,filter->getNumItems()-1),true);
  }


// Return current pattern
FXint TextWindow::getCurrentPattern() const {
  return filter->getCurrentItem();
  }


// Change patterns, each pattern separated by newline
void TextWindow::setPatternList(const FXString& patterns){
  FXint current=getCurrentPattern();
  filter->clearItems();
  filter->fillItems(patterns);
  if(!filter->getNumItems()) filter->appendItem("All Files (*)");
  setCurrentPattern(FXCLAMP(0,current,filter->getNumItems()-1));
  }


// Return list of patterns
FXString TextWindow::getPatternList() const {
  FXString pat;
  for(FXint i=0; i<filter->getNumItems(); ++i){
    if(!pat.empty()) pat+='\n';
    pat+=filter->getItemText(i);
    }
  return pat;
  }


// Change search paths
void TextWindow::setSearchPaths(const FXString& paths){
  searchpaths=paths;
  }


// Get seatch paths
FXString TextWindow::getSearchPaths() const {
  return searchpaths;
  }


// Search for file using search paths
// The search paths may contain relative directories, which
// are interpreted relative to the directory containing the
// current file.
FXString TextWindow::searchForFile(const FXString& file){
  if(!file.empty()){
    FXString base(FXPath::directory(getFilename()));
    FXString path(FXPath::absolute(base,file));
    if(FXStat::exists(path)) return path;
    for(FXint beg=0,end=0; searchpaths[beg]; beg=end){
      while(searchpaths[beg]==PATHLISTSEP) beg++;
      end=beg;
      while(searchpaths[end] && searchpaths[end]!=PATHLISTSEP) end++;
      if(beg==end) break;
      path=FXPath::absolute(FXPath::absolute(base,FXPath::expand(searchpaths.mid(beg,end-beg))),file);
      if(FXStat::exists(path)) return path;
      }
    }
  return FXString::null;
  }


// Set status message
void TextWindow::setStatusMessage(const FXString& msg){
  statusbar->getStatusLine()->setNormalText(msg);
  }


// Visit given line
void TextWindow::visitPosition(FXint pos){
  if(!editor->isPosVisible(pos)){ editor->setCenterLine(pos); }
  editor->setCursorPos(pos);
  editor->makePositionVisible(editor->getCursorPos());
  }


// Visit position
void TextWindow::visitLine(FXint line,FXint column){
  editor->setCursorRowColumn(line-1,column);
  editor->setCenterLine(editor->getCursorPos());
  editor->makePositionVisible(editor->getCursorPos());
  }


/*******************************************************************************/

// Load file into buffer
FXbool TextWindow::loadBuffer(const FXString& file,FXString& buffer,FXuint bits){
  FXFile textfile(file,FXFile::Reading);
  if(textfile.isOpen()){
    FXlong size=textfile.size();
    if(size<=MAXFILESIZE){
      if(buffer.length(size)){
        if(textfile.readBlock(buffer.text(),buffer.length())==buffer.length()){
          if(bits&TRIM){
            FXint i=0,j=0,k=0,c;
            while(j<buffer.length()){
              buffer[i]=c=buffer[j];
              if(c=='\n'){
                while(k<i && Ascii::isSpace(buffer[i-1])){
                  i--;
                  }
                buffer[i]='\n';
                k=i+1;
                }
              i++;
              j++;
              }
            buffer.trunc(i);
            }
          if(bits&LINE){
            if(buffer.tail()!='\n'){
              buffer.append('\n');
              }
            }
          if(bits&CRLF){
            dosToUnix(buffer);
            }
          return true;
          }
        }
      }
    }
  return false;
  }


// Save file from buffer
FXbool TextWindow::saveBuffer(const FXString& file,FXString& buffer,FXuint bits){
  FXFile textfile(file,FXFile::Writing);
  if(textfile.isOpen()){
    if(bits&TRIM){
      FXint i=0,j=0,k=0,c;
      while(j<buffer.length()){
        buffer[i]=c=buffer[j];
        if(c=='\n'){
          while(k<i && Ascii::isSpace(buffer[i-1])){
            i--;
            }
          buffer[i]='\n';
          k=i+1;
          }
        i++;
        j++;
        }
      buffer.trunc(i);
      }
    if(bits&LINE){
      if(buffer.tail()!='\n'){
        buffer.append('\n');
        }
      }
    if(bits&CRLF){
      unixToDos(buffer);
      }
    if(textfile.writeBlock(buffer.text(),buffer.length())==buffer.length()){
      return true;
      }
    }
  return false;
  }

/*******************************************************************************/

// Load file
FXbool TextWindow::loadFile(const FXString& file){
  FXString buffer;
  FXbool loaded=false;
  FXuint bits=0;

  // Set wait cursor
  getApp()->beginWaitCursor();

  // Load flags
  if(stripsp) bits|=TRIM;
  if(stripcr) bits|=CRLF;

  // Try load buffer
  if(TextWindow::loadBuffer(file,buffer,bits)){

    // Set text
    editor->setText(buffer);

    // Set filename
    setFilename(file);
    setFilenameSet(true);
    setFiletime(FXStat::modified(file));

    // Can we read/write it?
    setEditable(FXStat::isAccessible(file));

    // Recent files
    mrufiles.appendFile(file);

    // Clear undo records
    undolist.clear();

    // Mark undo state as clean (saved)
    undolist.mark();

    // Clear insert point
    insertpoint=0;

    // Success
    loaded=true;
    }

  // Kill wait cursor
  getApp()->endWaitCursor();
  return loaded;
  }


// Save file
FXbool TextWindow::saveFile(const FXString& file){
  FXString buffer;
  FXbool saved=false;
  FXuint bits=0;

  // Set wait cursor
  getApp()->beginWaitCursor();

  // Load flags
  if(stripsp) bits|=TRIM;
  if(appendcr) bits|=CRLF;
  if(appendnl) bits|=LINE;

  // Get text from editor
  editor->getText(buffer);

  // Try save buffer
  if(TextWindow::saveBuffer(file,buffer,bits)){

    // Set filename
    setFilename(file);
    setFilenameSet(true);
    setFiletime(FXStat::modified(file));

    // We just wrote it...
    setEditable(true);

    // Recent files
    mrufiles.appendFile(file);

    // Mark undo state as clean (saved)
    undolist.mark();

    // Success
    saved=true;
    }

  // Kill wait cursor
  getApp()->endWaitCursor();
  return saved;
  }


// Save to file; don't switch to it as current document
FXbool TextWindow::saveToFile(const FXString& file){
  FXString buffer;
  FXbool saved=false;
  FXuint bits=0;

  // Set wait cursor
  getApp()->beginWaitCursor();

  // Load flags
  if(stripsp) bits|=TRIM;
  if(appendcr) bits|=CRLF;
  if(appendnl) bits|=LINE;

  // Get text from editor
  editor->getText(buffer);

  // Try save buffer
  if(TextWindow::saveBuffer(file,buffer,bits)){
    saved=true;
    }

  // Kill wait cursor
  getApp()->endWaitCursor();
  return saved;
  }

/*******************************************************************************/

// Insert from file
FXbool TextWindow::insertFromFile(const FXString& file){
  FXbool loaded=false;
  FXString buffer;
  FXuint bits=0;

  // Set wait cursor
  getApp()->beginWaitCursor();

  // Load flags
  if(stripsp) bits|=TRIM;
  if(stripcr) bits|=CRLF;

  // Try load buffer
  if(TextWindow::loadBuffer(file,buffer,bits)){

    // Replace the selection, or insert at cursor
    if(!editor->replaceSelection(buffer,true)){
      editor->insertText(editor->getCursorPos(),buffer,true);
      }

    // Success
    loaded=true;
    }

  // Kill wait cursor
  getApp()->endWaitCursor();
  return loaded;
  }


// Extract file
FXbool TextWindow::extractToFile(const FXString& file){
  FXbool saved=false;
  FXString buffer;
  FXuint bits=0;

  // Set wait cursor
  getApp()->beginWaitCursor();

  // Load flags
  if(stripsp) bits|=TRIM;
  if(appendcr) bits|=CRLF;
  if(appendnl) bits|=LINE;

  // Grab selection
  buffer=editor->getSelectedText();

  // Don't bother if no text
  if(buffer.length()){

    // Try save buffer
    if(TextWindow::saveBuffer(file,buffer,bits)){
      saved=true;
      }
    }

  // Kill wait cursor
  getApp()->endWaitCursor();
  return saved;
  }

/*******************************************************************************/

// Read settings from registry
void TextWindow::readRegistry(){
  FXColor textback,textfore,textselback,textselfore,textcursor,texthilitefore,texthiliteback,textactiveback,textbar,textnumber;
  FXColor dirback,dirfore,dirselback,dirselfore,dirlines;
  FXbool hiddenfiles,showactive,showmatch,hidetree;
  FXbool hideclock,hidestatus,hideundo,hidetoolbar;
  FXint ww,hh,xx,yy,modebits,treewidth,loggerheight,wheellines;
  FXTime matchtime;

  // Font
  FXString fontspec=getApp()->reg().readStringEntry("SETTINGS","textfont","");

  // Set font
  if(!fontspec.empty()){
    font=new FXFont(getApp(),fontspec);
    editor->setFont(font);
    logger->setFont(font);
    }

  // Text colors
  textback=getApp()->reg().readColorEntry("SETTINGS","textbackground",editor->getBackColor());
  textfore=getApp()->reg().readColorEntry("SETTINGS","textforeground",editor->getTextColor());
  textselback=getApp()->reg().readColorEntry("SETTINGS","textselbackground",editor->getSelBackColor());
  textselfore=getApp()->reg().readColorEntry("SETTINGS","textselforeground",editor->getSelTextColor());
  textcursor=getApp()->reg().readColorEntry("SETTINGS","textcursor",editor->getCursorColor());
  texthilitefore=getApp()->reg().readColorEntry("SETTINGS","texthiliteforeground",editor->getHiliteTextColor());
  texthiliteback=getApp()->reg().readColorEntry("SETTINGS","texthilitebackground",editor->getHiliteBackColor());
  textactiveback=getApp()->reg().readColorEntry("SETTINGS","textactivebackground",editor->getActiveBackColor());
  textbar=getApp()->reg().readColorEntry("SETTINGS","textnumberbackground",editor->getBarColor());
  textnumber=getApp()->reg().readColorEntry("SETTINGS","textnumberforeground",editor->getNumberColor());

  // Directory colors
  dirback=getApp()->reg().readColorEntry("SETTINGS","browserbackground",dirlist->getBackColor());
  dirfore=getApp()->reg().readColorEntry("SETTINGS","browserforeground",dirlist->getTextColor());
  dirselback=getApp()->reg().readColorEntry("SETTINGS","browserselbackground",dirlist->getSelBackColor());
  dirselfore=getApp()->reg().readColorEntry("SETTINGS","browserselforeground",dirlist->getSelTextColor());
  dirlines=getApp()->reg().readColorEntry("SETTINGS","browserlines",dirlist->getLineColor());

  // Delimiters
  delimiters=getApp()->reg().readStringEntry("SETTINGS","delimiters","~.,/\\`'!@#$%^&*()-=+{}|[]\":;<>?");

  // Read mouse wheel lines
  wheellines=getApp()->reg().readIntEntry("SETTINGS","wheellines",10);

  // Get size
  xx=getApp()->reg().readIntEntry("SETTINGS","x",5);
  yy=getApp()->reg().readIntEntry("SETTINGS","y",5);
  ww=getApp()->reg().readIntEntry("SETTINGS","width",600);
  hh=getApp()->reg().readIntEntry("SETTINGS","height",400);

  // Initial rows and columns
  initialwidth=getApp()->reg().readIntEntry("SETTINGS","initialwidth",640);
  initialheight=getApp()->reg().readIntEntry("SETTINGS","initialheight",480);
  initialsize=getApp()->reg().readBoolEntry("SETTINGS","initialsize",false);

  // Use this instead of size from last time
  if(initialsize){
    ww=initialwidth;
    hh=initialheight;
    }

  // Hidden files shown
  hiddenfiles=getApp()->reg().readBoolEntry("SETTINGS","showhiddenfiles",false);
  dirlist->showHiddenFiles(hiddenfiles);

  // Showing undo counters?
  hideundo=getApp()->reg().readBoolEntry("SETTINGS","hideundo",true);

  // Showing the tree?
  hidetree=getApp()->reg().readBoolEntry("SETTINGS","hidetree",true);
  treewidth=getApp()->reg().readIntEntry("SETTINGS","treewidth",100);
  if(!hidetree) ww+=treewidth;
  treebox->setWidth(treewidth);

  // Showing the clock?
  hideclock=getApp()->reg().readBoolEntry("SETTINGS","hideclock",false);

  // Showing the status line?
  hidestatus=getApp()->reg().readBoolEntry("SETTINGS","hidestatus",false);

  // Showing the tool bar?
  hidetoolbar=getApp()->reg().readBoolEntry("SETTINGS","hidetoolbar",false);

  // Showing the search bar?
  showsearchbar=getApp()->reg().readBoolEntry("SETTINGS","showsearchbar",false);

  // Showing the errors display?
  showlogger=getApp()->reg().readBoolEntry("SETTINGS","showlogger",false);
  loggerheight=getApp()->reg().readIntEntry("SETTINGS","loggerheight",32);
  loggerframe->setHeight(loggerheight);

  // Highlight match time
  matchtime=getApp()->reg().readLongEntry("SETTINGS","bracematchpause",2000000000);

  // Brace matching
  showmatch=getApp()->reg().readBoolEntry("SETTINGS","bracematch",true);

  // Active Background
  showactive=getApp()->reg().readBoolEntry("SETTINGS","showactive",false);

  // Modeline support
  modeline=getApp()->reg().readBoolEntry("SETTINGS","modeline",false);

  // Autoindent
  autoindent=getApp()->reg().readBoolEntry("SETTINGS","autoindent",false);

  // Word wrapping
  wrapcols=getApp()->reg().readIntEntry("SETTINGS","wrapcols",80);
  wrapping=getApp()->reg().readBoolEntry("SETTINGS","wordwrap",false);
  fixedwrap=getApp()->reg().readBoolEntry("SETTINGS","fixedwrap",true);

  // Tab settings
  tabcols=getApp()->reg().readIntEntry("SETTINGS","tabcols",8);
  hardtabs=getApp()->reg().readBoolEntry("SETTINGS","hardtabs",true);

  // Space for line numbers
  barcols=getApp()->reg().readIntEntry("SETTINGS","barcols",0);

  // Various flags
  stripcr=getApp()->reg().readBoolEntry("SETTINGS","stripreturn",true);
  appendcr=getApp()->reg().readBoolEntry("SETTINGS","appendreturn",false);
  stripsp=getApp()->reg().readBoolEntry("SETTINGS","stripspaces",false);
  appendnl=getApp()->reg().readBoolEntry("SETTINGS","appendnewline",true);
  saveviews=getApp()->reg().readBoolEntry("SETTINGS","saveviews",false);
  savemarks=getApp()->reg().readBoolEntry("SETTINGS","savebookmarks",false);
  warnchanged=getApp()->reg().readBoolEntry("SETTINGS","warnchanged",true);
  colorize=getApp()->reg().readBoolEntry("SETTINGS","colorize",false);
  searchflags=getApp()->reg().readUIntEntry("SETTINGS","searchflags",SEARCH_FORWARD);

  // File patterns
  setPatternList(getApp()->reg().readStringEntry("SETTINGS","filepatterns","All Files (*)"));
  setCurrentPattern(getApp()->reg().readIntEntry("SETTINGS","filepatternno",0));

  // Search path
  searchpaths=getApp()->reg().readStringEntry("SETTINGS","searchpaths","/usr/include");

  // Change the colors
  editor->setTextColor(textfore);
  editor->setBackColor(textback);
  editor->setSelBackColor(textselback);
  editor->setSelTextColor(textselfore);
  editor->setCursorColor(textcursor);
  editor->setHiliteBackColor(texthiliteback);
  editor->setHiliteTextColor(texthilitefore);
  editor->setActiveBackColor(textactiveback);
  editor->setBarColor(textbar);
  editor->setNumberColor(textnumber);

  dirlist->setTextColor(dirfore);
  dirlist->setBackColor(dirback);
  dirlist->setSelBackColor(dirselback);
  dirlist->setSelTextColor(dirselfore);
  dirlist->setLineColor(dirlines);

  // Change delimiters
  editor->setDelimiters(delimiters.text());

  // Set vertical wheel lines
  editor->verticalScrollBar()->setWheelLines(wheellines);

  // Set horizontal wheel lines to number of tab columns
  editor->horizontalScrollBar()->setWheelLines(tabcols);

  // Highlight match time
  editor->setHiliteMatchTime(matchtime);

  // Hide tree if asked for
  if(hidetree) treebox->hide();

  // Hide clock
  if(hideclock) clock->hide();

  // Hide statusline
  if(hidestatus) statusbar->hide();

  // Hide toolbar
  if(hidetoolbar) toolbar->hide();

  // Hide search bar
  if(!showsearchbar) searchbar->hide();

  // Hide errors
  if(!showlogger) loggerframe->hide();

  // Hide undo counters
  if(hideundo) undoredoblock->hide();

  // Set editor modes
  modebits=0;
  if(!hardtabs) modebits|=TEXT_NO_TABS;
  if(wrapping) modebits|=TEXT_WORDWRAP;
  if(showactive) modebits|=TEXT_SHOWACTIVE;
  if(fixedwrap) modebits|=TEXT_FIXEDWRAP;
  if(autoindent) modebits|=TEXT_AUTOINDENT;
  if(showmatch) modebits|=TEXT_SHOWMATCH;
  editor->setTextStyle(modebits);

  // Wrap and tab columns
  editor->setWrapColumns(wrapcols);
  editor->setTabColumns(tabcols);
  editor->setBarColumns(barcols);

  // Search history
  loadSearchHistory();

  // Reposition window
  position(xx,yy,ww,hh);
  }

/*******************************************************************************/

// Save settings to registry
void TextWindow::writeRegistry(){

  // Font
  FXString fontspec=editor->getFont()->getFont();

  // Font
  getApp()->reg().writeStringEntry("SETTINGS","textfont",fontspec.text());

  // Colors of text
  getApp()->reg().writeColorEntry("SETTINGS","textbackground",editor->getBackColor());
  getApp()->reg().writeColorEntry("SETTINGS","textforeground",editor->getTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","textselbackground",editor->getSelBackColor());
  getApp()->reg().writeColorEntry("SETTINGS","textselforeground",editor->getSelTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","textcursor",editor->getCursorColor());
  getApp()->reg().writeColorEntry("SETTINGS","texthilitebackground",editor->getHiliteBackColor());
  getApp()->reg().writeColorEntry("SETTINGS","texthiliteforeground",editor->getHiliteTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","textactivebackground",editor->getActiveBackColor());
  getApp()->reg().writeColorEntry("SETTINGS","textnumberbackground",editor->getBarColor());
  getApp()->reg().writeColorEntry("SETTINGS","textnumberforeground",editor->getNumberColor());

  // Colors of directory
  getApp()->reg().writeColorEntry("SETTINGS","browserbackground",dirlist->getBackColor());
  getApp()->reg().writeColorEntry("SETTINGS","browserforeground",dirlist->getTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","browserselbackground",dirlist->getSelBackColor());
  getApp()->reg().writeColorEntry("SETTINGS","browserselforeground",dirlist->getSelTextColor());
  getApp()->reg().writeColorEntry("SETTINGS","browserlines",dirlist->getLineColor());

  // Delimiters
  getApp()->reg().writeStringEntry("SETTINGS","delimiters",delimiters.text());

  // Wheel lines
  getApp()->reg().writeIntEntry("SETTINGS","wheellines",editor->verticalScrollBar()->getWheelLines());

  // Write new window size back to registry
  getApp()->reg().writeIntEntry("SETTINGS","x",getX());
  getApp()->reg().writeIntEntry("SETTINGS","y",getY());
  getApp()->reg().writeIntEntry("SETTINGS","width",treebox->shown() ? getWidth()-treebox->getWidth() : getWidth());
  getApp()->reg().writeIntEntry("SETTINGS","height",getHeight());

  // Initial size setting
  getApp()->reg().writeIntEntry("SETTINGS","initialwidth",initialwidth);
  getApp()->reg().writeIntEntry("SETTINGS","initialheight",initialheight);
  getApp()->reg().writeBoolEntry("SETTINGS","initialsize",initialsize);

  // Were showing hidden files
  getApp()->reg().writeBoolEntry("SETTINGS","showhiddenfiles",dirlist->showHiddenFiles());

  // Was tree shown
  getApp()->reg().writeBoolEntry("SETTINGS","hidetree",!treebox->shown());
  getApp()->reg().writeIntEntry("SETTINGS","treewidth",treebox->getWidth());

  // Was status line shown
  getApp()->reg().writeBoolEntry("SETTINGS","hidestatus",!statusbar->shown());

  // Was clock shown
  getApp()->reg().writeBoolEntry("SETTINGS","hideclock",!clock->shown());

  // Was toolbar shown
  getApp()->reg().writeBoolEntry("SETTINGS","hidetoolbar",!toolbar->shown());

  // Was search bar shown
  getApp()->reg().writeBoolEntry("SETTINGS","showsearchbar",searchbar->shown());

  // Was logger display shown
  getApp()->reg().writeBoolEntry("SETTINGS","showlogger",loggerframe->shown());
  getApp()->reg().writeIntEntry("SETTINGS","loggerheight",loggerframe->getHeight());

  // Were undo counters shown
  getApp()->reg().writeBoolEntry("SETTINGS","hideundo",!undoredoblock->shown());

  // Highlight match time
  getApp()->reg().writeLongEntry("SETTINGS","bracematchpause",editor->getHiliteMatchTime());
  getApp()->reg().writeBoolEntry("SETTINGS","bracematch",(editor->getTextStyle()&TEXT_SHOWMATCH)!=0);

  // Active background
  getApp()->reg().writeBoolEntry("SETTINGS","showactive",(editor->getTextStyle()&TEXT_SHOWACTIVE)!=0);

  // Modeline support
  getApp()->reg().writeBoolEntry("SETTINGS","modeline",modeline);

  // Autoindent mode
  getApp()->reg().writeBoolEntry("SETTINGS","autoindent",autoindent);

  // Wrap mode
  getApp()->reg().writeIntEntry("SETTINGS","wrapcols",wrapcols);
  getApp()->reg().writeBoolEntry("SETTINGS","wordwrap",wrapping);
  getApp()->reg().writeBoolEntry("SETTINGS","fixedwrap",fixedwrap);

  // Tab settings
  getApp()->reg().writeIntEntry("SETTINGS","tabcols",tabcols);
  getApp()->reg().writeBoolEntry("SETTINGS","hardtabs",hardtabs);

  // Bar columns
  getApp()->reg().writeIntEntry("SETTINGS","barcols",editor->getBarColumns());

  // Strip returns
  getApp()->reg().writeBoolEntry("SETTINGS","stripreturn",stripcr);
  getApp()->reg().writeBoolEntry("SETTINGS","appendreturn",appendcr);
  getApp()->reg().writeBoolEntry("SETTINGS","stripspaces",stripsp);
  getApp()->reg().writeBoolEntry("SETTINGS","appendnewline",appendnl);
  getApp()->reg().writeBoolEntry("SETTINGS","saveviews",saveviews);
  getApp()->reg().writeBoolEntry("SETTINGS","savebookmarks",savemarks);
  getApp()->reg().writeBoolEntry("SETTINGS","warnchanged",warnchanged);
  getApp()->reg().writeBoolEntry("SETTINGS","colorize",colorize);
  getApp()->reg().writeUIntEntry("SETTINGS","searchflags",searchflags);

  // File patterns
  getApp()->reg().writeIntEntry("SETTINGS","filepatternno",getCurrentPattern());
  getApp()->reg().writeStringEntry("SETTINGS","filepatterns",getPatternList().text());

  // Search path
  getApp()->reg().writeStringEntry("SETTINGS","searchpaths",searchpaths.text());

  // Remove bookmarks and views
  if(!savemarks){ getApp()->reg().deleteSection("BOOKMARKS"); }
  if(!saveviews){ getApp()->reg().deleteSection("VIEW"); }

  // Search history
  saveSearchHistory();
  }

/*******************************************************************************/

// Close window
FXbool TextWindow::close(FXbool notify){

  // De-iconify so we can look at it
  if(isMinimized()){
    restore(notify);
    }

  // Prompt the user to save the edit buffer prior
  // to closing the window.  One more chance...
  if(!saveChanges()) return false;

  // Save settings
  writeRegistry();

  // Perform normal close stuff
  return FXMainWindow::close(notify);
  }


// Detach window
void TextWindow::detach(){
  FXMainWindow::detach();
  dragshell1->detach();
  dragshell2->detach();
  urilistType=0;
  }


// Clean up the mess
TextWindow::~TextWindow(){
  getApp()->removeWindow(this);
  getApp()->removeTimeout(this,ID_CLOCKTIME);
  delete shellCommand;
  delete font;
  delete dragshell1;
  delete dragshell2;
  delete dragshell3;
  delete filemenu;
  delete editmenu;
  delete gotomenu;
  delete searchmenu;
  delete shellmenu;
  delete optionmenu;
  delete viewmenu;
  delete windowmenu;
  delete helpmenu;
  delete syntaxmenu;
  delete tabsmenu;
  }

/*******************************************************************************/

// About box
long TextWindow::onCmdAbout(FXObject*,FXSelector,void*){
  FXDialogBox about(this,tr("About Adie"),DECOR_TITLE|DECOR_BORDER,0,0,0,0, 0,0,0,0, 0,0);
  FXGIFIcon picture(getApp(),adie_gif);
  new FXLabel(&about,FXString::null,&picture,FRAME_GROOVE|LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y|JUSTIFY_CENTER_X|JUSTIFY_CENTER_Y,0,0,0,0, 0,0,0,0);
  FXVerticalFrame* side=new FXVerticalFrame(&about,LAYOUT_SIDE_RIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 10,10,10,10, 0,0);
  new FXLabel(side,"A . d . i . e",nullptr,JUSTIFY_LEFT|ICON_BEFORE_TEXT|LAYOUT_FILL_X);
  new FXHorizontalSeparator(side,SEPARATOR_LINE|LAYOUT_FILL_X);
  new FXLabel(side,FXString::value(tr("\nThe Adie ADvanced Interactive Editor, version %d.%d.%d (%s).\n\nAdie is a fast and convenient programming text editor and file\nviewer with an integrated directory browser.\nUsing The FOX Toolkit (www.fox-toolkit.org), version %d.%d.%d.\nCopyright (C) 2000,2023 Jeroen van der Zijp (jeroen@fox-toolkit.net).\n "),VERSION_MAJOR,VERSION_MINOR,VERSION_PATCH,__DATE__,FOX_MAJOR,FOX_MINOR,FOX_LEVEL),nullptr,JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXButton *button=new FXButton(side,tr("&OK"),nullptr,&about,FXDialogBox::ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT,0,0,0,0,32,32,2,2);
  button->setFocus();
  about.execute(PLACEMENT_OWNER);
  return 1;
  }


// Show help window, create it on-the-fly
long TextWindow::onCmdHelp(FXObject*,FXSelector,void*){
  HelpWindow *helpwindow=new HelpWindow(getApp());
  helpwindow->create();
  helpwindow->show(PLACEMENT_CURSOR);
  return 1;
  }


// Show preferences dialog
long TextWindow::onCmdPreferences(FXObject*,FXSelector,void*){
  Preferences preferences(this);
  preferences.setPatternList(getPatternList());
  preferences.setSyntax(getSyntax());
  if(preferences.execute(PLACEMENT_OWNER)){
    setPatternList(preferences.getPatternList());
    }
  return 1;
  }


// Change font
long TextWindow::onCmdFont(FXObject*,FXSelector,void*){
  FXFontDialog fontdlg(this,tr("Change Font"),DECOR_BORDER|DECOR_TITLE);
  FXFontDesc fontdesc=editor->getFont()->getFontDesc();
  fontdlg.setFontDesc(fontdesc);
  if(fontdlg.execute(PLACEMENT_OWNER)){
    FXFont *oldfont=font;
    fontdesc=fontdlg.getFontDesc();
    font=new FXFont(getApp(),fontdesc);
    font->create();
    editor->setFont(font);
    logger->setFont(font);
    delete oldfont;
    }
  return 1;
  }


// Sensitize if editable
long TextWindow::onUpdIsEditable(FXObject* sender,FXSelector,void*){
  sender->handle(this,editor->isEditable()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Sensitize if selection
long TextWindow::onUpdHasSelection(FXObject* sender,FXSelector,void*){
  sender->handle(this,editor->hasSelection()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }

/*******************************************************************************/

// Check for external changes
// Check to see if the disk file was changed or removed. If it was changed,
// we may not want to lose the changes on the disk, so optionally reload them.
// If it was removed, the text buffer may contain the only copy of the file
// and we will want prompt the user to save it before it is lost.
FXbool TextWindow::checkChanges(){
  if(isFilenameSet() && 1<getFiletime()){
    FXStat info;

    // Check file on disk
    if(FXStat::statFile(getFilename(),info)){

      // Not changed, all is OK.
      if(getFiletime()==info.modified()) return true;

      // Don't ask again unless it changes again
      setFiletime(info.modified());

      // File was changed; we can cancel, reload file from disk,
      // or proceed as is
      if(FXMessageBox::warning(this,MBOX_YES_NO,tr("File Was Changed On Disk"),tr("Reload file: %s back from disk?"),getFilename().text())==MBOX_CLICKED_YES){
        if(!reloadDoc()) return false;
        }
      }

    // File is M.I.A.
    else{

      // Set the file time to 1, preventing repeated warnings.
      // When calling saveChanges, we will get one more chance
      // to save it, however.
      setFiletime(1);

      // File was deleted; we can cancel, save the buffer back to disk,
      if(FXMessageBox::warning(this,MBOX_YES_NO,tr("File Was Removed From Disk"),tr("Save file: %s back to disk?"),getFilename().text())==MBOX_CLICKED_YES){
        if(!saveDoc()) return false;
        }
      }
    }
  return true;
  }


// Check if file on disk changed by another program;
// If the file is present but has different filetime than the
// one of the buffer, it may have been changed by another program.
// Otherwise, if the filetime was unchanged, then the text buffer
// is baselined from this file.
// If the file was removed from the disk, and the filetime
// was not zero, the file in the buffer may be the last surviving
// copy of the data, and you may therefore want to save it.
FXint TextWindow::changedExternally(){
  if(isFilenameSet()){
    FXStat info;
    if(FXStat::statFile(getFilename(),info)){
      if(getFiletime()!=info.modified()) return 1;
      }
    else{
      if(getFiletime()!=0) return 2;
      }
    }
  return 0;
  }


// Before switching to another file or closing the window, check to see
// if the file was edited; also, check to see if the file on disk was
// removed (or changed).
// Either of these cases should result in a prompt to save the file.
// If we decide to save, and the filename was set, proceed to save to that
// filename.  Otherwise, prompt the user for the path and filename to save
// the file to.
// If the file was unchanged, and the filename was set, then save the book-
// marks and view, as they could have changed.
// Don't save the bookmarks if the buffer was changed but we're not saving
// it:- we don't want the bookmark locations to go out of sync.
FXbool TextWindow::saveChanges(){
  if(isModified() || changedExternally()){
    FXuint answer=FXMessageBox::warning(this,MBOX_YES_NO_CANCEL,tr("File Has Unsaved Changes"),tr("Save file: %s to disk?"),getFilename().text());
    if(answer==MBOX_CLICKED_CANCEL) return false;
    if(answer==MBOX_CLICKED_YES){
      if(isFilenameSet()){
        return saveDoc();
        }
      return saveDocAs();
      }
    return true;
    }
  if(isFilenameSet()){
    writeBookmarks(getFilename());
    writeView(getFilename());
    }
  return true;
  }


// Create new document
FXbool TextWindow::newDoc(){
  FXString folder=FXPath::directory(getFilename());
  FXString file=getApp()->unique(folder);
  TextWindow *window=new TextWindow(getApp());
  window->create();
  window->setBrowserCurrentDirectory(folder);
  window->setFilename(file);
  window->setFilenameSet(false);
  window->setFiletime(0);
  window->raise();
  window->setFocus();
  window->show(PLACEMENT_DEFAULT);
  return true;
  }


// Create new named document
FXbool TextWindow::createDoc(){
  FXFileDialog newdialog(this,tr("Create File"));
  FXString file=getApp()->unique(FXPath::directory(getFilename()));
  FXint patno=getApp()->reg().readIntEntry("File Dialog","pattern",0);
  newdialog.setSelectMode(SELECTFILE_ANY);
  newdialog.setAssociations(getApp()->getFileAssociations(),false);
  newdialog.setPatternList(getPatternList());
  newdialog.setCurrentPattern(FXCLAMP(0,patno,newdialog.getNumPatterns()-1));
  newdialog.setFilename(file);
  if(newdialog.execute()){
    getApp()->reg().writeIntEntry("File Dialog","pattern",newdialog.getCurrentPattern());
    file=newdialog.getFilename();
    return createDoc(file);
    }
  return false;
  }


// Create new named document
FXbool TextWindow::createDoc(const FXString& file){
  TextWindow *window=getApp()->findWindow(file);
  if(!window){
    window=getApp()->findUnused();
    if(!window){
      window=new TextWindow(getApp());
      window->create();
      }
    window->setBrowserCurrentFile(file);
    window->setFilename(file);
    window->setFilenameSet(true);
    window->setFiletime(0);
    window->determineSyntax();
    }
  window->raise();
  window->setFocus();
  window->show(PLACEMENT_DEFAULT);
  return true;
  }


// Save document
// If file was modified externally, prompt before overwriting
FXbool TextWindow::saveDoc(){
  if(isFilenameSet()){
    if(changedExternally()==1){
      FXuint answer=FXMessageBox::warning(this,MBOX_YES_NO_CANCEL,tr("File Was Changed On Disk"),tr("Overwrite file: %s?"),getFilename().text());
      if(answer==MBOX_CLICKED_CANCEL) return false;
      if(answer==MBOX_CLICKED_NO) return true;
      }
    if(saveFile(getFilename())){
      setBrowserCurrentFile(getFilename());
      writeBookmarks(getFilename());
      writeView(getFilename());
      return true;
      }
    FXMessageBox::error(this,MBOX_OK,tr("Error Saving File"),tr("Unable to save file: %s"),getFilename().text());
    }
  return false;
  }


// Save document under another name
// If file exist, prompt before overwriting
FXbool TextWindow::saveDocAs(){
  FXFileDialog savedialog(this,tr("Save File As"));
  FXString file=getFilename();
  FXint patno=getApp()->reg().readIntEntry("File Dialog","pattern",0);
  savedialog.setSelectMode(SELECTFILE_ANY);
  savedialog.setAssociations(getApp()->getFileAssociations(),false);
  savedialog.setPatternList(getPatternList());
  savedialog.setCurrentPattern(FXCLAMP(0,patno,savedialog.getNumPatterns()-1));
  savedialog.setFilename(file);
  if(savedialog.execute()){
    file=savedialog.getFilename();
    getApp()->reg().writeIntEntry("File Dialog","pattern",savedialog.getCurrentPattern());
    if(FXStat::exists(file)){
      if(FXMessageBox::warning(this,MBOX_YES_NO,tr("File Already Exists"),tr("Overwrite file: %s?"),file.text())==MBOX_CLICKED_NO) return false;
      }
    if(saveFile(file)){
      setBrowserCurrentFile(file);
      writeBookmarks(file);
      writeView(file);
      return true;
      }
    FXMessageBox::error(this,MBOX_OK,tr("Error Saving File"),tr("Unable to save file as: %s"),file.text());
    }
  return false;
  }


// Save document to another name
// If file exist, prompt before overwriting
FXbool TextWindow::saveDocTo(){
  FXFileDialog savedialog(this,tr("Save File To"));
  FXString file=getFilename();
  FXint patno=getApp()->reg().readIntEntry("File Dialog","pattern",0);
  savedialog.setSelectMode(SELECTFILE_ANY);
  savedialog.setAssociations(getApp()->getFileAssociations(),false);
  savedialog.setPatternList(getPatternList());
  savedialog.setCurrentPattern(FXCLAMP(0,patno,savedialog.getNumPatterns()-1));
  savedialog.setFilename(file);
  if(savedialog.execute()){
    file=savedialog.getFilename();
    getApp()->reg().writeIntEntry("File Dialog","pattern",savedialog.getCurrentPattern());
    if(FXStat::exists(file)){
      if(FXMessageBox::warning(this,MBOX_YES_NO,tr("File Already Exists"),tr("Overwrite file: %s?"),file.text())==MBOX_CLICKED_NO) return false;
      }
    if(saveToFile(file)){
      return true;
      }
    FXMessageBox::error(this,MBOX_OK,tr("Error Saving To File"),tr("Unable to save to file: %s"),file.text());
    }
  return false;
  }


// Reload file from disk
FXbool TextWindow::reloadDoc(){
  if(isFilenameSet()){
    FXint oldtop=editor->getTopLine();
    FXint oldpos=editor->getCursorPos();
    if(loadFile(getFilename())){
      editor->setTopLine(oldtop);
      editor->setCursorPos(oldpos);
      readBookmarks(getFilename());
      determineSyntax();
      parseModeline();
      return true;
      }
    FXMessageBox::error(this,MBOX_OK,tr("Error Reloading File"),tr("Unable to reload file: %s"),getFilename().text());
    }
  return false;
  }


// Open a file
FXbool TextWindow::openDoc(){
  FXFileDialog opendialog(this,tr("Open File"));
  FXString file=getFilename();
  FXint patno=getApp()->reg().readIntEntry("File Dialog","pattern",0);
  opendialog.setSelectMode(SELECTFILE_EXISTING);
  opendialog.setAssociations(getApp()->getFileAssociations(),false);
  opendialog.setPatternList(getPatternList());
  opendialog.setCurrentPattern(FXCLAMP(0,patno,opendialog.getNumPatterns()-1));
  opendialog.setFilename(file);
  if(opendialog.execute()){
    file=opendialog.getFilename();
    getApp()->reg().writeIntEntry("File Dialog","pattern",opendialog.getCurrentPattern());
    return openDoc(file);
    }
  return false;
  }


// Open given file
FXbool TextWindow::openDoc(const FXString& file){
  TextWindow *window=getApp()->findWindow(file);
  if(!window){
    window=getApp()->findUnused();
    if(!window){
      window=new TextWindow(getApp());
      window->create();
      }
    if(!window->loadFile(file)){
      FXMessageBox::error(this,MBOX_OK,tr("Error Loading File"),tr("Unable to load file: %s"),file.text());
      return false;
      }
    window->setBrowserCurrentFile(file);
    window->readBookmarks(file);
    window->readView(file);
    window->determineSyntax();
    window->parseModeline();
    }
  window->raise();
  window->setFocus();
  window->show(PLACEMENT_DEFAULT);
  return true;
  }


// Switch to a file
FXbool TextWindow::switchDoc(){
  FXFileDialog opendialog(this,tr("Switch To File"));
  FXString file=getFilename();
  FXint patno=getApp()->reg().readIntEntry("File Dialog","pattern",0);
  opendialog.setSelectMode(SELECTFILE_EXISTING);
  opendialog.setAssociations(getApp()->getFileAssociations(),false);
  opendialog.setPatternList(getPatternList());
  opendialog.setCurrentPattern(FXCLAMP(0,patno,opendialog.getNumPatterns()-1));
  opendialog.setFilename(file);
  if(opendialog.execute()){
    file=opendialog.getFilename();
    getApp()->reg().writeIntEntry("File Dialog","pattern",opendialog.getCurrentPattern());
    return switchDoc(file);
    }
  return false;
  }


// Switch to given file
FXbool TextWindow::switchDoc(const FXString& file){
  if(loadFile(file)){
    setBrowserCurrentFile(file);
    readBookmarks(file);
    readView(file);
    determineSyntax();
    parseModeline();
    return true;
    }
  FXMessageBox::error(this,MBOX_OK,tr("Error Switching Files"),tr("Unable to load file: %s"),file.text());
  return false;
  }


// Open selected document
FXbool TextWindow::openSelDoc(){
  FXString string;

  // Get selection
  if(getDNDData(FROM_SELECTION,stringType,string)){

    // Its too big, most likely not a file name
    if(string.length()<1024){

      // Strip leading/trailing space
      string.trim();

      // Search file
      FXString file=searchForFile(string);
      FXint lineno=0;
      FXint column=0;

      // Pick apart the string
      if(file.empty()){
        FXchar name[1024];

        // Extract name from #include "file.h" syntax
        if(string.scan("#%*[ \t]include \"%1023[^\"]\"",name)==1){
          file=searchForFile(name);
          }

        // Extract name from #include <file.h> syntax
        else if(string.scan("#%*[ \t]include <%1023[^>]>",name)==1){
          file=searchForFile(name);
          }

        // Compiler output in the form <filename>:<number>:<number>: Error message
        else if(string.scan("%1023[^:]:%d:%d",name,&lineno,&column)==3){
          file=searchForFile(name);
          }

        // Compiler output in the form <filename>:<number>: Error message
        else if(string.scan("%1023[^:]:%d",name,&lineno)==2){
          file=searchForFile(name);
          }

        // Compiler output in the form <filename>(<number>) : Error message
        else if(string.scan("%1023[^(](%d)",name,&lineno)==2){
          file=searchForFile(name);
          }

        // Compiler output in the form "<filename>", line <number>: Error message
        else if(string.scan("\"%1023[^\"]\", line %d",name,&lineno)==2){
          file=searchForFile(name);
          }

        // Compiler output in the form ... File = <filename>, Line = <number>
        else if(string.scan("%*[^:]: %*s File = %1023[^,], Line = %d",name,&lineno)==2){
          file=searchForFile(name);
          }

        // Compiler output in the form filename: Other stuff
        else if(string.scan("%1023[^:]:",name)==1){
          file=searchForFile(name);
          }

        // Last straw, maybe dequote it?
        if(file.empty()){
          file=FXPath::dequote(string);
          }
        }

      // If exists, try load it!
      if(FXStat::exists(file)){

        // File loaded already?
        TextWindow *window=getApp()->findWindow(file);
        if(!window){
          window=getApp()->findUnused();
          if(!window){
            window=new TextWindow(getApp());
            window->create();
            }
          if(!window->loadFile(file)){
            FXMessageBox::error(this,MBOX_OK,tr("Error Loading File"),tr("Unable to load file: %s"),file.text());
            return false;
            }

          // Set file browser, bookmarks, etc.
          window->setBrowserCurrentFile(file);
          window->readBookmarks(file);
          window->readView(file);
          window->determineSyntax();
          window->parseModeline();
          }

        // Switch line number only
        if(lineno){
          window->visitLine(lineno,column);
          }

        // Bring up the window
        window->raise();
        window->setFocus();
        window->show(PLACEMENT_DEFAULT);
        return true;
        }
      }
    }
  return false;
  }


// Insert text from file
FXbool TextWindow::insertSel(){
  FXFileDialog opendialog(this,tr("Insert Text From File"));
  FXint patno=getApp()->reg().readIntEntry("File Dialog","pattern",0);
  opendialog.setSelectMode(SELECTFILE_EXISTING);
  opendialog.setAssociations(getApp()->getFileAssociations(),false);
  opendialog.setPatternList(getPatternList());
  opendialog.setCurrentPattern(FXCLAMP(0,patno,opendialog.getNumPatterns()-1));
  opendialog.setDirectory(FXPath::directory(getFilename()));
  if(opendialog.execute()){
    FXString file=opendialog.getFilename();
    getApp()->reg().writeIntEntry("File Dialog","pattern",opendialog.getCurrentPattern());
    if(!insertFromFile(file)){
      FXMessageBox::error(this,MBOX_OK,tr("Error Inserting From File"),tr("Unable to insert file: %s."),file.text());
      return false;
      }
    return true;
    }
  return false;
  }


// Extract selection to file
FXbool TextWindow::extractSel(){
  FXFileDialog savedialog(this,tr("Extract Text To File"));
  FXint patno=getApp()->reg().readIntEntry("File Dialog","pattern",0);
  savedialog.setSelectMode(SELECTFILE_ANY);
  savedialog.setAssociations(getApp()->getFileAssociations(),false);
  savedialog.setPatternList(getPatternList());
  savedialog.setCurrentPattern(FXCLAMP(0,patno,savedialog.getNumPatterns()-1));
  savedialog.setDirectory(FXPath::directory(getFilename()));
  savedialog.setFilename("extract");
  if(savedialog.execute()){
    FXString file=savedialog.getFilename();
    getApp()->reg().writeIntEntry("File Dialog","pattern",savedialog.getCurrentPattern());
    if(FXStat::exists(file)){
      if(FXMessageBox::warning(this,MBOX_YES_NO,tr("File Already Exists"),tr("Overwrite file: %s?"),file.text())==MBOX_CLICKED_NO) return false;
      }
    if(!extractToFile(file)){
      FXMessageBox::error(this,MBOX_OK,tr("Error Extracting To File"),tr("Unable to extract text to file: %s."),file.text());
      return false;
      }
    return true;
    }
  return false;
  }

/*******************************************************************************/

// New document
long TextWindow::onCmdNew(FXObject*,FXSelector,void*){
  newDoc();
  return 1;
  }


// New File
long TextWindow::onCmdNewFile(FXObject*,FXSelector,void*){
  createDoc();
  return 1;
  }


// Open File
long TextWindow::onCmdOpen(FXObject*,FXSelector,void*){
  openDoc();
  return 1;
  }


// Open Recent File
long TextWindow::onCmdOpenRecent(FXObject*,FXSelector,void* ptr){
  FXString file((const FXchar*)ptr);
  if(!openDoc(file)){
    mrufiles.removeFile(file);
    }
  return 1;
  }


// Open File from directory list
long TextWindow::onCmdOpenTree(FXObject*,FXSelector,void* ptr){
  FXTreeItem *item=(FXTreeItem*)ptr;
  if(item && dirlist->isItemFile(item)){
    FXString file=dirlist->getItemPathname(item);
    if(saveChanges()){
      switchDoc(file);
      }
    }
  return 1;
  }


// Switch to another file
long TextWindow::onCmdSwitch(FXObject*,FXSelector,void*){
  if(saveChanges()){
    switchDoc();
    }
  return 1;
  }


// Open file dropped into the editor
long TextWindow::onTextDNDDrop(FXObject*,FXSelector,void*){
  FXString string;
  if(getDNDData(FROM_DRAGNDROP,urilistType,string)){
    FXString file=FXURL::fileFromURL(string.before('\r'));
    if(saveChanges()){
      switchDoc(file);
      }
    return 1;
    }
  return 0;
  }


// Accept a drop if the DND type is a File (encoded as uri-list)
long TextWindow::onTextDNDMotion(FXObject*,FXSelector,void*){
  if(offeredDNDType(FROM_DRAGNDROP,urilistType)){
    acceptDrop(DRAG_COPY);
    return 1;
    }
  return 0;
  }


// Open a file from a selection in any widget anywhere on the screen
long TextWindow::onCmdOpenSelected(FXObject*,FXSelector,void*){
  openSelDoc();
  return 1;
  }


// Reload document from file
// Proceed to reload if a filename was set, the contents of the buffer were
// not modified, or it is OK to discard the modified contents
long TextWindow::onCmdReload(FXObject*,FXSelector,void*){
  if(!isModified() || FXMessageBox::warning(this,MBOX_YES_NO,tr("File Has Unsaved Changes"),tr("Discard changes and reload file: %s?"),getFilename().text())==MBOX_CLICKED_YES){
    reloadDoc();
    }
  return 1;
  }


// Update reload document from file
long TextWindow::onUpdReload(FXObject* sender,FXSelector,void* ptr){
  sender->handle(this,isFilenameSet()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),ptr);
  return 1;
  }


// Save
long TextWindow::onCmdSave(FXObject*,FXSelector,void*){
  if(isFilenameSet()){
    saveDoc();
    return 1;
    }
  saveDocAs();
  return 1;
  }


// Save Update
long TextWindow::onUpdSave(FXObject* sender,FXSelector,void*){
  sender->handle(this,isModified()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Save As
long TextWindow::onCmdSaveAs(FXObject*,FXSelector,void*){
  saveDocAs();
  return 1;
  }


// Save To
long TextWindow::onCmdSaveTo(FXObject*,FXSelector,void*){
  saveDocTo();
  return 1;
  }


// Replace by text from file
long TextWindow::onCmdInsertFile(FXObject*,FXSelector,void*){
  insertSel();
  return 1;
  }


// Extract selection to file
long TextWindow::onCmdExtractFile(FXObject*,FXSelector,void*){
  extractSel();
  return 1;
  }


// User clicks on one of the window menus
long TextWindow::onCmdWindow(FXObject*,FXSelector sel,void*){
  FXint which=FXSELID(sel)-ID_WINDOW_1;
  if(which<getApp()->numWindows()){
    getApp()->getWindow(which)->raise();
    getApp()->getWindow(which)->setFocus();
    }
  return 1;
  }


// Update handler for window menus
long TextWindow::onUpdWindow(FXObject *sender,FXSelector sel,void*){
  FXint which=FXSELID(sel)-ID_WINDOW_1;
  if(which<getApp()->numWindows()){
    TextWindow *window=getApp()->getWindow(which);
    FXString string=FXString::value("%d %s",which+1,window->getTitle().text());
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&string);
    if(window==getApp()->getActiveWindow())
      sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_CHECK),nullptr);
    else
      sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_UNCHECK),nullptr);
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SHOW),nullptr);
    }
  else{
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_HIDE),nullptr);
    }
  return 1;
  }


// Update title from current filename
long TextWindow::onUpdate(FXObject* sender,FXSelector sel,void* ptr){
  FXMainWindow::onUpdate(sender,sel,ptr);
  FXString ttl=FXPath::name(getFilename());
  if(isModified()) ttl.append(tr(" (changed)"));
  FXString directory=FXPath::directory(getFilename());
  if(!directory.empty()) ttl.append(" - " + directory);
  setTitle(ttl);
  return 1;
  }


// Print the text
long TextWindow::onCmdPrint(FXObject*,FXSelector,void*){
  FXPrintDialog dlg(this,tr("Print File"));
  FXPrinter printer;
  if(dlg.execute()){
    dlg.getPrinter(printer);
    FXTRACE((100,"Printer = %s\n",printer.name.text()));
    }
  return 1;
  }


// Toggle file browser
long TextWindow::onCmdToggleBrowser(FXObject*,FXSelector,void*){
  if(treebox->shown()){
    treebox->hide();
    position(getX(),getY(),getWidth()-treebox->getWidth(),getHeight());
    }
  else{
    treebox->show();
    position(getX(),getY(),getWidth()+treebox->getWidth(),getHeight());
    }
  return 1;
  }


// Toggle file browser
long TextWindow::onUpdToggleBrowser(FXObject* sender,FXSelector,void*){
  sender->handle(this,treebox->shown() ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Toggle hidden files display
long TextWindow::onCmdToggleHidden(FXObject*,FXSelector,void*){
  dirlist->showHiddenFiles(!dirlist->showHiddenFiles());
  if(dirlist->getCurrentItem()){
    dirlist->makeItemVisible(dirlist->getCurrentItem());
    }
  return 1;
  }


// Update toggle hidden files widget
long TextWindow::onUpdToggleHidden(FXObject* sender,FXSelector,void*){
  sender->handle(this,dirlist->showHiddenFiles()?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }

/*******************************************************************************/

// Save settings
long TextWindow::onCmdSaveSettings(FXObject*,FXSelector,void*){
  writeRegistry();
  getApp()->reg().write();
  return 1;
  }


// Toggle mode line parsing
long TextWindow::onCmdModeline(FXObject*,FXSelector,void*){
  modeline=!modeline;
  return 1;
  }


// Update mode line parsing
long TextWindow::onUpdModeline(FXObject* sender,FXSelector,void*){
  sender->handle(this,modeline ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Toggle autoindent; we remember this value as the default
long TextWindow::onCmdAutoIndent(FXObject*,FXSelector,void*){
  FXuint modebits=editor->getTextStyle()^TEXT_AUTOINDENT;
  editor->setTextStyle(modebits);
  autoindent=((modebits&TEXT_AUTOINDENT)!=0);
  return 1;
  }


// Update autoindent; show indeterminate if default is different from current
long TextWindow::onUpdAutoIndent(FXObject* sender,FXSelector,void*){
  FXbool ai=(editor->getTextStyle()&TEXT_AUTOINDENT)!=0;
  sender->handle(this,(autoindent!=ai) ? FXSEL(SEL_COMMAND,ID_UNKNOWN) : ai ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change wrap columns; we remember this value as the default
long TextWindow::onCmdWrapColumns(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&wrapcols);
  editor->setWrapColumns(wrapcols);
  return 1;
  }


// Update wrap columns
long TextWindow::onUpdWrapColumns(FXObject* sender,FXSelector,void*){
  FXint wrap=editor->getWrapColumns();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&wrap);
  return 1;
  }


// Toggle wrap mode; we remember this value as the default
long TextWindow::onCmdWrap(FXObject*,FXSelector,void*){
  FXuint modebits=editor->getTextStyle()^TEXT_WORDWRAP;
  editor->setTextStyle(modebits);
  wrapping=((modebits&TEXT_WORDWRAP)!=0);
  return 1;
  }


// Update toggle wrap mode
long TextWindow::onUpdWrap(FXObject* sender,FXSelector,void*){
  FXbool wr=(editor->getTextStyle()&TEXT_WORDWRAP)!=0;
  sender->handle(this,(wrapping!=wr) ? FXSEL(SEL_COMMAND,ID_UNKNOWN) : wr ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Toggle fixed wrap mode; we remember this value as the default
long TextWindow::onCmdWrapFixed(FXObject*,FXSelector,void*){
  FXuint modebits=editor->getTextStyle()^TEXT_FIXEDWRAP;
  editor->setTextStyle(modebits);
  fixedwrap=((modebits&TEXT_FIXEDWRAP)!=0);
  return 1;
  }


// Update toggle fixed wrap mode; show indeterminate if default is different from current
long TextWindow::onUpdWrapFixed(FXObject* sender,FXSelector,void*){
  FXbool fw=(editor->getTextStyle()&TEXT_FIXEDWRAP)!=0;
  sender->handle(this,(fixedwrap!=fw) ? FXSEL(SEL_COMMAND,ID_UNKNOWN) : fw ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change tab columns; we remember this value as the default
long TextWindow::onCmdTabColumns(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&tabcols);
  editor->setTabColumns(tabcols);
  return 1;
  }


// Update tab columns
long TextWindow::onUpdTabColumns(FXObject* sender,FXSelector,void*){
  FXint tabs=editor->getTabColumns();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&tabs);
  return 1;
  }


// Select tab columns; we remember this value as the default
long TextWindow::onCmdTabSelect(FXObject*,FXSelector sel,void*){
  tabcols=FXSELID(sel)-ID_TABSELECT_0;
  editor->setTabColumns(tabcols);
  return 1;
  }


// Update select tab columns
long TextWindow::onUpdTabSelect(FXObject* sender,FXSelector sel,void*){
  FXint tabs=FXSELID(sel)-ID_TABSELECT_0;
  sender->handle(this,editor->getTabColumns()==tabs?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Toggle insertion of tabs; we remember this value as the default
long TextWindow::onCmdInsertTabs(FXObject*,FXSelector,void*){
  FXuint modebits=editor->getTextStyle()^TEXT_NO_TABS;
  editor->setTextStyle(modebits);
  hardtabs=((modebits&TEXT_NO_TABS)==0);
  return 1;
  }


// Update insertion of tabs; show indeterminate if default is different from current
long TextWindow::onUpdInsertTabs(FXObject* sender,FXSelector,void*){
  FXbool ht=(editor->getTextStyle()&TEXT_NO_TABS)==0;
  sender->handle(this,(hardtabs!=ht) ? FXSEL(SEL_COMMAND,ID_UNKNOWN) : ht ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Toggle show active background mode
long TextWindow::onCmdShowActive(FXObject*,FXSelector,void*){
  editor->setTextStyle(editor->getTextStyle()^TEXT_SHOWACTIVE);
  return 1;
  }


// Update show active background mode
long TextWindow::onUpdShowActive(FXObject* sender,FXSelector,void*){
  sender->handle(this,(editor->getTextStyle()&TEXT_SHOWACTIVE) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Toggle strip returns mode
long TextWindow::onCmdStripReturns(FXObject*,FXSelector,void* ptr){
  stripcr=!!ptr;
  return 1;
  }


// Update toggle strip returns mode
long TextWindow::onUpdStripReturns(FXObject* sender,FXSelector,void*){
  sender->handle(this,stripcr ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Enable warning if file changed externally
long TextWindow::onCmdWarnChanged(FXObject*,FXSelector,void* ptr){
  warnchanged=!!ptr;
  return 1;
  }


// Update check button for warning
long TextWindow::onUpdWarnChanged(FXObject* sender,FXSelector,void*){
  sender->handle(this,warnchanged ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Set initial size flag
long TextWindow::onCmdUseInitialSize(FXObject*,FXSelector,void* ptr){
  initialsize=!!ptr;
  return 1;
  }


// Update initial size flag
long TextWindow::onUpdUseInitialSize(FXObject* sender,FXSelector,void*){
  sender->handle(this,initialsize ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Remember this as the initial size
long TextWindow::onCmdSetInitialSize(FXObject*,FXSelector,void*){
  initialwidth=getWidth();
  initialheight=getHeight();
  return 1;
  }


// Toggle strip spaces mode
long TextWindow::onCmdStripSpaces(FXObject*,FXSelector,void* ptr){
  stripsp=!!ptr;
  return 1;
  }


// Update toggle strip spaces mode
long TextWindow::onUpdStripSpaces(FXObject* sender,FXSelector,void*){
  sender->handle(this,stripsp ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Toggle append newline mode
long TextWindow::onCmdAppendNewline(FXObject*,FXSelector,void* ptr){
  appendnl=!!ptr;
  return 1;
  }


// Update toggle append newline mode
long TextWindow::onUpdAppendNewline(FXObject* sender,FXSelector,void*){
  sender->handle(this,appendnl ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }



// Toggle append carriage return mode
long TextWindow::onCmdAppendCarriageReturn(FXObject*,FXSelector,void* ptr){
  appendcr=!!ptr;
  return 1;
  }


// Update toggle append carriage return mode
long TextWindow::onUpdAppendCarriageReturn(FXObject* sender,FXSelector,void*){
  sender->handle(this,appendcr ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change line number column
long TextWindow::onCmdLineNumbers(FXObject* sender,FXSelector,void*){
  FXint cols;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&cols);
  editor->setBarColumns(cols);
  return 1;
  }


// Update line number column
long TextWindow::onUpdLineNumbers(FXObject* sender,FXSelector,void*){
  FXint cols=editor->getBarColumns();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&cols);
  return 1;
  }


// Set brace matching
long TextWindow::onCmdBraceMatch(FXObject*,FXSelector,void*){
  editor->setTextStyle(editor->getTextStyle()^TEXT_SHOWMATCH);
  return 1;
  }


// Update brace matching
long TextWindow::onUpdBraceMatch(FXObject* sender,FXSelector,void*){
  sender->handle(this,(editor->getTextStyle()&TEXT_SHOWMATCH) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Set brace match time
long TextWindow::onCmdBraceMatchTime(FXObject* sender,FXSelector,void*){
  FXTime value;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETLONGVALUE),(void*)&value);
  editor->setHiliteMatchTime(value*1000000);
  return 1;
  }


// Update brace match time
long TextWindow::onUpdBraceMatchTime(FXObject* sender,FXSelector,void*){
  FXTime value=editor->getHiliteMatchTime();
  if(value<forever){
    value/=1000000;
    sender->handle(this,FXSEL(SEL_COMMAND,ID_SETLONGVALUE),(void*)&value);
    }
  else{
    sender->handle(this,FXSEL(SEL_COMMAND,ID_SETVALUE),(void*)"forever");
    }
  return 1;
  }


// Set brace match to remain highlighted forever
long TextWindow::onCmdBraceMatchStay(FXObject*,FXSelector,void*){
  FXTime value=editor->getHiliteMatchTime();
  value=(value<forever) ? forever : 2000000000;
  editor->setHiliteMatchTime(value);
  return 1;
  }


// Update brace match remain highlighted forever
long TextWindow::onUpdBraceMatchStay(FXObject* sender,FXSelector,void*){
  FXTime value=editor->getHiliteMatchTime();
  sender->handle(this,(value==forever) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change word delimiters
long TextWindow::onCmdDelimiters(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETSTRINGVALUE),(void*)&delimiters);
  editor->setDelimiters(delimiters.text());
  return 1;
  }


// Update word delimiters
long TextWindow::onUpdDelimiters(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&delimiters);
  return 1;
  }


// Update box for overstrike mode display
long TextWindow::onUpdOverstrike(FXObject* sender,FXSelector,void*){
  FXString mode((editor->getTextStyle()&TEXT_OVERSTRIKE)?"OVR":"INS");
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&mode);
  return 1;
  }


// Update box for readonly display
long TextWindow::onUpdReadOnly(FXObject* sender,FXSelector,void*){
  FXString rw((editor->getTextStyle()&TEXT_READONLY)?"RO":"RW");
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&rw);
  return 1;
  }


// Update box for tabmode display
long TextWindow::onUpdTabMode(FXObject* sender,FXSelector,void*){
  FXString tab((editor->getTextStyle()&TEXT_NO_TABS)?"EMT":"TAB");
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&tab);
  return 1;
  }


// Set scroll wheel lines (Mathew Robertson <mathew@optushome.com.au>)
long TextWindow::onCmdWheelAdjust(FXObject* sender,FXSelector,void*){
  FXint value;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&value);
  editor->verticalScrollBar()->setWheelLines(value);
  return 1;
  }


// Update scroll wheel lines
long TextWindow::onUpdWheelAdjust(FXObject* sender,FXSelector,void*){
  FXint value=editor->verticalScrollBar()->getWheelLines();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&value);
  return 1;
  }


/*******************************************************************************/


// Change text color
long TextWindow::onCmdTextForeColor(FXObject*,FXSelector,void* ptr){
  editor->setTextColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Update text color
long TextWindow::onUpdTextForeColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getTextColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change text background color
long TextWindow::onCmdTextBackColor(FXObject*,FXSelector,void* ptr){
  editor->setBackColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Update background color
long TextWindow::onUpdTextBackColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getBackColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change selected text foreground color
long TextWindow::onCmdTextSelForeColor(FXObject*,FXSelector,void* ptr){
  editor->setSelTextColor((FXColor)(FXuval)ptr);
  return 1;
  }


// Update selected text foregoround color
long TextWindow::onUpdTextSelForeColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getSelTextColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change selected text background color
long TextWindow::onCmdTextSelBackColor(FXObject*,FXSelector,void* ptr){
  editor->setSelBackColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Update selected text background color
long TextWindow::onUpdTextSelBackColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getSelBackColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change hilight text color
long TextWindow::onCmdTextHiliteForeColor(FXObject*,FXSelector,void* ptr){
  editor->setHiliteTextColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Update hilight text color
long TextWindow::onUpdTextHiliteForeColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getHiliteTextColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change hilight text background color
long TextWindow::onCmdTextHiliteBackColor(FXObject*,FXSelector,void* ptr){
  editor->setHiliteBackColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Update hilight text background color
long TextWindow::onUpdTextHiliteBackColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getHiliteBackColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change active text background color
long TextWindow::onCmdTextActBackColor(FXObject*,FXSelector,void* ptr){
  editor->setActiveBackColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Update active text background color
long TextWindow::onUpdTextActBackColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getActiveBackColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change cursor color
long TextWindow::onCmdTextCursorColor(FXObject*,FXSelector,void* ptr){
  editor->setCursorColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Update cursor color
long TextWindow::onUpdTextCursorColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getCursorColor();
  sender->handle(sender,FXSEL(SEL_COMMAND,FXWindow::ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change line numbers background color
long TextWindow::onCmdTextBarColor(FXObject*,FXSelector,void* ptr){
  editor->setBarColor((FXColor)(FXuval)ptr);
  return 1;
  }


// Update line numbers background color
long TextWindow::onUpdTextBarColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getBarColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }

// Change line numbers color
long TextWindow::onCmdTextNumberColor(FXObject*,FXSelector,void* ptr){
  editor->setNumberColor((FXColor)(FXuval)ptr);
  return 1;
  }


// Update line numbers color
long TextWindow::onUpdTextNumberColor(FXObject* sender,FXSelector,void*){
  FXColor color=editor->getNumberColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change both tree background color
long TextWindow::onCmdDirBackColor(FXObject*,FXSelector,void* ptr){
  dirlist->setBackColor((FXColor)(FXuval)ptr);
  return 1;
  }


// Update background color
long TextWindow::onUpdDirBackColor(FXObject* sender,FXSelector,void*){
  FXColor color=dirlist->getBackColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change both text and tree selected background color
long TextWindow::onCmdDirSelBackColor(FXObject*,FXSelector,void* ptr){
  dirlist->setSelBackColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Update selected background color
long TextWindow::onUpdDirSelBackColor(FXObject* sender,FXSelector,void*){
  FXColor color=dirlist->getSelBackColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change both text and tree text color
long TextWindow::onCmdDirForeColor(FXObject*,FXSelector,void* ptr){
  dirlist->setTextColor((FXColor)(FXuval)ptr);
  return 1;
  }

// Forward GUI update to text widget
long TextWindow::onUpdDirForeColor(FXObject* sender,FXSelector,void*){
  FXColor color=dirlist->getTextColor();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change both text and tree
long TextWindow::onCmdDirSelForeColor(FXObject*,FXSelector,void* ptr){
  dirlist->setSelTextColor((FXColor)(FXuval)ptr);
  return 1;
  }


// Forward GUI update to text widget
long TextWindow::onUpdDirSelForeColor(FXObject* sender,FXSelector,void*){
  FXColor color=dirlist->getSelTextColor();
  sender->handle(sender,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change both text and tree
long TextWindow::onCmdDirLineColor(FXObject*,FXSelector,void* ptr){
  dirlist->setLineColor((FXColor)(FXuval)ptr);
  return 1;
  }


// Forward GUI update to text widget
long TextWindow::onUpdDirLineColor(FXObject* sender,FXSelector,void*){
  FXColor color=dirlist->getLineColor();
  sender->handle(sender,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&color);
  return 1;
  }


// Change the pattern
long TextWindow::onCmdFilter(FXObject*,FXSelector,void* ptr){
  dirlist->setPattern(FXFileSelector::patternFromText((FXchar*)ptr));
  dirlist->makeItemVisible(dirlist->getCurrentItem());
  return 1;
  }


// Change search paths
long TextWindow::onCmdSearchPaths(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETSTRINGVALUE),(void*)&searchpaths);
  return 1;
  }


// Update search paths
long TextWindow::onUpdSearchPaths(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&searchpaths);
  return 1;
  }


// Find in files
long TextWindow::onCmdFindInFiles(FXObject*,FXSelector,void*){
  FindInFiles *findwindow=new FindInFiles(getApp());
  findwindow->setPatternList(getPatternList());
  findwindow->setDirectory(FXPath::directory(getFilename()));
  findwindow->setSearchText(searchstring);
  findwindow->create();
  findwindow->show(PLACEMENT_CURSOR);
  return 1;
  }


/*******************************************************************************/

// Evaluate expression
long TextWindow::onCmdExpression(FXObject*,FXSelector,void*){
  FXString string(editor->getSelectedText());
  FXExpression expression;
  if(expression.parse(string)==FXExpression::ErrOK){
    FXString result(FXString::value(expression.evaluate(),15,2));
    editor->replaceSelection(result,true);
    editor->setFocus();
    return 1;
    }
  return 1;
  }


// Update evaluate expression; ensure that no more than one line is selected
long TextWindow::onUpdExpression(FXObject* sender,FXSelector,void*){
  if(editor->getSelStartPos()<editor->getSelEndPos()){
    FXString string(editor->getSelectedText());
    FXExpression expression;
    if(expression.parse(string)==FXExpression::ErrOK){
      sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),nullptr);
      return 1;
      }
    }
  sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }

/*******************************************************************************/

// Encode URL characters
long TextWindow::onCmdURLEncode(FXObject*,FXSelector,void*){
  FXString string(editor->getSelectedText());
  editor->replaceSelection(FXURL::encode(string,"<>#%{}|^~[]`\"\\?$&'*,;= @:"),true);
  return 1;
  }

// Decode URL characters
long TextWindow::onCmdURLDecode(FXObject*,FXSelector,void*){
  FXString string(editor->getSelectedText());
  editor->replaceSelection(FXURL::decode(string),true);
  return 1;
  }


// Enable url decode/encode if single line
long TextWindow::onUpdURLCoding(FXObject* sender,FXSelector,void*){
  FXbool onelineselected=editor->getSelStartPos()<editor->getSelEndPos() && editor->getSelEndPos()<=editor->lineEnd(editor->getSelStartPos());
  sender->handle(this,onelineselected?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }

/*******************************************************************************/

// Parse associated command string exec
// This command string may contain some special codes which are expanded
// in this routine.
//
// The special codes are as follows:
//
//      %f           Replaced by (quoted) current full path name
//      %F           Replaced by the (quoted) selected full path names
//      %n           Replaced by the (quoted) current file name
//      %N           Replaced by the (quoted) selected file names
//      %u           Replaced by (quoted) URL-encoded of the current filename
//      %U           Replaced by the (quoted) URL-encoded selected filenames
//      %d           Replaced by current directory
//      %%           Replaced by '%'
//
// Quoting is performed along the needs of the shell; this is slightly different
// between UNIX and Windows.
FXString TextWindow::makeCommandline(const FXString& cmd) const {
  FXString result;
  FXint s=0,q=0;
  while(q<cmd.length()){
    if(cmd[q++]=='%'){
      result.append(&cmd[s],q-s-1);
      switch(cmd[q++]){
      case 'f':         // One file
        result.append(' ');
        result.append(FXPath::enquote(getFilename()));
        break;
      case 'F':         // Multiple files
        for(FXint w=0; w<getApp()->numWindows(); ++w){
          result.append(' ');
          result.append(FXPath::enquote(getApp()->getWindow(w)->getFilename()));
          }
        break;
      case 'n':         // Name only
        result.append(' ');
        result.append(FXPath::enquote(FXPath::name(getFilename())));
        break;
      case 'N':         // Multiple files, names only
        for(FXint w=0; w<getApp()->numWindows(); ++w){
          result.append(' ');
          result.append(FXPath::enquote(FXPath::name(getApp()->getWindow(w)->getFilename())));
          }
        break;
      case 'u':         // URL-ified filename
        result.append(' ');
        result.append(FXPath::enquote(FXURL::fileToURL(getFilename())));
        break;
      case 'U':         // URL-ified multiple filenames
        for(FXint w=0; w<getApp()->numWindows(); ++w){
          result.append(' ');
          result.append(FXPath::enquote(FXURL::fileToURL(getApp()->getWindow(w)->getFilename())));
          }
        break;
      case 'd':         // Current directory
        result.append(' ');
        result.append(FXPath::enquote(FXPath::directory(getFilename())));
        break;
      case '%':         // We wanted a '%'
        result.append('%');
        break;
        }
      s=q;
      }
    }
  result.append(&cmd[s],q-s);
  return result;
  }

// FIXME editable flag should be off while running...

// Start shell command
FXbool TextWindow::startCommand(const FXString& command,FXint inp,FXint out,FXint flgs){
  if(!shellCommand){
    FXTextSelection sel;
    FXString input;

    // No selection
    sel.startpos=0;
    sel.endpos=-1;
    sel.startcol=0;
    sel.endcol=-1;

    // Whole document is input
    if(inp==FROM_DOC || inp==FROM_ANY){
      sel.startpos=0;
      sel.endpos=editor->getLength();
      }

    // Selection is input
    if(editor->hasSelection()){
      if(inp==FROM_SEL || inp==FROM_ANY){
        sel.startpos=editor->getSelStartPos();
        sel.endpos=editor->getSelEndPos();
        sel.startcol=editor->getSelStartColumn();
        sel.endcol=editor->getSelEndColumn();
        }
      }

    // Grab input
    input=editor->getSelectedText();

    // Insert at cursor
    replace.startpos=editor->getCursorPos();
    replace.endpos=replace.startpos;
    replace.startcol=0;
    replace.endcol=-1;

    // FIXME maybe a horizontal, multi-column "cursor" is
    // just as useful as a vertical multi-row "cursor" ?
/*
    // Output
    if(out==TO_REP){
      replace=sel;
      }
    else if(out==TO_INS){
      }
*/

    // New shell command instance
    shellCommand=new ShellCommand(this,FXSEL(SEL_COMMAND,ID_SHELL_OUTPUT),FXSEL(SEL_COMMAND,ID_SHELL_ERROR),FXSEL(SEL_COMMAND,ID_SHELL_DONE));

    // Start program in same directory as current file
    shellCommand->setDirectory(FXPath::directory(getFilename()));

    // Pass input buffer
    shellCommand->setInput(input);

    // Launch it
    if(!shellCommand->start(command)){
      FXMessageBox::error(this,MBOX_OK,tr("Command Error"),tr("Unable to execute command: %s"),command.text());
      delete shellCommand;
      shellCommand=nullptr;
      return false;
      }

    // Display wait cursor
    getApp()->beginWaitCursor();

    // All input will be undoable
    undolist.begin(new FXCommandGroup);

    // Clear logger area
    logger->clearText();

    return true;
    }
  return false;
  }


// Start shell command
FXbool TextWindow::startCommand(const FXString& command,FXString& input){
  if(!shellCommand){

    // New shell command instance
    shellCommand=new ShellCommand(this,FXSEL(SEL_COMMAND,ID_SHELL_OUTPUT),FXSEL(SEL_COMMAND,ID_SHELL_ERROR),FXSEL(SEL_COMMAND,ID_SHELL_DONE));

    // Start program in same directory as current file
    shellCommand->setDirectory(FXPath::directory(getFilename()));

    // Pass input buffer
    shellCommand->setInput(input);

    // Launch it
    if(!shellCommand->start(command)){
      FXMessageBox::error(this,MBOX_OK,tr("Command Error"),tr("Unable to execute command: %s"),command.text());
      delete shellCommand;
      shellCommand=nullptr;
      return false;
      }

    // Display wait cursor
    getApp()->beginWaitCursor();

    // All input will be undoable
    undolist.begin(new FXCommandGroup);

    // Clear logger area
    logger->clearText();

    return true;
    }
  return false;
  }


// Stop shell command
FXbool TextWindow::stopCommand(){
  if(shellCommand){

    // Close undo command
    undolist.end();

    // Hide logger
    if(!showlogger){
      loggerframe->hide();
      loggerframe->recalc();
      }

    // Kill the command
    shellCommand->cancel();

    // Display normal cursor
    getApp()->endWaitCursor();

    // Clean up the mess
    delete shellCommand;
    shellCommand=nullptr;
    return true;
    }
  return false;
  }


// Done with command
FXbool TextWindow::doneCommand(){
  if(shellCommand){

    // Close undo command
    undolist.end();

    // Display normal cursor
    getApp()->endWaitCursor();

    // Clean up the mess
    delete shellCommand;
    shellCommand=nullptr;
    return true;
    }
  return false;
  }


// Shell command dialog
long TextWindow::onCmdShellDialog(FXObject*,FXSelector,void*){
  if(!shellCommand){
    ShellDialog dialog(this,tr("Execute Command"),tr("&Execute shell command:"));
    if(dialog.execute(PLACEMENT_OWNER)){

      // Output goes to insertion point
      replaceStart=replaceEnd=editor->getCursorPos();

      // Get text
      FXString text=dialog.getText();

      // Replace magic %f etc.
      FXString command=makeCommandline(text);

//fxmessage("Execute: \"%s\" cmdline: \"%s\"\n",text.text(),command.text());
//return 1;

      // No input
      FXString input;

      // Start it
      startCommand(command,input);
      }
    }
  return 1;
  }


// Update command dialog
long TextWindow::onUpdShellDialog(FXObject* sender,FXSelector,void*){
  sender->handle(this,!shellCommand?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Execute current line as command line
long TextWindow::onCmdShellCommand(FXObject*,FXSelector,void*){
  if(!shellCommand){
    FXint currentpos=editor->getCursorPos();
    FXint startpos=editor->lineStart(currentpos);
    FXint endpos=editor->lineEnd(currentpos);
    if(startpos<endpos){

      // Get command from text buffer
      FXString command=editor->extractText(startpos,endpos-startpos);

      // No input
      FXString input;

      // Add newline
      editor->insertText(endpos,"\n",1,true);

      // Output goes to insertion point
      replaceStart=replaceEnd=endpos+1;

      // Start
      startCommand(command,input);
      }
    }
  return 1;
  }


// Update command line
long TextWindow::onUpdShellCommand(FXObject* sender,FXSelector,void*){
  sender->handle(this,!shellCommand?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Filter selection through shell command
long TextWindow::onCmdShellFilter(FXObject*,FXSelector,void*){
  if(!shellCommand){
    ShellDialog dialog(this,tr("Filter Selection"),tr("&Filter selection with shell command:"));
    if(dialog.execute(PLACEMENT_OWNER)){

      // Get selection range
      replaceStart=0;
      replaceEnd=editor->getLength();
      if(editor->hasSelection()){
        replaceStart=editor->getSelStartPos();
        replaceEnd=editor->getSelEndPos();
        }

      // Get command
      FXString command=dialog.getText();

      // Grab selected text to feed into command
      FXString input=editor->extractText(replaceStart,replaceEnd-replaceStart);

      // Start
      startCommand(command,input);
      }
    }
  return 1;
  }


// Update filter selection
long TextWindow::onUpdShellFilter(FXObject* sender,FXSelector,void*){
  sender->handle(this,!shellCommand?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Cancel shell command
long TextWindow::onCmdShellCancel(FXObject*,FXSelector,void*){
  stopCommand();
  return 1;
  }


// Update shell command
long TextWindow::onUpdShellCancel(FXObject* sender,FXSelector,void*){
  sender->handle(this,shellCommand?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Output from shell
long TextWindow::onCmdShellOutput(FXObject*,FXSelector,void* ptr){
  FXTRACE((100,"TextWindow::onCmdShellOutput\n"));
  const FXchar* string=(const FXchar*)ptr;
  FXint len=strlen(string);
  if(replaceStart>editor->getLength()) replaceStart=editor->getLength();
  if(replaceEnd>editor->getLength()) replaceEnd=editor->getLength();
  editor->replaceText(replaceStart,replaceEnd-replaceStart,string,len,true);
  replaceStart=replaceStart+len;
  replaceEnd=replaceStart;
  editor->setCursorPos(replaceEnd);
  editor->makePositionVisible(replaceEnd);
  return 1;
  }


// Shell command has error
long TextWindow::onCmdShellError(FXObject*,FXSelector,void* ptr){
  FXTRACE((100,"TextWindow::onCmdShellError\n"));
  const FXchar* string=(const FXchar*)ptr;
  FXint len=strlen(string);
  showlogger=loggerframe->shown();
  if(!showlogger){
    loggerframe->show();
    loggerframe->recalc();
    }
  logger->appendText(string,len,true);
  logger->setCursorPos(logger->getLength());
  logger->makePositionVisible(logger->getLength());
  return 1;
  }


// Shell command is done
long TextWindow::onCmdShellDone(FXObject*,FXSelector,void*){
  FXTRACE((100,"TextWindow::onCmdShellDone\n"));
  doneCommand();
  return 1;
  }


// Shell command section key
const FXchar shellSectionKey[]="ShellCommands";

// Shell command entry key
const FXchar shellEntryKey[16][5]={
  {"CMD0"},{"CMD1"},{"CMD2"},{"CMD3"},{"CMD4"},{"CMD5"},{"CMD6"},{"CMD7"},
  {"CMD8"},{"CMD9"},{"CMDA"},{"CMDB"},{"CMDC"},{"CMDD"},{"CMDE"},{"CMDF"}
  };


// Example
// [ShellCommands]
// CMD1="Word Count:Count words in file.:wc -w %f:WAG"

// Shell menu command
long TextWindow::onCmdShellMenu(FXObject*,FXSelector sel,void*){
  FXint which=FXSELID(sel)-ID_CUST_CMD0;
  FXchar cmdline[1000],cmdopts[8];
  if(getApp()->reg().readFormatEntry(shellSectionKey,shellEntryKey[which],"%*100[^:]:%*200[^:]:%1000[^:]:%8[^:]",cmdline,cmdopts)==2){
    FXString command=makeCommandline(cmdline);
    FXint cmdflg=0;
    FXint cmdout=0;
    FXint cmdinp=0;
    for(FXint f=0; cmdopts[f]; ++f){
      switch(cmdopts[f]){
        case 'S': cmdinp=FROM_SEL; break;
        case 'D': cmdinp=FROM_DOC; break;
        case 'A': cmdinp=FROM_ANY; break;
        case 'I': cmdout=TO_INS; break;
        case 'R': cmdout=TO_REP; break;
        case 'G': cmdout=TO_LOG; break;
        case 'N': cmdout=TO_NEW; break;
        case 'W': cmdflg|=SAVE_DOC; break;
        case 'L': cmdflg|=LOAD_DOC; break;
        }
      }
    fprintf(stderr,"Execute: \"%s\" cmdline: \"%s\" cmdopts: \"%s\" in: %d out: %d flg: %d\n",command.text(),cmdline,cmdopts,cmdinp,cmdout,cmdflg);
    startCommand(command,cmdinp,cmdout,cmdflg);
    }
  return 1;
  }


// Update shell menu command
long TextWindow::onUpdShellMenu(FXObject* sender,FXSelector sel,void*){
  FXint which=FXSELID(sel)-ID_CUST_CMD0;
  FXchar cmdname[100],cmdhelp[200];
  if(getApp()->reg().readFormatEntry(shellSectionKey,shellEntryKey[which],"%100[^:]:%200[^:]:%*1000[^:]:%*8[^:]",cmdname,cmdhelp)==2){
    FXString label(cmdname),help(cmdhelp);
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&label);
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETHELPSTRING),(void*)&help);
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SHOW),nullptr);
    }
  else{
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_HIDE),nullptr);
    }
  return 1;
  }

/*******************************************************************************/

// Goto line number
long TextWindow::onCmdGotoLine(FXObject*,FXSelector,void*){
  FXGIFIcon dialogicon(getApp(),goto_gif);
  FXint row=editor->getCursorRow()+1;
  if(FXInputDialog::getInteger(row,this,tr("Goto Line"),tr("&Goto line number:"),&dialogicon,1,2147483647)){
    editor->setCursorRow(row-1,true);
    editor->makePositionVisible(editor->getCursorPos());
    }
  return 1;
  }


// Goto selected line number
long TextWindow::onCmdGotoSelected(FXObject*,FXSelector,void*){
  FXString string;
  if(getDNDData(FROM_SELECTION,stringType,string)){
    FXint row=0,s;
    if((s=string.find_first_of("0123456789"))>=0){
      while(Ascii::isDigit(string[s])){
        row=row*10+Ascii::digitValue(string[s]);
        s++;
        }
      if(1<=row){
        editor->setCursorRow(row-1,true);
        editor->makePositionVisible(editor->getCursorPos());
        return 1;
        }
      }
    }
  return 1;
  }

/*******************************************************************************/

// Check if the selection (if any) matches the pattern
FXbool TextWindow::matchesSelection(const FXString& string,FXint* beg,FXint* end,FXuint flgs,FXint npar) const {
  FXint selstart=editor->getSelStartPos();
  FXint selend=editor->getSelEndPos();
  if((selstart<selend) && (0<npar)){
    if(editor->findText(string,beg,end,selstart,flgs&~(SEARCH_FORWARD|SEARCH_BACKWARD),npar)){
      return (beg[0]==selstart) && (end[0]==selend);
      }
    }
  return false;
  }


// Search text
long TextWindow::onCmdSearch(FXObject*,FXSelector,void*){
  FXTRACE((100,"TextWindow::onCmdSearch()\n"));
  FXGIFIcon dialogicon(getApp(),searchicon_gif);
  FXSearchDialog searchdialog(this,tr("Search"),&dialogicon);
  FXint    beg[10],end[10],pos,code;
  FXuint   placement=PLACEMENT_OWNER;
  FXString findstring;
  FXuint   findflags;

  // Start the search
  setStatusMessage(tr("Search for a string in the file."));

  // Start with same flags as last time
//  searchdialog.setSearchMode(searchflags);
//  searchdialog.setSearchText(searchstring);

  // First time, throw dialog over window
  while((code=searchdialog.execute(placement))!=FXSearchDialog::DONE){

    // User may have moved the panel
    placement=PLACEMENT_DEFAULT;

    // Grab the search parameters
//    searchstring=searchdialog.getSearchText();
//    searchflags=searchdialog.getSearchMode();
    findstring=searchdialog.getSearchText();
    findflags=searchdialog.getSearchMode();

    // If search string matches the selection, start from the end (or begin
    // when seaching backwards) of the selection.  Otherwise proceed from the
    // cursor position.
    pos=editor->getCursorPos();
    if(matchesSelection(findstring,beg,end,findflags,10)){
      pos=(findflags&SEARCH_BACKWARD) ? beg[0]-1 : end[0];
      }

    // Search the text
    if(editor->findText(findstring,beg,end,pos,findflags,10)){

      // Feed back success, search box turns green
      setStatusMessage(tr("String found!"));
      searchdialog.setSearchTextColor(FXRGB(128,255,128));

      // Flag a wraparound the text
      if(findflags&SEARCH_BACKWARD){
        if(pos<=beg[0]){ setStatusMessage(tr("Search wrapped around.")); }
        }
      else{
        if(beg[0]<pos){ setStatusMessage(tr("Search wrapped around.")); }
        }

      // Beep if same location in buffer
      if((beg[0]==editor->getSelStartPos()) && (end[0]==editor->getSelEndPos())){
        getApp()->beep();
        }

      // Select new text
      editor->setAnchorPos(beg[0]);
      editor->moveCursorAndSelect(end[0],FXText::SelectChars,true);
      }
    else{

      // Feedback failure, search box turns red
      setStatusMessage(tr("String not found!"));
      searchdialog.setSearchTextColor(FXRGB(255,128,128));
      getApp()->beep();
      }
    }

  // Restore normal message
  setStatusMessage(tr("Ready."));
  return 1;
  }


// Substitute algorithm
static FXString substitute(const FXString& original,const FXString& replace,FXint* beg,FXint* end,FXint npar){
  FXint adjbeg[10],adjend[10],i;
  for(i=0; i<npar; ++i){
    adjbeg[i]=beg[i]-beg[0];
    adjend[i]=end[i]-beg[0];
    }
  return FXRex::substitute(original,adjbeg,adjend,replace,npar);
  }


// Replace text
long TextWindow::onCmdReplace(FXObject*,FXSelector,void*){
  FXTRACE((100,"TextWindow::onCmdReplace()\n"));
  FXGIFIcon dialogicon(getApp(),searchicon_gif);
  FXReplaceDialog replacedialog(this,tr("Replace"),&dialogicon);
  FXint    beg[10],end[10],pos,finish,fm,to,code;
  FXuint   placement=PLACEMENT_OWNER;
  FXString originalvalue;
  FXString findstring;
  FXString replacestring;
  FXString replacevalue;
  FXuint   findflags;
  FXbool   found;

  // Start the search/replace
  setStatusMessage(tr("Search and replace strings in the file."));

  // Start with same flags as last time
//  replacedialog.setSearchMode(searchflags);
//  replacedialog.setSearchText(searchstring);
//  replacedialog.setReplaceText(searchstring);

  // First time, throw dialog over window
  while((code=replacedialog.execute(placement))!=FXReplaceDialog::DONE){

    // User may have moved the panel
    placement=PLACEMENT_DEFAULT;

    // Grab the search parameters
//    searchstring=replacedialog.getSearchText();
//    replacestring=replacedialog.getReplaceText();
//    searchflags=replacedialog.getSearchMode();
//    replacevalue=FXString::null;

    findstring=replacedialog.getSearchText();
    findflags=replacedialog.getSearchMode();
    replacestring=replacedialog.getReplaceText();
    replacevalue=FXString::null;

    // Search or replace one instance
    if((code==FXReplaceDialog::SEARCH) || (code==FXReplaceDialog::REPLACE)){

      // If search string matches the selection, start from the end (or begin
      // when seaching backwards) of the selection.  Otherwise proceed from the
      // cursor position.
      pos=editor->getCursorPos();
      found=matchesSelection(findstring,beg,end,findflags,10);
      if(found){
        pos=(findflags&SEARCH_BACKWARD) ? beg[0]-1 : end[0];
        }

      // Perform a search if no match yet, or we're just searching
      if(!found || (code==FXReplaceDialog::SEARCH)){
        found=editor->findText(findstring,beg,end,pos,findflags|SEARCH_WRAP,10);
        }

      // Found a match; if just searching, select the match, otherwise, select
      // the replaced text to what the match was replaced with.
      if(found){
        setStatusMessage(tr("String found!"));
        replacedialog.setSearchTextColor(FXRGB(128,255,128));
        replacedialog.setReplaceTextColor(FXRGB(128,255,128));

        // Flag a wraparound the text
        if(findflags&SEARCH_BACKWARD){
          if(pos<=beg[0]){ setStatusMessage(tr("Search wrapped around.")); }
          }
        else{
          if(beg[0]<pos){ setStatusMessage(tr("Search wrapped around.")); }
          }

        // Replace the string
        if(code==FXReplaceDialog::REPLACE){
          if(findflags&SEARCH_REGEX){
            editor->extractText(originalvalue,beg[0],end[0]-beg[0]);
            replacevalue=substitute(originalvalue,replacestring,beg,end,10);
            }
          else{
            replacevalue=replacestring;
            }
          editor->replaceText(beg[0],end[0]-beg[0],replacevalue,true);

          // Highlight last changed
          editor->setAnchorPos(beg[0]);
          editor->moveCursorAndSelect(beg[0]+replacevalue.length(),FXText::SelectChars,true);
          }

        // Just highlight it
        else{
          editor->setAnchorPos(beg[0]);
          editor->moveCursorAndSelect(end[0],FXText::SelectChars,true);
          }
        }

      // Not found
      else{
        setStatusMessage(tr("String not found!"));
        replacedialog.setSearchTextColor(FXRGB(255,128,128));
        replacedialog.setReplaceTextColor(FXRGB(255,128,128));
        getApp()->beep();
        }
      }

    // Replace multiple instances
    else{
      fm=-1;
      to=-1;

      // Replace range
      if(code==FXReplaceDialog::REPLACE_ALL){
        pos=0;
        finish=editor->getLength();
        }
      else{
        pos=editor->getSelStartPos();
        finish=editor->getSelEndPos();
        }

      // Scan through text buffer
      while(editor->findText(findstring,beg,end,pos,((findflags&~(SEARCH_WRAP|SEARCH_BACKWARD|SEARCH_FORWARD))|SEARCH_FORWARD),10) && end[0]<=finish){

        // Start buffer mutation at first occurrence
        if(fm<0){ fm=to=beg[0]; }

        // Unchanged piece is just copied over
        if(to<beg[0]){
          editor->extractText(originalvalue,to,beg[0]-to);
          replacevalue.append(originalvalue);
          }

        // For changed piece, use substitution pattern
        if(findflags&SEARCH_REGEX){
          editor->extractText(originalvalue,beg[0],end[0]-beg[0]);
          replacevalue.append(substitute(originalvalue,replacestring,beg,end,10));
          }
        else{
          replacevalue.append(replacestring);
          }

        // End of buffer mutation at end of last occurrence
        to=end[0];

        // Advance at least one character
        pos=to;
        if(beg[0]==end[0]) pos++;
        }

      // Got anything at all?
      if(0<=fm && 0<=to){
        setStatusMessage(tr("Strings replaced!"));
        replacedialog.setSearchTextColor(FXRGB(128,255,128));
        replacedialog.setReplaceTextColor(FXRGB(128,255,128));

        // Replace the text
        editor->replaceText(fm,to-fm,replacevalue,true);
        editor->moveCursor(fm+replacevalue.length(),true);
        }
      else{
        setStatusMessage(tr("String not found!"));
        replacedialog.setSearchTextColor(FXRGB(255,128,128));
        replacedialog.setReplaceTextColor(FXRGB(255,128,128));
        getApp()->beep();
        }
      }
    }

  // Restore normal message
  setStatusMessage(tr("Ready."));
  return 1;
  }


// Search selected
long TextWindow::onCmdSearchSel(FXObject*,FXSelector sel,void*){
  FXTRACE((100,"TextWindow::onCmdSearchSel()\n"));
  FXString string;
  if(getDNDData(FROM_SELECTION,utf8Type,string)){               // UTF8 string
    searchstring=string;
    searchflags=SEARCH_EXACT;
    }
  else if(getDNDData(FROM_SELECTION,utf16Type,string)){         // UTF16 string
    FXUTF16LECodec unicode;
    searchstring=unicode.mb2utf(string);
    searchflags=SEARCH_EXACT;
    }
  else if(getDNDData(FROM_SELECTION,stringType,string)){        // ASCII string
    FX88591Codec ascii;
    searchstring=ascii.mb2utf(string);
    searchflags=SEARCH_EXACT;
    }
  if(!searchstring.empty()){
    FXint selstart=editor->getSelStartPos();
    FXint selend=editor->getSelEndPos();
    FXint pos=editor->getCursorPos();
    FXint beg[10],end[10];
    if(FXSELID(sel)==ID_SEARCH_SEL_FORW){
      if(editor->isPosSelected(pos)) pos=selend;                // Start from selection end if position is selected
      searchflags=(searchflags&~SEARCH_BACKWARD)|SEARCH_FORWARD;
      }
    else{
      if(editor->isPosSelected(pos)) pos=selstart-1;            // Start from selection start-1 if position is selected
      searchflags=(searchflags&~SEARCH_FORWARD)|SEARCH_BACKWARD;
      }
    FXTRACE((100,"searchstring=\n\"\"\"\n%s\n\"\"\"\n",searchstring.text()));
    if(editor->findText(searchstring,beg,end,pos,searchflags|SEARCH_WRAP,10)){
      if(beg[0]!=selstart || end[0]!=selend){
        editor->setAnchorPos(beg[0]);
        editor->moveCursorAndSelect(end[0],FXText::SelectChars,true);
        return 1;
        }
      }
    }
  getApp()->beep();
  return 1;
  }


// Search for next occurence
long TextWindow::onCmdSearchNext(FXObject*,FXSelector sel,void*){
  FXTRACE((100,"TextWindow::onCmdSearchNext()\n"));
  if(!searchstring.empty()){
    FXint selstart=editor->getSelStartPos();
    FXint selend=editor->getSelEndPos();
    FXint pos=editor->getCursorPos();
    FXint beg[10];
    FXint end[10];
    if(FXSELID(sel)==ID_SEARCH_NXT_FORW){
      if(editor->isPosSelected(pos)) pos=selend;                // Start from selection end if position is selected
      searchflags=(searchflags&~SEARCH_BACKWARD)|SEARCH_FORWARD;
      }
    else{
      if(editor->isPosSelected(pos)) pos=selstart-1;            // Start from selection start-1 if position is selected
      searchflags=(searchflags&~SEARCH_FORWARD)|SEARCH_BACKWARD;
      }
    if(editor->findText(searchstring,beg,end,pos,searchflags|SEARCH_WRAP,10)){
      if(beg[0]!=selstart || end[0]!=selend){
        editor->setAnchorPos(beg[0]);
        editor->moveCursorAndSelect(end[0],FXText::SelectChars,true);
        return 1;
        }
      }
    }
  getApp()->beep();
  return 1;
  }

/*******************************************************************************/

// Start incremental search; show search bar if not permanently visible
void TextWindow::startISearch(){
  if(!searching){
    showsearchbar=searchbar->shown();
    if(!showsearchbar){
      searchbar->show();
      searchbar->recalc();
      }
    searchtext->setBackColor(getApp()->getBackColor());
    searchtext->setText(FXString::null);
    searchtext->setFocus();
    searchstring=FXString::null;
    searchflags=(searchflags&~SEARCH_BACKWARD)|SEARCH_FORWARD;
    isearchReplace=false;
    isearchpos=-1;
    searching=true;
    }
  }


// Finish incremental search; hide search bar if not permanently visible
void TextWindow::finishISearch(){
  if(searching){
    if(!showsearchbar){
      searchbar->hide();
      searchbar->recalc();
      }
    searchtext->setBackColor(getApp()->getBackColor());
    searchtext->setText(FXString::null);
    editor->setFocus();
    isearchReplace=false;
    isearchpos=-1;
    searching=false;
    }
  }


// Search next incremental text
FXbool TextWindow::performISearch(const FXString& text,FXuint opts,FXbool advance,FXbool notify){
  FXint beg[10],end[10],start,mode;
  FXRex rex;

  // Figure start of search
  start=editor->getCursorPos();
  if(isearchpos==-1) isearchpos=start;
  if(advance){
    if(editor->isPosSelected(start)){
      if((opts&SEARCH_BACKWARD)){
        start=editor->getSelStartPos();
        }
      else{
        start=editor->getSelEndPos();
        }
      }
    }
  else{
    start=isearchpos;
    }

  // Back off a bit for a backward search
  if((opts&SEARCH_BACKWARD) && start>0) start--;

  // Restore normal color
  searchtext->setBackColor(getApp()->getBackColor());

  // If entry is empty, clear selection and jump back to start
  if(text.empty()){
    editor->killSelection(notify);
    editor->makePositionVisible(isearchpos);
    editor->setCursorPos(isearchpos,notify);
    getApp()->beep();
    return true;
    }

  // Check syntax of regex; ignore if input not yet complete
  mode=FXRex::Syntax;
  if(!(opts&SEARCH_REGEX)) mode|=FXRex::Verbatim;
  if(opts&SEARCH_IGNORECASE) mode|=FXRex::IgnoreCase;
  if(opts&SEARCH_WORDS) mode|=FXRex::Words;
  if(rex.parse(text,mode)==FXRex::ErrOK){

    // Search text, beep if not found
    if(!editor->findText(text,beg,end,start,opts,10)){
      searchtext->setBackColor(FXRGB(255,128,128));
      getApp()->beep();
      return false;
      }

    // Matching zero-length assertion at start position; advance to next one
    if(!(opts&SEARCH_BACKWARD) && start==beg[0] && beg[0]==end[0]){
      if(!editor->findText(text,beg,end,start+1,opts,10)){
        searchtext->setBackColor(FXRGB(255,128,128));
        getApp()->beep();
        return false;
        }
      }

    // Select matching text
    if(opts&SEARCH_BACKWARD){
      editor->setAnchorPos(end[0]);
      editor->extendSelection(beg[0],FXText::SelectChars,notify);
      editor->makePositionVisible(beg[0]);
      editor->setCursorPos(beg[0],notify);
      }
    else{
      editor->setAnchorPos(beg[0]);
      editor->extendSelection(end[0],FXText::SelectChars,notify);
      editor->makePositionVisible(end[0]);
      editor->setCursorPos(end[0],notify);
      }
    }
  return true;
  }


// Incremental search text changed
long TextWindow::onChgISearchText(FXObject*,FXSelector,void*){
  searchstring=searchtext->getText();
  performISearch(searchstring,searchflags,false,true);
  addSearchHistory(searchstring,searchflags,isearchReplace);
  isearchReplace=true;
  return 1;
  }


// Incremental search text command
long TextWindow::onCmdISearchText(FXObject*,FXSelector,void*){
  searchstring=searchtext->getText();
  performISearch(searchstring,searchflags,true,true);
  isearchReplace=false;
  return 1;
  }


// Incremental search text command
long TextWindow::onKeyISearchText(FXObject*,FXSelector,void* ptr){
  switch(((FXEvent*)ptr)->code){
    case KEY_Escape:
      finishISearch();
      return 1;
    case KEY_Page_Down:
      return onCmdISearchNext(this,FXSEL(SEL_COMMAND,ID_ISEARCH_NEXT),nullptr);
    case KEY_Page_Up:
      return onCmdISearchPrev(this,FXSEL(SEL_COMMAND,ID_ISEARCH_PREV),nullptr);
    case KEY_Down:
      return onCmdISearchHistDn(this,FXSEL(SEL_COMMAND,ID_ISEARCH_HIST_DN),nullptr);
    case KEY_Up:
      return onCmdISearchHistUp(this,FXSEL(SEL_COMMAND,ID_ISEARCH_HIST_UP),nullptr);
    case KEY_i:
      if(!(((FXEvent*)ptr)->state&CONTROLMASK)) return 0;
      return onCmdISearchCase(this,FXSEL(SEL_COMMAND,ID_ISEARCH_IGNCASE),nullptr);
    case KEY_e:
      if(!(((FXEvent*)ptr)->state&CONTROLMASK)) return 0;
      return onCmdISearchRegex(this,FXSEL(SEL_COMMAND,ID_ISEARCH_REGEX),nullptr);
    case KEY_w:
      if(!(((FXEvent*)ptr)->state&CONTROLMASK)) return 0;
      return onCmdISearchWords(this,FXSEL(SEL_COMMAND,ID_ISEARCH_WORDS),nullptr);
    case KEY_d:
      if(!(((FXEvent*)ptr)->state&CONTROLMASK)) return 0;
      return onCmdISearchDir(this,FXSEL(SEL_COMMAND,ID_ISEARCH_REVERSE),nullptr);
    }
  return 0;
  }


// Append entry
void TextWindow::addSearchHistory(const FXString& pat,FXuint opt,FXbool rep){
  if(!pat.empty()){
    if(!rep && isearchString[0]!=pat){
      for(FXint i=19; i>0; --i){
        swap(isearchString[i],isearchString[i-1]);
        swap(isearchOption[i],isearchOption[i-1]);
        }
      }
    isearchString[0]=pat;
    isearchOption[0]=opt;
    isearchIndex=-1;
    }
  }


// Search history section key
const FXchar sectionKey[]="ISearch";

// Search history search string keys
static const FXchar skey[20][3]={
  "SA","SB","SC","SD","SE","SF","SG","SH","SI","SJ",
  "SK","SL","SM","SN","SO","SP","SQ","SR","SS","ST"
  };

// Search history mode keys
static const FXchar mkey[20][3]={
  "MA","MB","MC","MD","ME","MF","MG","MH","MI","MJ",
  "MK","ML","MM","MN","MO","MP","MQ","MR","MS","MT"
  };


// Load incremental search history
void TextWindow::loadSearchHistory(){
  for(FXint i=0; i<20; ++i){
    isearchString[i]=getApp()->reg().readStringEntry(sectionKey,skey[i],FXString::null);
    isearchOption[i]=getApp()->reg().readUIntEntry(sectionKey,mkey[i],SEARCH_EXACT|SEARCH_FORWARD|SEARCH_WRAP);
    }
  isearchIndex=-1;
  }


// Save incremental search history
void TextWindow::saveSearchHistory(){
  for(FXint i=0; i<20; ++i){
    getApp()->reg().writeStringEntry(sectionKey,skey[i],isearchString[i].text());
    getApp()->reg().writeUIntEntry(sectionKey,mkey[i],isearchOption[i]);
    }
  }


// Scroll back in incremental search history
long TextWindow::onCmdISearchHistUp(FXObject*,FXSelector,void*){
  if(isearchIndex+1<20 && !isearchString[isearchIndex+1].empty()){
    isearchIndex++;
    FXASSERT(0<=isearchIndex && isearchIndex<20);
    searchstring=isearchString[isearchIndex];
    searchtext->setText(searchstring);
    performISearch(searchstring,searchflags,false,true);
    }
  else{
    getApp()->beep();
    }
  return 1;
  }


// Scroll forward in incremental search history
long TextWindow::onCmdISearchHistDn(FXObject*,FXSelector,void*){
  if(0<isearchIndex){
    isearchIndex--;
    FXASSERT(0<=isearchIndex && isearchIndex<20);
    searchstring=isearchString[isearchIndex];
    searchtext->setText(searchstring);
    performISearch(searchstring,searchflags,false,true);
    }
  else{
    isearchIndex=-1;
    searchstring=FXString::null;
    searchtext->setText(FXString::null,true);
    }
  return 1;
  }


// Search incremental backward for next occurrence
long TextWindow::onCmdISearchPrev(FXObject*,FXSelector,void*){
  searchflags=(searchflags&~SEARCH_FORWARD)|SEARCH_BACKWARD;
  performISearch(searchstring,searchflags,true,true);
  return 1;
  }


// Search incremental forward for next occurrence
long TextWindow::onCmdISearchNext(FXObject*,FXSelector,void*){
  searchflags=(searchflags&~SEARCH_BACKWARD)|SEARCH_FORWARD;
  performISearch(searchstring,searchflags,true,true);
  return 1;
  }


// Start incremental search
long TextWindow::onCmdISearchStart(FXObject*,FXSelector,void*){
  startISearch();
  return 1;
  }


// End incremental search
long TextWindow::onCmdISearchFinish(FXObject*,FXSelector,void*){
  finishISearch();
  return 1;
  }


// Update case sensitive mode
long TextWindow::onUpdISearchCase(FXObject* sender,FXSelector,void*){
  sender->handle(this,(searchflags&SEARCH_IGNORECASE)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change case sensitive mode
long TextWindow::onCmdISearchCase(FXObject*,FXSelector,void*){
  searchflags^=SEARCH_IGNORECASE;
  performISearch(searchstring,searchflags,false,true);
  return 1;
  }


// Update whole words mode
long TextWindow::onUpdISearchWords(FXObject* sender,FXSelector,void*){
  sender->handle(this,(searchflags&SEARCH_WORDS)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change whole words mode
long TextWindow::onCmdISearchWords(FXObject*,FXSelector,void*){
  searchflags^=SEARCH_WORDS;
  performISearch(searchstring,searchflags,false,true);
  return 1;
  }


// Update search direction
long TextWindow::onUpdISearchDir(FXObject* sender,FXSelector,void*){
  sender->handle(this,(searchflags&SEARCH_BACKWARD)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change search direction
long TextWindow::onCmdISearchDir(FXObject*,FXSelector,void*){
  searchflags^=(SEARCH_FORWARD|SEARCH_BACKWARD);
  performISearch(searchstring,searchflags,false,true);
  return 1;
  }


// Update search mode
long TextWindow::onUpdISearchRegex(FXObject* sender,FXSelector,void*){
  sender->handle(this,(searchflags&SEARCH_REGEX)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change search mode
long TextWindow::onCmdISearchRegex(FXObject*,FXSelector,void*){
  searchflags^=SEARCH_REGEX;
  performISearch(searchstring,searchflags,false,true);
  return 1;
  }

/*******************************************************************************/

// Text window got focus; terminate incremental search
long TextWindow::onTextFocus(FXObject*,FXSelector,void*){
  finishISearch();
  return 1;
  }


// Text inserted; callback has [pos nins]
long TextWindow::onTextInserted(FXObject*,FXSelector,void* ptr){
  const FXTextChange *change=(const FXTextChange*)ptr;

  FXTRACE((140,"Inserted: pos=%d ndel=%d nins=%d\n",change->pos,change->ndel,change->nins));

  // Log undo record
  if(!undolist.busy()){
    undolist.add(new FXTextInsert(editor,change->pos,change->nins,change->ins));
    if(undolist.size()>MAXUNDOSIZE) undolist.trimSize(KEEPUNDOSIZE);
    }

  // Update bookmark locations
  updateBookmarks(change->pos,change->ndel,change->nins);

  // Restyle text
  restyleText(change->pos,change->ndel,change->nins);

  // Update insert point
  insertpoint=change->pos+change->nins;
  return 1;
  }


// Text replaced; callback has [pos ndel nins]
long TextWindow::onTextReplaced(FXObject*,FXSelector,void* ptr){
  const FXTextChange *change=(const FXTextChange*)ptr;

  FXTRACE((140,"Replaced: pos=%d ndel=%d nins=%d\n",change->pos,change->ndel,change->nins));

  // Log undo record
  if(!undolist.busy()){
    undolist.add(new FXTextReplace(editor,change->pos,change->ndel,change->nins,change->del,change->ins));
    if(undolist.size()>MAXUNDOSIZE) undolist.trimSize(KEEPUNDOSIZE);
    }

  // Update bookmark locations
  updateBookmarks(change->pos,change->ndel,change->nins);

  // Restyle text
  restyleText(change->pos,change->ndel,change->nins);

  // Update insert point
  insertpoint=change->pos+change->nins;

  return 1;
  }


// Text deleted; callback has [pos ndel]
long TextWindow::onTextDeleted(FXObject*,FXSelector,void* ptr){
  const FXTextChange *change=(const FXTextChange*)ptr;

  FXTRACE((140,"Deleted: pos=%d ndel=%d nins=%d\n",change->pos,change->ndel,change->nins));

  // Log undo record
  if(!undolist.busy()){
    undolist.add(new FXTextDelete(editor,change->pos,change->ndel,change->del));
    if(undolist.size()>MAXUNDOSIZE) undolist.trimSize(KEEPUNDOSIZE);
    }

  // Update bookmark locations
  updateBookmarks(change->pos,change->ndel,change->nins);

  // Restyle text
  restyleText(change->pos,change->ndel,change->nins);

  // Update insert point
  insertpoint=change->pos+change->nins;

  return 1;
  }


// Released right button in text
long TextWindow::onTextRightMouse(FXObject*,FXSelector,void* ptr){
  FXEvent* event=(FXEvent*)ptr;
  if(!event->moved){

    // Context menu
    FXMenuPane popupmenu(this,POPUP_SHRINKWRAP);
    new FXMenuCommand(&popupmenu,tr("Undo"),getApp()->undoicon,&undolist,FXUndoList::ID_UNDO);
    new FXMenuCommand(&popupmenu,tr("Redo"),getApp()->redoicon,&undolist,FXUndoList::ID_REDO);
    new FXMenuSeparator(&popupmenu);
    new FXMenuCommand(&popupmenu,tr("Find next selected\t\tSearch next occurrence of selected text."),getApp()->searchnexticon,this,ID_SEARCH_SEL_FORW);
    new FXMenuCommand(&popupmenu,tr("Find previous selected\t\tSearch previous occurrence of selected text."),getApp()->searchprevicon,this,ID_SEARCH_SEL_BACK);
    new FXMenuSeparator(&popupmenu);

    // Open selected
    new FXMenuCommand(&popupmenu,tr("Open selected...\t\tOpen selected file."),getApp()->openselicon,this,ID_OPEN_SELECTED);
    new FXMenuSeparator(&popupmenu);

    // Switch between open windows
    new FXMenuCascade(&popupmenu,tr("Switch windows\t\tSwitch to another window."),getApp()->docsicon,windowmenu);
    new FXMenuSeparator(&popupmenu);

    // Editing
    new FXMenuCommand(&popupmenu,tr("Cut"),getApp()->cuticon,editor,FXText::ID_CUT_SEL);
    new FXMenuCommand(&popupmenu,tr("Copy"),getApp()->copyicon,editor,FXText::ID_COPY_SEL);
    new FXMenuCommand(&popupmenu,tr("Paste"),getApp()->pasteicon,editor,FXText::ID_PASTE_SEL);
    new FXMenuCommand(&popupmenu,tr("Delete"),getApp()->deleteicon,editor,FXText::ID_DELETE_SEL);
    new FXMenuSeparator(&popupmenu);

    // Bookmarks
    new FXMenuCommand(&popupmenu,tr("Insert point\t\tGo back to position of last edit."),getApp()->pointicon,this,ID_INSERTPOINT);
    new FXMenuCommand(&popupmenu,tr("Set bookmark\t\tSet bookmark at cursor location."),getApp()->bookseticon,this,ID_SET_MARK);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_0);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_1);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_2);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_3);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_4);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_5);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_6);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_7);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_8);
    new FXMenuCheck(&popupmenu,FXString::null,this,ID_MARK_9);
    new FXMenuCommand(&popupmenu,tr("Delete bookmark\t\tDelete bookmark at cursor."),getApp()->bookdelicon,this,ID_DEL_MARK);
    new FXMenuCommand(&popupmenu,tr("Clear all bookmarks\t\tClear all bookmarks."),getApp()->bookclricon,this,ID_CLEAR_MARKS);

    // Run the popup
    popupmenu.forceRefresh();
    popupmenu.create();
    popupmenu.popup(nullptr,event->root_x,event->root_y);
    getApp()->runModalWhileShown(&popupmenu);
    }
  return 1;
  }


// Released right button in logger
long TextWindow::onLoggerRightMouse(FXObject*,FXSelector,void* ptr){
  FXEvent* event=(FXEvent*)ptr;
  if(!event->moved){
    FXMenuPane popupmenu(this,POPUP_SHRINKWRAP);
    new FXMenuCommand(&popupmenu,tr("Cut"),getApp()->cuticon,logger,FXText::ID_CUT_SEL);
    new FXMenuCommand(&popupmenu,tr("Copy"),getApp()->copyicon,logger,FXText::ID_COPY_SEL);
    new FXMenuCommand(&popupmenu,tr("Paste"),getApp()->pasteicon,logger,FXText::ID_PASTE_SEL);
    new FXMenuSeparator(&popupmenu);
    popupmenu.forceRefresh();
    popupmenu.create();
    popupmenu.popup(nullptr,event->root_x,event->root_y);
    getApp()->runModalWhileShown(&popupmenu);
    }
  return 1;
  }


/*******************************************************************************/


// Check file when focus moves in
long TextWindow::onFocusIn(FXObject* sender,FXSelector sel,void* ptr){
  FXMainWindow::onFocusIn(sender,sel,ptr);
  if(warnchanged){
    checkChanges();
    }
  return 1;
  }


// Update clock
long TextWindow::onClock(FXObject*,FXSelector,void*){
  FXTime current=FXThread::time();
  clock->setText(FXSystem::localTime(current,tr("%H:%M:%S")));
  clock->setTipText(FXSystem::localTime(current,tr("%A %B %d %Y")));
  getApp()->addTimeout(this,ID_CLOCKTIME,CLOCKTIMER);
  return 0;
  }


/*******************************************************************************/

// Update insert point
long TextWindow::onUpdInsertPoint(FXObject* sender,FXSelector,void*){
  sender->handle(this,(0<insertpoint)?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Jump back to insert point
long TextWindow::onCmdInsertPoint(FXObject*,FXSelector,void*){
  visitPosition(insertpoint);
  return 1;
  }


// Next bookmarked place
long TextWindow::onCmdNextMark(FXObject*,FXSelector,void*){
  if(bookmark[0]!=0){
    FXint pos=editor->lineEnd(editor->getCursorPos());
    for(FXuint b=0; b<ARRAYNUMBER(bookmark) && bookmark[b]; b++){
      if(pos<bookmark[b]){ visitPosition(bookmark[b]); break; }
      }
    }
  return 1;
  }


// Sensitize if bookmark beyond cursor pos
long TextWindow::onUpdNextMark(FXObject* sender,FXSelector,void*){
  if(bookmark[0]!=0){
    FXint pos=editor->lineEnd(editor->getCursorPos());
    for(FXuint b=0; b<ARRAYNUMBER(bookmark) && bookmark[b]; b++){
      if(pos<bookmark[b]){ sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),nullptr); return 1; }
      }
    }
  sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Previous bookmarked place
long TextWindow::onCmdPrevMark(FXObject*,FXSelector,void*){
  if(bookmark[0]!=0){
    FXint pos=editor->lineEnd(editor->getCursorPos());
    for(FXuint b=0; b<ARRAYNUMBER(bookmark) && bookmark[b]; b++){
      if(bookmark[b]<pos){ visitPosition(bookmark[b]); break; }
      }
    }
  return 1;
  }


// Sensitize if bookmark before cursor pos
long TextWindow::onUpdPrevMark(FXObject* sender,FXSelector,void*){
  if(bookmark[0]!=0){
    FXint pos=editor->lineStart(editor->getCursorPos());
    for(FXuint b=0; b<ARRAYNUMBER(bookmark) && bookmark[b]; b++){
      if(bookmark[b]<pos){ sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),nullptr); return 1; }
      }
    }
  sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Set bookmark, but don't set more than one per line
long TextWindow::onCmdSetMark(FXObject*,FXSelector,void*){
  setBookmark(editor->getCursorPos());
  return 1;
  }


// Update set bookmark
long TextWindow::onUpdSetMark(FXObject* sender,FXSelector,void*){
  sender->handle(this,(bookmark[ARRAYNUMBER(bookmark)-1]==0)?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Goto bookmark i
long TextWindow::onCmdGotoMark(FXObject*,FXSelector sel,void*){
  FXint pos=bookmark[FXSELID(sel)-ID_MARK_0];
  if(pos){
    visitPosition(pos);
    }
  return 1;
  }


// Update bookmark i
long TextWindow::onUpdGotoMark(FXObject* sender,FXSelector sel,void*){
  FXint pos=bookmark[FXSELID(sel)-ID_MARK_0];
  if(0<pos && pos<=editor->getLength()){
    FXint b=editor->lineStart(pos);     // Extent of bookmarked line
    FXint e=editor->lineEnd(pos);
    FXint p=editor->getCursorPos();
    FXint c=(b<=p&&p<=e);               // Cursor inside bookmarked line
    FXString string;
    FXASSERT(0<=b && e<=editor->getLength());
    if(b==e){                           // Empty line: just show line number
      string.format("<<%d>>",pos);
      }
    else{                               // Show squeezed text of line as label
      string=editor->extractText(b,e-b);
      string.simplify();
      if(50<=string.length()){          // If too long after squeeze, suffix "..."
        string.replace(50,string.length()-50,"...");
        }
      }
    sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&string);
    sender->handle(this,FXSEL(SEL_COMMAND,ID_SETVALUE),(void*)(FXuval)c);
    sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),nullptr);
    return 1;
    }
  sender->handle(this,FXSEL(SEL_COMMAND,ID_HIDE),nullptr);
  return 1;
  }


// Highlight if there's a bookmark on the line at pos
long TextWindow::onUpdDelMark(FXObject* sender,FXSelector,void*){
  if(bookmark[0]!=0){
    FXint pos=editor->getCursorPos();
    FXint beg=editor->lineStart(pos);
    FXint end=editor->lineEnd(pos);
    FXuint i=0;
    while(i<ARRAYNUMBER(bookmark)){
      if(beg<=bookmark[i] && bookmark[i]<=end){
        sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),nullptr);
        return 1;
        }
      i++;
      }
    }
  sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Clear bookmark at cursor
long TextWindow::onCmdDelMark(FXObject*,FXSelector,void*){
  clearBookmark(editor->getCursorPos());
  return 1;
  }



// Highlight if cursor on bookmarked line
long TextWindow::onUpdClearMarks(FXObject* sender,FXSelector,void*){
  sender->handle(this,bookmark[0]?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Clear bookmarks
long TextWindow::onCmdClearMarks(FXObject*,FXSelector,void*){
  clearBookmarks();
  return 1;
  }


// Set new bookmark after checking the line at pos doesn't
// already contain one.
void TextWindow::setBookmark(FXint pos){
  if(bookmark[ARRAYNUMBER(bookmark)-1]==0){
    FXint beg=editor->lineStart(pos);
    FXint end=editor->lineEnd(pos);
    FXuint i=0;
    while(i<ARRAYNUMBER(bookmark)){
      if(beg<=bookmark[i] && bookmark[i]<=end) return;
      i++;
      }
    while(0<i && bookmark[i-1]==0){
      i--;
      }
    while(0<i && pos<bookmark[i-1]){
      bookmark[i]=bookmark[i-1];
      i--;
      }
    FXASSERT(i<ARRAYNUMBER(bookmark));
    bookmark[i]=pos;
    }
  }


// Remove bookmark inside line at pos
void TextWindow::clearBookmark(FXint pos){
  if(bookmark[0]!=0){
    FXint beg=editor->lineStart(pos);
    FXint end=editor->lineEnd(pos);
    FXuint i=0;
    FXuint j=0;
    FXint p;
    while(j<ARRAYNUMBER(bookmark)){
      p=bookmark[j++];
      if(beg<=p && p<=end) continue;
      bookmark[i++]=p;
      }
    while(i<ARRAYNUMBER(bookmark)){
      bookmark[i++]=0;
      }
    }
  }


// Update bookmarks upon a text mutation, deleting the bookmark
// if it was inside the deleted text and moving its position otherwise
void TextWindow::updateBookmarks(FXint pos,FXint nd,FXint ni){
  if(bookmark[0]!=0){
    FXuint i=0;
    FXuint j=0;
    while(j<ARRAYNUMBER(bookmark)){
      if(bookmark[j]<=pos){ bookmark[i++]=bookmark[j++]; }
      else if(pos+nd<=bookmark[j]){ bookmark[i++]=bookmark[j++]+ni-nd; }
      else{ bookmark[j++]=0; }
      }
    }
  }


// Clear bookmarks
void TextWindow::clearBookmarks(){
  bookmark[0]=bookmark[1]=bookmark[2]=bookmark[3]=bookmark[4]=bookmark[5]=bookmark[6]=bookmark[7]=bookmark[8]=bookmark[9]=0;
  }


// Read bookmarks associated with file
void TextWindow::readBookmarks(const FXString& file){
  clearBookmarks();
  getApp()->reg().readFormatEntry("BOOKMARKS",FXPath::name(file),"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&bookmark[0],&bookmark[1],&bookmark[2],&bookmark[3],&bookmark[4],&bookmark[5],&bookmark[6],&bookmark[7],&bookmark[8],&bookmark[9]);
  }


// Write bookmarks associated with file, if any were set
void TextWindow::writeBookmarks(const FXString& file){
  if(savemarks && bookmark[0]){
    getApp()->reg().writeFormatEntry("BOOKMARKS",FXPath::name(file).text(),"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",bookmark[0],bookmark[1],bookmark[2],bookmark[3],bookmark[4],bookmark[5],bookmark[6],bookmark[7],bookmark[8],bookmark[9]);
    }
  }


// Toggle saving of bookmarks
long TextWindow::onCmdSaveMarks(FXObject*,FXSelector,void*){
  savemarks=!savemarks;
  return 1;
  }


// Update saving bookmarks
long TextWindow::onUpdSaveMarks(FXObject* sender,FXSelector,void*){
  sender->handle(this,savemarks?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Toggle saving of views
long TextWindow::onCmdSaveViews(FXObject*,FXSelector,void*){
  saveviews=!saveviews;
  return 1;
  }


// Update saving views
long TextWindow::onUpdSaveViews(FXObject* sender,FXSelector,void*){
  sender->handle(this,saveviews?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Read view of the file
void TextWindow::readView(const FXString& file){
  editor->setTopLine(getApp()->reg().readIntEntry("VIEW",FXPath::name(file),0));
  }


// Write current view of the file
void TextWindow::writeView(const FXString& file){
  if(saveviews && editor->getTopLine()){
    getApp()->reg().writeIntEntry("VIEW",FXPath::name(file),editor->getTopLine());
    }
  }


/*******************************************************************************/

// Parse modeline from the text buffer
void TextWindow::parseModeline(){
  if(modeline){
    Modeline modes;
    FXString lines;
    FXString language;

    // Buffer chunk
    FXint len=editor->getLength();
    FXint pos=editor->nextLine(0,30);

    // Extract top few lines and check for modeline
    editor->extractText(lines,0,pos);
    if(!modes.parseModeline(lines)){
      if(pos>=len) return;
      pos=editor->prevLine(len,10);

      // Extract bottom few lines and check for modeline
      editor->extractText(lines,pos,len-pos);
      if(!modes.parseModeline(lines)) return;
      }

    // See if a new langauge is called for
    language=modes.getLanguage();
    if(!language.empty()){
      Syntax* stx=getApp()->getSyntaxByName(language);
      if(stx){
        setSyntax(stx);
        }
      }

    // Get original parameters
    FXuint modebits=editor->getTextStyle();
    FXint  tabwidth=editor->getTabColumns();
    FXint  wrapwidth=editor->getWrapColumns();

    // Modes from the mode line
    if(modes.getAutoIndent()==0) modebits&=~TEXT_AUTOINDENT;
    if(modes.getAutoIndent()==1) modebits|=TEXT_AUTOINDENT;
    if(modes.getWrapMode()==0) modebits&=~(TEXT_WORDWRAP|TEXT_FIXEDWRAP);
    if(modes.getWrapMode()==1) modebits|=(TEXT_WORDWRAP|TEXT_FIXEDWRAP);
    if(modes.getTabMode()==0) modebits&=~TEXT_NO_TABS;
    if(modes.getTabMode()==1) modebits|=TEXT_NO_TABS;

    // Tab and wrap widths
    if(modes.getTabWidth()>0) tabwidth=modes.getTabWidth();
    if(modes.getWrapWidth()>0) wrapwidth=modes.getWrapWidth();

    // Put back (modified) parameters
    editor->setTextStyle(modebits);
    editor->setTabColumns(tabwidth);
    editor->setWrapColumns(wrapwidth);
    }
  }

/*******************************************************************************/

// Determine language
// If modeline support is turned on, parse chunk of the file to get the modeline-
// settable parameters.  If modeline is turned on and syntax mapping not forced by
// user, use the one provided by the modeline.  Override editor settings with those
// from modeline regardless of how language-mode was resolved.
void TextWindow::determineSyntax(){
  FXString file(FXPath::name(getFilename()));

  // This filename was to be directly associated with a particular syntax
  // Ignore syntax settings in the modeline, user has explicitly set it
  Syntax* stx=getApp()->getSyntaxByRegistry(file);
  if(!stx){

    // Use the syntax whose wildcards match the given filename
    stx=getApp()->getSyntaxByPattern(file);
    if(!stx){
      FXString lines;

      // Grab up to here
      FXint pos=editor->nextLine(0,30);

      // Grab some lines of the top of the file and see if it contains
      // anything one of the syntaxes would match against
      editor->extractText(lines,0,pos);

      // Use the syntax matching some regex patterns inside the file
      stx=getApp()->getSyntaxByContents(lines);
      }
    }

  // Change the syntax coloring scheme
  setSyntax(stx);
  }


// Switch syntax
long TextWindow::onCmdSyntaxSwitch(FXObject*,FXSelector sel,void*){
  FXint syn=FXSELID(sel)-ID_SYNTAX_FIRST;
  FXString file=FXPath::name(getFilename());
  if(0<syn){
    getApp()->reg().writeStringEntry("SYNTAX",file,getApp()->getSyntax(syn-1)->getName().text());
    setSyntax(getApp()->getSyntax(syn-1));
    }
  else{
    getApp()->reg().deleteEntry("SYNTAX",file);
    setSyntax(nullptr);
    }
  return 1;
  }


// Switch syntax
long TextWindow::onUpdSyntaxSwitch(FXObject* sender,FXSelector sel,void*){
  FXint syn=FXSELID(sel)-ID_SYNTAX_FIRST;
  FXASSERT(0<=syn && syn<=getApp()->numSyntaxes());
  Syntax *sntx=syn?getApp()->getSyntax(syn-1):nullptr;
  sender->handle(this,(sntx==syntax)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }

/*******************************************************************************/

// Change normal foreground color
long TextWindow::onCmdStyleNormalFG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_NORMAL_FG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&styles[index].normalForeColor);
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update normal foreground color
long TextWindow::onUpdStyleNormalFG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_NORMAL_FG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&styles[index].normalForeColor);
  return 1;
  }


// Change normal background color
long TextWindow::onCmdStyleNormalBG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_NORMAL_BG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&styles[index].normalBackColor);
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update normal background color
long TextWindow::onUpdStyleNormalBG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_NORMAL_BG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&styles[index].normalBackColor);
  return 1;
  }


// Change selected foreground color
long TextWindow::onCmdStyleSelectFG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_SELECT_FG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&styles[index].selectForeColor);
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update selected foreground color
long TextWindow::onUpdStyleSelectFG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_SELECT_FG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&styles[index].selectForeColor);
  return 1;
  }


// Change selected background color
long TextWindow::onCmdStyleSelectBG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_SELECT_BG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&styles[index].selectBackColor);
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update selected background color
long TextWindow::onUpdStyleSelectBG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_SELECT_BG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&styles[index].selectBackColor);
  return 1;
  }


// Change highlight foreground color
long TextWindow::onCmdStyleHiliteFG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_HILITE_FG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&styles[index].hiliteForeColor);
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update highlight foreground color
long TextWindow::onUpdStyleHiliteFG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_HILITE_FG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&styles[index].hiliteForeColor);
  return 1;
  }


// Change highlight background color
long TextWindow::onCmdStyleHiliteBG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_HILITE_BG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&styles[index].hiliteBackColor);
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update highlight background color
long TextWindow::onUpdStyleHiliteBG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_HILITE_BG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&styles[index].hiliteBackColor);
  return 1;
  }


// Change active background color
long TextWindow::onCmdStyleActiveBG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_ACTIVE_BG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_GETINTVALUE),(void*)&styles[index].activeBackColor);
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update active background color
long TextWindow::onUpdStyleActiveBG(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_ACTIVE_BG_FIRST;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&styles[index].activeBackColor);
  return 1;
  }


// Change underline style
long TextWindow::onCmdStyleUnderline(FXObject*,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_UNDERLINE_FIRST;
  styles[index].style^=FXText::STYLE_UNDERLINE;
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update underline style
long TextWindow::onUpdStyleUnderline(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_UNDERLINE_FIRST;
  sender->handle(this,(styles[index].style&FXText::STYLE_UNDERLINE)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change strikeout style
long TextWindow::onCmdStyleStrikeout(FXObject*,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_STRIKEOUT_FIRST;
  styles[index].style^=FXText::STYLE_STRIKEOUT;
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update strikeout style
long TextWindow::onUpdStyleStrikeout(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_STRIKEOUT_FIRST;
  sender->handle(this,(styles[index].style&FXText::STYLE_STRIKEOUT)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change bold style
long TextWindow::onCmdStyleBold(FXObject*,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_BOLD_FIRST;
  styles[index].style^=FXText::STYLE_BOLD;
  writeStyleForRule(syntax->getGroup(),syntax->getRule(index+1)->getName(),styles[index]);
  editor->update();
  return 1;
  }

// Update bold style
long TextWindow::onUpdStyleBold(FXObject* sender,FXSelector sel,void*){
  FXint index=FXSELID(sel)-ID_STYLE_BOLD_FIRST;
  sender->handle(this,(styles[index].style&FXText::STYLE_BOLD)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }

/*******************************************************************************/

// Set language
void TextWindow::setSyntax(Syntax* syn){
  syntax=syn;

  // Set editor attributes to syntax mode
  if(syntax){

    // Get original parameters
    FXuint modebits=editor->getTextStyle();
    FXint  tabwidth=editor->getTabColumns();
    FXint  wrapwidth=editor->getWrapColumns();

    // Set number of syntax styles
    styles.no(syntax->getNumRules()-1);

    // Read attributes for each style
    for(FXint rule=1; rule<syntax->getNumRules(); rule++){
      styles[rule-1]=readStyleForRule(syntax->getGroup(),syntax->getRule(rule)->getName(),syntax->getRule(rule)->getStyle());
      }

    // Set styles
    editor->setHiliteStyles(styles.data());
    editor->setStyled(colorize);

    // Other syntax-determined stuff
    editor->setDelimiters(syntax->getDelimiters().text());

    // Modes from the syntax
    if(syntax->getAutoIndent()==0) modebits&=~TEXT_AUTOINDENT;
    if(syntax->getAutoIndent()==1) modebits|=TEXT_AUTOINDENT;
    if(syntax->getWrapMode()==0) modebits&=~(TEXT_WORDWRAP|TEXT_FIXEDWRAP);
    if(syntax->getWrapMode()==1) modebits|=(TEXT_WORDWRAP|TEXT_FIXEDWRAP);
    if(syntax->getTabMode()==0) modebits&=~TEXT_NO_TABS;
    if(syntax->getTabMode()==1) modebits|=TEXT_NO_TABS;

    // Tab and wrap widths
    if(syntax->getTabWidth()>0) tabwidth=syntax->getTabWidth();
    if(syntax->getWrapWidth()>0) wrapwidth=syntax->getWrapWidth();

    // Put back (modified) parameters
    editor->setTextStyle(modebits);
    editor->setTabColumns(tabwidth);
    editor->setWrapColumns(wrapwidth);

    // Full recolorization is called for
    restyleText();
    }

  // Reset editor attributes to defaults
  else{
    FXuint modebits=editor->getTextStyle();
    editor->setHiliteStyles(nullptr);
    editor->setStyled(false);
    editor->setDelimiters(delimiters.text());
    if(autoindent) modebits|=TEXT_AUTOINDENT; else modebits&=~TEXT_AUTOINDENT;
    if(wrapping) modebits|=TEXT_WORDWRAP; else modebits&=~TEXT_WORDWRAP;
    if(fixedwrap) modebits|=TEXT_FIXEDWRAP; else modebits&=~TEXT_FIXEDWRAP;
    if(hardtabs) modebits&=~TEXT_NO_TABS; else modebits|=TEXT_NO_TABS;
    editor->setTextStyle(modebits);
    editor->setTabColumns(tabcols);
    editor->setWrapColumns(wrapcols);
    }
  }


// Scan forward by context amount
FXint TextWindow::forwardByContext(FXint pos) const {
  FXint nlines=syntax->getContextLines();
  FXint nchars=syntax->getContextChars();
  FXint r1=pos;
  FXint r2=pos;
  if(0<nchars){
    r1=editor->validPos(pos+nchars);
    }
  if(0<nlines){
    r2=editor->nextLine(pos,nlines);
    }
  return Math::imin(editor->getLength(),Math::imax(r1,r2));
  }


// Scan backward by context amount
FXint TextWindow::backwardByContext(FXint pos) const {
  FXint nlines=syntax->getContextLines();
  FXint nchars=syntax->getContextChars();
  FXint r1=pos;
  FXint r2=pos;
  if(0<nchars){
    r1=editor->validPos(pos-nchars);
    }
  if(0<nlines){
    r2=editor->prevLine(pos,nlines);
    }
  return Math::imax(0,Math::imin(r1,r2));
  }


// Find restyle point
FXint TextWindow::findRestylePoint(FXint pos,FXint& style) const {
  FXint probepos,prevprobepos,safepos,beforesafepos,runstyle,s;

  // Return 0 for style unless we found something else
  style=0;

  // Scan back by a certain amount of match context
  probepos=backwardByContext(pos);

  // At begin of buffer, so restyle from begin
  if(probepos==0) return 0;

  // Get style here
  runstyle=editor->getStyle(probepos);

  // Outside of colorized part, so restyle from here
  if(runstyle==0) return probepos;

  // Scan back one more context
  safepos=backwardByContext(probepos);

  // And one before that
  beforesafepos=backwardByContext(safepos);

  // Scan back for style change
  while(0<probepos){

    // Go back one character (not byte)
    prevprobepos=editor->dec(probepos);

    // Style prior to probe position
    s=editor->getStyle(prevprobepos);

    // Style change?
    if(runstyle!=s){

      // Begin of child-pattern, return parent style
      if(syntax->isAncestor(s,runstyle)){
        style=s;
        return probepos;
        }

      // End of child-pattern, return running style
      if(syntax->isAncestor(runstyle,s)){
        style=runstyle;
        return probepos;
        }

      // Return common ancestor style
      style=syntax->commonAncestor(runstyle,s);
      return probepos;
      }

    // Scan back
    probepos=prevprobepos;

    // Further back
    if(probepos<beforesafepos){
      style=runstyle;
      return safepos;
      }
    }
  return 0;
  }


// Restyle range; returns affected style end, i.e. one beyond
// the last position where the style changed from the original style
FXint TextWindow::restyleRange(FXint beg,FXint end,FXint& changed,FXint rule){
  FXint len=end-beg,pos=0,chg=0;
  FXString text;
  FXASSERT(0<=beg && beg<=end && end<=editor->getLength());
  if(text.length(len+len+len)){
    FXASSERT(0<=rule && rule<syntax->getNumRules());
    editor->extractText(&text[0],beg,len);
    editor->extractStyle(&text[len],beg,len);
    pos=chg=syntax->getRule(rule)->stylizeBody(&text[0],&text[len+len],0,len);
    while(0<chg && text[len+chg-1]==text[len+len+chg-1]){ --chg; }
    editor->changeStyle(beg,&text[len+len],chg);
    }
  changed=beg+chg;
  return beg+pos;
  }


// Restyle text after change in buffer
void TextWindow::restyleText(FXint pos,FXint del,FXint ins){
  FXTRACE((100,"restyleText(pos=%d,del=%d,ins=%d)\n",pos,del,ins));
  if(colorize && syntax){
    FXint tail,changed,affected,beg,end,len,rule,restylejump;

    // Length of text
    len=editor->getLength();

    // End of buffer modification
    changed=pos+ins;

    // Scan back to a place where the style changed, return
    // the style rule in effect at that location
    beg=findRestylePoint(pos,rule);

    // Scan forward by one context
    end=forwardByContext(changed);

    FXTRACE((110,"pos=%d del=%d ins=%d beg=%d end=%d len=%d rule=%d (%s)\n",pos,del,ins,beg,end,len,rule,syntax->getRule(rule)->getName().text()));

    FXASSERT(0<=rule && rule<syntax->getNumRules());

    // Restyle until we fully enclose the style change
    restylejump=RESTYLEJUMP;
    while(1){

      // Restyle [beg,end> using rule, return the end of matched range
      // and the range of affected characters where style was changed
      tail=restyleRange(beg,end,affected,rule);

      FXTRACE((110,"affected=%d beg=%d end=%d tail=%d, rule=%d (%s)\n",affected,beg,end,tail,rule,syntax->getRule(rule)->getName().text()));

      // Not all colored yet, continue coloring with parent rule from tail
      if(tail<end){
        beg=tail;
        end=forwardByContext(Math::imax(affected,changed));
        if(rule==0){ fxwarning("Top level patterns did not color everything.\n"); return; }
        rule=syntax->getRule(rule)->getParent();
        continue;
        }

      // Style changed in unchanged text
      if(affected>changed){
        restylejump<<=1;
	changed=affected;
    	end=changed+restylejump;
    	if(end>len) end=len;
        continue;
        }

      // Everything was recolored and style didn't change anymore
      return;
      }
    }
  }


// Restyle entire text
void TextWindow::restyleText(){
  if(colorize && syntax){
    FXint len=editor->getLength();
    FXString text;
    if(text.length(len+len)){
      editor->extractText(&text[0],0,len);
      syntax->getRule(0)->stylize(&text[0],&text[len],0,len);
      editor->changeStyle(0,&text[len],len);
      }
    }
  }


// Read style
FXHiliteStyle TextWindow::readStyleForRule(const FXString& group,const FXString& name,const FXString& style){
  FXchar nfg[100],nbg[100],sfg[100],sbg[100],hfg[100],hbg[100],abg[100]; FXint sty;
  FXHiliteStyle result={0,0,0,0,0,0,0,0};
  result.normalForeColor=colorFromName(style);  // Fallback color
  if(getApp()->reg().readFormatEntry(group,name,"%99[^,],%99[^,],%99[^,],%99[^,],%99[^,],%99[^,],%99[^,],%d",nfg,nbg,sfg,sbg,hfg,hbg,abg,&sty)==8){
    result.normalForeColor=colorFromName(nfg);
    result.normalBackColor=colorFromName(nbg);
    result.selectForeColor=colorFromName(sfg);
    result.selectBackColor=colorFromName(sbg);
    result.hiliteForeColor=colorFromName(hfg);
    result.hiliteBackColor=colorFromName(hbg);
    result.activeBackColor=colorFromName(abg);
    result.style=sty;
    }
  return result;
  }


// Write style
void TextWindow::writeStyleForRule(const FXString& group,const FXString& name,const FXHiliteStyle& style){
  FXchar nfg[100],nbg[100],sfg[100],sbg[100],hfg[100],hbg[100],abg[100];
  nameFromColor(nfg,style.normalForeColor);
  nameFromColor(nbg,style.normalBackColor);
  nameFromColor(sfg,style.selectForeColor);
  nameFromColor(sbg,style.selectBackColor);
  nameFromColor(hfg,style.hiliteForeColor);
  nameFromColor(hbg,style.hiliteBackColor);
  nameFromColor(abg,style.activeBackColor);
  getApp()->reg().writeFormatEntry(group,name,"%s,%s,%s,%s,%s,%s,%s,%d",nfg,nbg,sfg,sbg,hfg,hbg,abg,style.style);
  }


// Toggle syntax coloring
long TextWindow::onCmdSyntax(FXObject*,FXSelector,void* ptr){
  colorize=!!ptr;
  if(syntax && colorize){
    editor->setStyled(true);
    restyleText();
    }
  else{
    editor->setStyled(false);
    }
  return 1;
  }


// Update syntax coloring
long TextWindow::onUpdSyntax(FXObject* sender,FXSelector,void*){
  sender->handle(this,colorize?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Restyle text
long TextWindow::onCmdRestyle(FXObject*,FXSelector,void*){
  restyleText();
  return 1;
  }


// Update restyle text
long TextWindow::onUpdRestyle(FXObject* sender,FXSelector,void*){
  sender->handle(this,editor->isStyled()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Show syntax in tooltip
long TextWindow::onQueryTextTip(FXObject* sender,FXSelector,void*){
  if(getSyntax() && editor->isStyled()){
    FXint vx=editor->getVisibleX();
    FXint vy=editor->getVisibleY();
    FXint vw=editor->getVisibleWidth();
    FXint vh=editor->getVisibleHeight();
    FXint pos,x,y,s;
    FXuint btns;
    if(editor->getCursorPosition(x,y,btns)){
      if(vx<=x && vy<=y && x<vx+vw && y<vy+vh){
        pos=editor->getPosAt(x,y);
        s=editor->getStyle(pos);
        if(0<s && s<getSyntax()->getNumRules()){
          FXString tipstring=getSyntax()->getRule(s)->getName();
          sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&tipstring);
          return 1;
          }
        }
      }
    }
  return 0;
  }
