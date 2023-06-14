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
#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H


class Adie;
class Syntax;
class HelpWindow;
class Preferences;
class ShellCommand;

// Array of styles
typedef FXArray<FXHiliteStyle> FXHiliteArray;


// Editor main window
class TextWindow : public FXMainWindow {
  FXDECLARE(TextWindow)
protected:
  FXToolBarShell      *dragshell1;              // Shell for floating menubar
  FXToolBarShell      *dragshell2;              // Shell for floating toolbar
  FXToolBarShell      *dragshell3;              // Shell for floating search bar
  FXDockSite          *topdock;                 // Topmost dock area
  FXDockSite          *bottomdock;              // Bottom dock area
  FXDockSite          *leftdock;                // Left dock area
  FXDockSite          *rightdock;               // Bottom dock area
  FXMenuPane          *filemenu;                // File menu
  FXMenuPane          *editmenu;                // Edit menu
  FXMenuPane          *gotomenu;                // Goto menu
  FXMenuPane          *searchmenu;              // Search menu
  FXMenuPane          *shellmenu;               // Shell menu
  FXMenuPane          *optionmenu;              // Option menu
  FXMenuPane          *viewmenu;                // View menu
  FXMenuPane          *windowmenu;              // Window menu
  FXMenuPane          *helpmenu;                // Help menu
  FXMenuPane          *syntaxmenu;              // Syntax menu
  FXMenuPane          *tabsmenu;                // Tab selection menu
  FXVerticalFrame     *treebox;                 // Tree box containing directories/files
  FXHorizontalFrame   *undoredoblock;           // Undo/redo block on status line
  FXHorizontalFrame   *editorframe;             // Editor frame
  FXHorizontalFrame   *loggerframe;             // Logger frame
  FXText              *editor;                  // Editor text widget
  FXText              *logger;                  // Logger text widget
  FXDirList           *dirlist;                 // Directory view
  FXComboBox          *filter;                  // Combobox for pattern list
  FXLabel             *clock;                   // Time
  FXMenuBar           *menubar;                 // Menu bar
  FXToolBar           *toolbar;                 // Tool bar
  FXToolBar           *searchbar;               // Incremental search bar
  FXTextField         *searchtext;              // Incremental search text field
  FXStatusBar         *statusbar;               // Status bar
  FXFont              *font;                    // Text window font
  FXint                bookmark[10];            // Book marks
  FXint                insertpoint;             // Insert point
  Syntax              *syntax;                  // Syntax highlighter
  FXUndoList           undolist;                // Undo list
  FXRecentFiles        mrufiles;                // Recent files list
  FXString             filename;                // File being edited
  FXTime               filetime;                // Original modtime of file
  FXbool               filenameset;             // Filename is set
  FXString             delimiters;              // Text delimiters
  FXString             searchpaths;             // Search paths for files
  FXHiliteArray        styles;                  // Highlight styles
  ShellCommand        *shellCommand;            // Running shell command, if any
  FXTextSelection      replace;                 // Text region to be replaced
  FXint                replaceStart;            // Start of text to be replaced
  FXint                replaceEnd;              // End of text to be replaced
  FXint                initialwidth;            // Initial width
  FXint                initialheight;           // Initial height
  FXbool               initialsize;             // New window is initialwidth x initialheight
  FXString             isearchString[20];       // Incremental search strings
  FXuint               isearchOption[20];       // Incremental search options
  FXbool               isearchReplace;          // Replace new entry
  FXint                isearchIndex;            // Incremental search index
  FXint                isearchpos;              // Incremental search position
  FXString             searchstring;            // String of last search
  FXuint               searchflags;             // Incremental search flags
  FXbool               searching;               // Incremental search in effect
  FXbool               showsearchbar;           // Showing incremental search bar
  FXbool               modeline;                // Set modes from modeline
  FXbool               autoindent;              // Autoindent mode
  FXint                wrapcols;                // Wrap lines at this column if fixed wrap
  FXbool               wrapping;                // Wrapping lines
  FXbool               fixedwrap;               // Wrap lines at fixed column
  FXint                tabcols;                 // Tab indents this many columns
  FXbool               hardtabs;                // Insert hard tabs
  FXint                barcols;                 // Number of columns for line numbers
  FXbool               showlogger;              // Showing error logger
  FXbool               colorize;                // Syntax coloring on if possible
  FXbool               stripcr;                 // Strip carriage returns
  FXbool               stripsp;                 // Strip trailing spaces
  FXbool               appendcr;                // Append carriage returns
  FXbool               appendnl;                // Append missing newline at end of text
  FXbool               saveviews;               // Save views of files
  FXbool               savemarks;               // Save bookmarks of files
  FXbool               warnchanged;             // Warn if changed by other program
protected:

  // Undo management
  enum{
    MAXUNDOSIZE=1000000,        // Don't let the undo buffer get out of hand
    KEEPUNDOSIZE=500000         // When MAXUNDOSIZE was exceeded, trim down to this size
    };

  // Buffer I/O conversions
  enum{
    CRLF=1,                     // CRLF versus LF
    LINE=2,                     // Append end of line
    TRIM=4                      // trim trailing space
    };

  // Command Input options
  enum{
    FROM_SEL=1,                 // Input from selection
    FROM_DOC=2,                 // Input from document
    FROM_ANY=3,                 // Input from document or selection
    };

  // Command Output options
  enum{
    TO_INS=1,                   // Output inserted at cursor
    TO_REP=2,                   // Output replaces input
    TO_LOG=3,                   // Output to log window
    TO_NEW=4                    // Output to new document window
    };

  // Command Flags
  enum{
    SAVE_DOC=1,                 // Save document before
    LOAD_DOC=2,                 // Load back after
    };

private:
  TextWindow(){}
  TextWindow(const TextWindow&);
  TextWindow& operator=(const TextWindow&);
  void createMenubar();
  void createToolbar();
  void createSearchbar();
  void createStatusbar();
  void readRegistry();
  void writeRegistry();
  FXint forwardByContext(FXint pos) const;
  FXint backwardByContext(FXint pos) const;
  FXint findRestylePoint(FXint pos,FXint& style) const;
  void restyleText();
  void restyleText(FXint pos,FXint del,FXint ins);
  FXint restyleRange(FXint beg,FXint end,FXint& changed,FXint rule);
  FXHiliteStyle readStyleForRule(const FXString& group,const FXString& name,const FXString& style);
  void writeStyleForRule(const FXString& group,const FXString& name,const FXHiliteStyle& style);
  void loadSearchHistory();
  void saveSearchHistory();
  void addSearchHistory(const FXString& pat,FXuint opt,FXbool rep);
  void updateBookmarks(FXint pos,FXint nd,FXint ni);
  FXbool matchesSelection(const FXString& string,FXint* beg,FXint* end,FXuint flgs,FXint npar) const;
  FXint changedExternally();
  FXbool newDoc();
  FXbool createDoc();
  FXbool createDoc(const FXString& file);
  FXbool saveDoc();
  FXbool saveDocAs();
  FXbool saveDocTo();
  FXbool reloadDoc();
  FXbool openDoc();
  FXbool openDoc(const FXString& file);
  FXbool switchDoc();
  FXbool switchDoc(const FXString& file);
  FXbool openSelDoc();
  FXbool insertSel();
  FXbool extractSel();
private:
  static FXbool loadBuffer(const FXString& file,FXString& buffer,FXuint bits=0);
  static FXbool saveBuffer(const FXString& file,FXString& buffer,FXuint bits=0);
public:
  long onUpdate(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onUpdIsEditable(FXObject*,FXSelector,void*);
  long onUpdHasSelection(FXObject*,FXSelector,void*);

  // File management
  long onCmdNew(FXObject*,FXSelector,void*);
  long onCmdNewFile(FXObject*,FXSelector,void*);
  long onCmdOpen(FXObject*,FXSelector,void*);
  long onCmdOpenRecent(FXObject*,FXSelector,void*);
  long onCmdOpenTree(FXObject*,FXSelector,void*);
  long onCmdOpenSelected(FXObject*,FXSelector,void*);
  long onCmdSwitch(FXObject*,FXSelector,void*);
  long onCmdReload(FXObject*,FXSelector,void*);
  long onUpdReload(FXObject*,FXSelector,void*);
  long onCmdSave(FXObject*,FXSelector,void*);
  long onUpdSave(FXObject*,FXSelector,void*);
  long onCmdSaveAs(FXObject*,FXSelector,void*);
  long onCmdSaveTo(FXObject*,FXSelector,void*);
  long onCmdInsertFile(FXObject*,FXSelector,void*);
  long onCmdExtractFile(FXObject*,FXSelector,void*);

  // Text display
  long onCmdTextBackColor(FXObject*,FXSelector,void*);
  long onUpdTextBackColor(FXObject*,FXSelector,void*);
  long onCmdTextForeColor(FXObject*,FXSelector,void*);
  long onUpdTextForeColor(FXObject*,FXSelector,void*);
  long onCmdTextSelBackColor(FXObject*,FXSelector,void*);
  long onUpdTextSelBackColor(FXObject*,FXSelector,void*);
  long onCmdTextSelForeColor(FXObject*,FXSelector,void*);
  long onUpdTextSelForeColor(FXObject*,FXSelector,void*);
  long onCmdTextHiliteBackColor(FXObject*,FXSelector,void*);
  long onUpdTextHiliteBackColor(FXObject*,FXSelector,void*);
  long onCmdTextHiliteForeColor(FXObject*,FXSelector,void*);
  long onUpdTextHiliteForeColor(FXObject*,FXSelector,void*);
  long onCmdTextCursorColor(FXObject*,FXSelector,void*);
  long onUpdTextCursorColor(FXObject*,FXSelector,void*);
  long onCmdTextActBackColor(FXObject*,FXSelector,void*);
  long onUpdTextActBackColor(FXObject*,FXSelector,void*);
  long onCmdTextBarColor(FXObject*,FXSelector,void*);
  long onUpdTextBarColor(FXObject*,FXSelector,void*);
  long onCmdTextNumberColor(FXObject*,FXSelector,void*);
  long onUpdTextNumberColor(FXObject*,FXSelector,void*);
  long onCmdDirBackColor(FXObject*,FXSelector,void*);
  long onUpdDirBackColor(FXObject*,FXSelector,void*);
  long onCmdDirForeColor(FXObject*,FXSelector,void*);
  long onUpdDirForeColor(FXObject*,FXSelector,void*);
  long onCmdDirSelBackColor(FXObject*,FXSelector,void*);
  long onUpdDirSelBackColor(FXObject*,FXSelector,void*);
  long onCmdDirSelForeColor(FXObject*,FXSelector,void*);
  long onUpdDirSelForeColor(FXObject*,FXSelector,void*);
  long onCmdDirLineColor(FXObject*,FXSelector,void*);
  long onUpdDirLineColor(FXObject*,FXSelector,void*);
  long onCmdShowActive(FXObject*,FXSelector,void*);
  long onUpdShowActive(FXObject*,FXSelector,void*);
  long onCmdLineNumbers(FXObject*,FXSelector,void*);
  long onUpdLineNumbers(FXObject*,FXSelector,void*);
  long onCmdTabColumns(FXObject*,FXSelector,void*);
  long onUpdTabColumns(FXObject*,FXSelector,void*);
  long onCmdTabSelect(FXObject*,FXSelector,void*);
  long onUpdTabSelect(FXObject*,FXSelector,void*);
  long onCmdBraceMatch(FXObject*,FXSelector,void*);
  long onUpdBraceMatch(FXObject*,FXSelector,void*);
  long onCmdBraceMatchTime(FXObject*,FXSelector,void*);
  long onUpdBraceMatchTime(FXObject*,FXSelector,void*);
  long onCmdBraceMatchStay(FXObject*,FXSelector,void*);
  long onUpdBraceMatchStay(FXObject*,FXSelector,void*);
  long onCmdModeline(FXObject*,FXSelector,void*);
  long onUpdModeline(FXObject*,FXSelector,void*);
  long onCmdAutoIndent(FXObject*,FXSelector,void*);
  long onUpdAutoIndent(FXObject*,FXSelector,void*);
  long onCmdInsertTabs(FXObject*,FXSelector,void*);
  long onUpdInsertTabs(FXObject*,FXSelector,void*);
  long onCmdWrapColumns(FXObject*,FXSelector,void*);
  long onUpdWrapColumns(FXObject*,FXSelector,void*);
  long onCmdWrapFixed(FXObject*,FXSelector,void*);
  long onUpdWrapFixed(FXObject*,FXSelector,void*);
  long onCmdWrap(FXObject*,FXSelector,void*);
  long onUpdWrap(FXObject*,FXSelector,void*);

  // Text changes
  long onTextFocus(FXObject*,FXSelector,void*);
  long onTextInserted(FXObject*,FXSelector,void*);
  long onTextReplaced(FXObject*,FXSelector,void*);
  long onTextDeleted(FXObject*,FXSelector,void*);
  long onTextDNDDrop(FXObject*,FXSelector,void*);
  long onTextDNDMotion(FXObject*,FXSelector,void*);
  long onTextRightMouse(FXObject*,FXSelector,void*);

  // Miscellaneous
  long onCmdAbout(FXObject*,FXSelector,void*);
  long onCmdHelp(FXObject*,FXSelector,void*);
  long onCmdFont(FXObject*,FXSelector,void*);
  long onCmdPrint(FXObject*,FXSelector,void*);
  long onCmdSaveSettings(FXObject*,FXSelector,void*);
  long onCmdStripReturns(FXObject*,FXSelector,void*);
  long onUpdStripReturns(FXObject*,FXSelector,void*);
  long onCmdStripSpaces(FXObject*,FXSelector,void*);
  long onUpdStripSpaces(FXObject*,FXSelector,void*);
  long onCmdAppendNewline(FXObject*,FXSelector,void*);
  long onUpdAppendNewline(FXObject*,FXSelector,void*);
  long onCmdAppendCarriageReturn(FXObject*,FXSelector,void*);
  long onUpdAppendCarriageReturn(FXObject*,FXSelector,void*);
  long onCmdFilter(FXObject*,FXSelector,void*);
  long onUpdOverstrike(FXObject*,FXSelector,void*);
  long onUpdReadOnly(FXObject*,FXSelector,void*);
  long onUpdTabMode(FXObject*,FXSelector,void*);
  long onClock(FXObject*,FXSelector,void*);
  long onCmdPreferences(FXObject*,FXSelector,void*);
  long onCmdDelimiters(FXObject*,FXSelector,void*);
  long onUpdDelimiters(FXObject*,FXSelector,void*);
  long onCmdWheelAdjust(FXObject*,FXSelector,void*);
  long onUpdWheelAdjust(FXObject*,FXSelector,void*);
  long onCmdInsertPoint(FXObject*,FXSelector,void*);
  long onUpdInsertPoint(FXObject*,FXSelector,void*);
  long onCmdSetMark(FXObject*,FXSelector,void*);
  long onUpdSetMark(FXObject*,FXSelector,void*);
  long onCmdNextMark(FXObject*,FXSelector,void*);
  long onUpdNextMark(FXObject*,FXSelector,void*);
  long onCmdPrevMark(FXObject*,FXSelector,void*);
  long onUpdPrevMark(FXObject*,FXSelector,void*);
  long onCmdGotoMark(FXObject*,FXSelector,void*);
  long onUpdGotoMark(FXObject*,FXSelector,void*);
  long onCmdDelMark(FXObject*,FXSelector,void*);
  long onUpdDelMark(FXObject*,FXSelector,void*);
  long onUpdClearMarks(FXObject*,FXSelector,void*);
  long onCmdClearMarks(FXObject*,FXSelector,void*);
  long onCmdSaveMarks(FXObject*,FXSelector,void*);
  long onUpdSaveMarks(FXObject*,FXSelector,void*);
  long onCmdSaveViews(FXObject*,FXSelector,void*);
  long onUpdSaveViews(FXObject*,FXSelector,void*);
  long onCmdWarnChanged(FXObject*,FXSelector,void*);
  long onUpdWarnChanged(FXObject*,FXSelector,void*);
  long onCmdSyntax(FXObject*,FXSelector,void*);
  long onUpdSyntax(FXObject*,FXSelector,void*);
  long onCmdRestyle(FXObject*,FXSelector,void*);
  long onUpdRestyle(FXObject*,FXSelector,void*);
  long onCmdWindow(FXObject*,FXSelector,void*);
  long onUpdWindow(FXObject*,FXSelector,void*);
  long onCmdSyntaxSwitch(FXObject*,FXSelector,void*);
  long onUpdSyntaxSwitch(FXObject*,FXSelector,void*);
  long onCmdUseInitialSize(FXObject*,FXSelector,void*);
  long onUpdUseInitialSize(FXObject*,FXSelector,void*);
  long onCmdSetInitialSize(FXObject*,FXSelector,void*);
  long onCmdToggleBrowser(FXObject*,FXSelector,void*);
  long onUpdToggleBrowser(FXObject*,FXSelector,void*);
  long onCmdToggleHidden(FXObject*,FXSelector,void*);
  long onUpdToggleHidden(FXObject*,FXSelector,void*);
  long onCmdSearchPaths(FXObject*,FXSelector,void*);
  long onUpdSearchPaths(FXObject*,FXSelector,void*);
  long onCmdFindInFiles(FXObject*,FXSelector,void*);
  long onQueryTextTip(FXObject*,FXSelector,void*);
  long onLoggerRightMouse(FXObject*,FXSelector,void*);

  // Shell commands
  long onCmdShellDialog(FXObject*,FXSelector,void*);
  long onUpdShellDialog(FXObject*,FXSelector,void*);
  long onCmdShellCommand(FXObject*,FXSelector,void*);
  long onUpdShellCommand(FXObject*,FXSelector,void*);
  long onCmdShellFilter(FXObject*,FXSelector,void*);
  long onUpdShellFilter(FXObject*,FXSelector,void*);
  long onCmdShellCancel(FXObject*,FXSelector,void*);
  long onUpdShellCancel(FXObject*,FXSelector,void*);
  long onCmdShellOutput(FXObject*,FXSelector,void*);
  long onCmdShellError(FXObject*,FXSelector,void*);
  long onCmdShellDone(FXObject*,FXSelector,void*);
  long onCmdShellMenu(FXObject*,FXSelector,void*);
  long onUpdShellMenu(FXObject*,FXSelector,void*);

  // Evaluate expression
  long onCmdExpression(FXObject*,FXSelector,void*);
  long onUpdExpression(FXObject*,FXSelector,void*);

  // URL encode/decode
  long onCmdURLEncode(FXObject*,FXSelector,void*);
  long onCmdURLDecode(FXObject*,FXSelector,void*);
  long onUpdURLCoding(FXObject*,FXSelector,void*);

  // Jumping around
  long onCmdGotoLine(FXObject*,FXSelector,void*);
  long onCmdGotoSelected(FXObject*,FXSelector,void*);

  // Search
  long onCmdSearch(FXObject*,FXSelector,void*);
  long onCmdReplace(FXObject*,FXSelector,void*);
  long onCmdSearchSel(FXObject*,FXSelector,void*);
  long onCmdSearchNext(FXObject*,FXSelector,void*);

  // Incremental search
  long onChgISearchText(FXObject*,FXSelector,void*);
  long onCmdISearchText(FXObject*,FXSelector,void*);
  long onKeyISearchText(FXObject*,FXSelector,void*);
  long onCmdISearchPrev(FXObject*,FXSelector,void*);
  long onCmdISearchNext(FXObject*,FXSelector,void*);
  long onCmdISearchStart(FXObject*,FXSelector,void*);
  long onCmdISearchFinish(FXObject*,FXSelector,void*);
  long onCmdISearchHistUp(FXObject*,FXSelector,void*);
  long onCmdISearchHistDn(FXObject*,FXSelector,void*);
  long onUpdISearchCase(FXObject*,FXSelector,void*);
  long onCmdISearchCase(FXObject*,FXSelector,void*);
  long onUpdISearchWords(FXObject*,FXSelector,void*);
  long onCmdISearchWords(FXObject*,FXSelector,void*);
  long onUpdISearchDir(FXObject*,FXSelector,void*);
  long onCmdISearchDir(FXObject*,FXSelector,void*);
  long onUpdISearchRegex(FXObject*,FXSelector,void*);
  long onCmdISearchRegex(FXObject*,FXSelector,void*);

  // Style changes
  long onCmdStyleNormalFG(FXObject*,FXSelector,void*);
  long onUpdStyleNormalFG(FXObject*,FXSelector,void*);
  long onCmdStyleNormalBG(FXObject*,FXSelector,void*);
  long onUpdStyleNormalBG(FXObject*,FXSelector,void*);
  long onCmdStyleSelectFG(FXObject*,FXSelector,void*);
  long onUpdStyleSelectFG(FXObject*,FXSelector,void*);
  long onCmdStyleSelectBG(FXObject*,FXSelector,void*);
  long onUpdStyleSelectBG(FXObject*,FXSelector,void*);
  long onCmdStyleHiliteFG(FXObject*,FXSelector,void*);
  long onUpdStyleHiliteFG(FXObject*,FXSelector,void*);
  long onCmdStyleHiliteBG(FXObject*,FXSelector,void*);
  long onUpdStyleHiliteBG(FXObject*,FXSelector,void*);
  long onCmdStyleActiveBG(FXObject*,FXSelector,void*);
  long onUpdStyleActiveBG(FXObject*,FXSelector,void*);
  long onCmdStyleUnderline(FXObject*,FXSelector,void*);
  long onUpdStyleUnderline(FXObject*,FXSelector,void*);
  long onCmdStyleStrikeout(FXObject*,FXSelector,void*);
  long onUpdStyleStrikeout(FXObject*,FXSelector,void*);
  long onCmdStyleBold(FXObject*,FXSelector,void*);
  long onUpdStyleBold(FXObject*,FXSelector,void*);
public:
  enum{
    ID_ABOUT=FXMainWindow::ID_LAST,
    ID_FILEFILTER,
    ID_NEW,
    ID_NEW_FILE,
    ID_OPEN,
    ID_OPEN_TREE,
    ID_OPEN_SELECTED,
    ID_OPEN_RECENT,
    ID_SWITCH,
    ID_RELOAD,
    ID_SAVE,
    ID_SAVEAS,
    ID_SAVETO,
    ID_FONT,
    ID_HELP,
    ID_WINDOW,
    ID_PRINT,
    ID_TEXT_BACK,
    ID_TEXT_FORE,
    ID_TEXT_SELBACK,
    ID_TEXT_SELFORE,
    ID_TEXT_HILITEBACK,
    ID_TEXT_HILITEFORE,
    ID_TEXT_ACTIVEBACK,
    ID_TEXT_CURSOR,
    ID_TEXT_NUMBACK,
    ID_TEXT_NUMFORE,
    ID_TEXT_LINENUMS,
    ID_DIR_BACK,
    ID_DIR_FORE,
    ID_DIR_SELBACK,
    ID_DIR_SELFORE,
    ID_DIR_LINES,
    ID_TOGGLE_WRAP,
    ID_FIXED_WRAP,
    ID_SAVE_SETTINGS,
    ID_TEXT,
    ID_LOGGER,
    ID_STRIP_CR,
    ID_STRIP_SP,
    ID_APPEND_NL,
    ID_APPEND_CR,
    ID_OVERSTRIKE,
    ID_READONLY,
    ID_TABMODE,
    ID_CLOCKTIME,
    ID_PREFERENCES,
    ID_TABCOLUMNS,
    ID_WRAPCOLUMNS,
    ID_DELIMITERS,
    ID_INSERTTABS,
    ID_AUTOINDENT,
    ID_MODELINE,
    ID_BRACEMATCH,
    ID_BRACEMATCHTIME,
    ID_BRACEMATCHSTAY,
    ID_INSERT_FILE,
    ID_EXTRACT_FILE,
    ID_WHEELADJUST,
    ID_INSERTPOINT,
    ID_SET_MARK,
    ID_NEXT_MARK,
    ID_PREV_MARK,
    ID_MARK_0,
    ID_MARK_1,
    ID_MARK_2,
    ID_MARK_3,
    ID_MARK_4,
    ID_MARK_5,
    ID_MARK_6,
    ID_MARK_7,
    ID_MARK_8,
    ID_MARK_9,
    ID_DEL_MARK,
    ID_CLEAR_MARKS,
    ID_SAVEMARKS,
    ID_SAVEVIEWS,
    ID_SHOWACTIVE,
    ID_WARNCHANGED,
    ID_SYNTAX,
    ID_RESTYLE,
    ID_WINDOW_1,
    ID_WINDOW_2,
    ID_WINDOW_3,
    ID_WINDOW_4,
    ID_WINDOW_5,
    ID_WINDOW_6,
    ID_WINDOW_7,
    ID_WINDOW_8,
    ID_WINDOW_9,
    ID_WINDOW_10,
    ID_WINDOW_11,
    ID_WINDOW_12,
    ID_WINDOW_13,
    ID_WINDOW_14,
    ID_WINDOW_15,
    ID_SYNTAX_FIRST,
    ID_SYNTAX_LAST=ID_SYNTAX_FIRST+100,
    ID_STYLE_NORMAL_FG_FIRST,
    ID_STYLE_NORMAL_FG_LAST=ID_STYLE_NORMAL_FG_FIRST+100,
    ID_STYLE_NORMAL_BG_FIRST,
    ID_STYLE_NORMAL_BG_LAST=ID_STYLE_NORMAL_BG_FIRST+100,
    ID_STYLE_SELECT_FG_FIRST,
    ID_STYLE_SELECT_FG_LAST=ID_STYLE_SELECT_FG_FIRST+100,
    ID_STYLE_SELECT_BG_FIRST,
    ID_STYLE_SELECT_BG_LAST=ID_STYLE_SELECT_BG_FIRST+100,
    ID_STYLE_HILITE_FG_FIRST,
    ID_STYLE_HILITE_FG_LAST=ID_STYLE_HILITE_FG_FIRST+100,
    ID_STYLE_HILITE_BG_FIRST,
    ID_STYLE_HILITE_BG_LAST=ID_STYLE_HILITE_BG_FIRST+100,
    ID_STYLE_ACTIVE_BG_FIRST,
    ID_STYLE_ACTIVE_BG_LAST=ID_STYLE_ACTIVE_BG_FIRST+100,
    ID_STYLE_UNDERLINE_FIRST,
    ID_STYLE_UNDERLINE_LAST=ID_STYLE_UNDERLINE_FIRST+100,
    ID_STYLE_STRIKEOUT_FIRST,
    ID_STYLE_STRIKEOUT_LAST=ID_STYLE_STRIKEOUT_FIRST+100,
    ID_STYLE_BOLD_FIRST,
    ID_STYLE_BOLD_LAST=ID_STYLE_BOLD_FIRST+100,
    ID_USE_INITIAL_SIZE,
    ID_SET_INITIAL_SIZE,
    ID_TOGGLE_BROWSER,
    ID_TOGGLE_DOTFILES,
    ID_FINDFILES,
    ID_SEARCHPATHS,
    ID_EXPRESSION,
    ID_GOTO_LINE,
    ID_GOTO_SELECTED,
    ID_SEARCH,
    ID_REPLACE,
    ID_SEARCH_SEL_FORW,
    ID_SEARCH_SEL_BACK,
    ID_SEARCH_NXT_FORW,
    ID_SEARCH_NXT_BACK,
    ID_ISEARCH_TEXT,
    ID_ISEARCH_REVERSE,
    ID_ISEARCH_IGNCASE,
    ID_ISEARCH_REGEX,
    ID_ISEARCH_PREV,
    ID_ISEARCH_NEXT,
    ID_ISEARCH_START,
    ID_ISEARCH_FINISH,
    ID_ISEARCH_HIST_UP,
    ID_ISEARCH_HIST_DN,
    ID_ISEARCH_WORDS,
    ID_TABSELECT_0,
    ID_TABSELECT_1,
    ID_TABSELECT_2,
    ID_TABSELECT_3,
    ID_TABSELECT_4,
    ID_TABSELECT_5,
    ID_TABSELECT_6,
    ID_TABSELECT_7,
    ID_TABSELECT_8,
    ID_SHELL_DIALOG,
    ID_SHELL_FILTER,
    ID_SHELL_COMMAND,
    ID_SHELL_CANCEL,
    ID_SHELL_OUTPUT,
    ID_SHELL_ERROR,
    ID_SHELL_DONE,
    ID_URL_ENCODE,
    ID_URL_DECODE,
    ID_CUST_CMD0,
    ID_CUST_CMD1,
    ID_CUST_CMD2,
    ID_CUST_CMD3,
    ID_CUST_CMD4,
    ID_CUST_CMD5,
    ID_CUST_CMD6,
    ID_CUST_CMD7,
    ID_CUST_CMD8,
    ID_CUST_CMD9,
    ID_CUST_CMDA,
    ID_CUST_CMDB,
    ID_CUST_CMDC,
    ID_CUST_CMDD,
    ID_CUST_CMDE,
    ID_CUST_CMDF,
    ID_LAST
    };
public:

  // Create new text window
  TextWindow(Adie* a);

  // Create window
  virtual void create();

  // Return Adie application
  Adie* getApp() const { return (Adie*)FXMainWindow::getApp(); }

  // Change this window's filename
  void setFilename(const FXString& file){ filename=file; }

  // Return this window's filename
  const FXString& getFilename() const { return filename; }

  // Change file time
  void setFiletime(FXTime t){ filetime=t; }

  // Get file time
  FXTime getFiletime() const { return filetime; }

  // Set filename is set flag
  void setFilenameSet(FXbool set){ filenameset=set; }

  // Has a filename been set
  FXbool isFilenameSet() const { return filenameset; }

  // Is it modified
  FXbool isModified() const;

  // Set current file of directory browser
  void setBrowserCurrentFile(const FXString& file);

  // Get current file of directory browser
  FXString getBrowserCurrentFile() const;

  // Set current directory
  void setBrowserCurrentDirectory(const FXString& dir);

  // Get current directory
  FXString getBrowserCurrentDirectory() const;

  // Set editable flag
  void setEditable(FXbool edit=true);

  // Is it editable
  FXbool isEditable() const;

  // Change current file pattern
  void setCurrentPattern(FXint n);

  // Return current file pattern
  FXint getCurrentPattern() const;

  // Change pattern list
  void setPatternList(const FXString& patterns);

  // Get pattern list
  FXString getPatternList() const;

  // Set search paths
  void setSearchPaths(const FXString& paths);

  // Get search paths
  FXString getSearchPaths() const;

  // Search for file using search paths
  FXString searchForFile(const FXString& file);

  // Visit position
  void visitPosition(FXint pos);

  // Visit given line, and column
  void visitLine(FXint line,FXint column=0);

  // Ask user to save changes
  FXbool saveChanges();

  // Check for external changes
  FXbool checkChanges();

  // Load text from file
  FXbool loadFile(const FXString& file);

  // Save text to file
  FXbool saveFile(const FXString& file);

  // Save text to file
  FXbool saveToFile(const FXString& file);

  // Insert file at cursor or replace selection
  FXbool insertFromFile(const FXString& file);

  // Extract selection to file
  FXbool extractToFile(const FXString& file);

  // Add bookmark at given position pos
  void setBookmark(FXint pos);

  // Clear bookmarks
  void clearBookmarks();

  // Read/write bookmarks
  void readBookmarks(const FXString& file);
  void writeBookmarks(const FXString& file);

  // Remove bookmark at given position pos
  void clearBookmark(FXint pos);

  // Read/write view
  void readView(const FXString& file);
  void writeView(const FXString& file);

  // Start incremental search
  void startISearch();

  // Finish incremental search
  void finishISearch();

  // Search next incremental text
  FXbool performISearch(const FXString& text,FXuint opts,FXbool advance=false,FXbool notify=false);

  // Start shell command
  FXbool startCommand(const FXString& command,FXint inp=0,FXint out=0,FXint flgs=0);

  // Start shell command
  FXbool startCommand(const FXString& command,FXString& input);

  // Done with command
  FXbool doneCommand();

  // Stop shell command
  FXbool stopCommand();

  // Create command line
  FXString makeCommandline(const FXString& cmd) const;

  // Set syntax
  void setSyntax(Syntax* syn);

  // Get syntax
  Syntax* getSyntax() const { return syntax; }

  // Determine syntax
  void determineSyntax();

  // Parse modeline
  void parseModeline();

  // Set status message
  void setStatusMessage(const FXString& msg);

  // Detach window
  virtual void detach();

  // Close the window, return true if actually closed
  virtual FXbool close(FXbool notify=false);

  // Delete text window
  virtual ~TextWindow();
  };


typedef FXObjectListOf<TextWindow> TextWindowList;

#endif
