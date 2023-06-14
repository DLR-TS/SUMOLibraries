/********************************************************************************
*                                                                               *
*              T h e   P a t h F i n d e r   F i l e   B r o w s e r            *
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
#ifndef PATHFINDER_H
#define PATHFINDER_H


class PropertyDialog;
class Preferences;


// Version
#define VERSION_MAJOR 2
#define VERSION_MINOR 1
#define VERSION_PATCH 0


// PathFinder Main Window
class PathFinderMain : public FXMainWindow {
  FXDECLARE(PathFinderMain)
protected:
  FXToolBarShell     *dragshell1;               // For floating menu bar
  FXToolBarShell     *dragshell2;               // For floating tool bar
  FXToolBarShell     *dragshell3;               // For floating location bar
  FXToolBar          *toolbar;                  // Toolbar
  FXToolBar          *locationbar;              // Location bar
  FXStatusBar        *statusbar;                // Status bar
  FXMenuPane         *filemenu;
  FXMenuPane         *editmenu;
  FXMenuPane         *viewmenu;
  FXMenuPane         *gomenu;
  FXMenuPane         *arrangemenu;
  FXMenuPane         *sortmenu;
  FXMenuPane         *prefmenu;
  FXMenuPane         *helpmenu;
  FXMenuPane         *bookmarkmenu;
  FXDirBox           *dirbox;		        // Directory box
  FXSwitcher         *switcher;		        // Switcher for preview
  FXSplitter         *splitter;
  FXVerticalFrame    *group1;
  FXVerticalFrame    *group2;
  FXDirList          *dirlist;		        // Directory tree
  FXFileList         *filelist;		        // File list
  FXComboBox         *pattern;		        // Filter pattern
  FXLabel            *diskspace;                // Show free space
  FXTextField        *address;		        // Address field
  FXImageView        *imagepreview;	        // Image preview panel
  FXRecentFiles       bookmarkeddirs;	        // Bookmarked directories
  FXFileAssociations *associations;	        // File associations
  FXIcon             *foxbigicon;	        // Icons
  FXIcon             *foxminiicon;
  FXIcon             *cuticon;
  FXIcon             *copyicon;
  FXIcon             *moveicon;
  FXIcon             *linkicon;
  FXIcon             *renameicon;
  FXIcon             *pasteicon;
  FXIcon             *upicon;
  FXIcon             *homeicon;
  FXIcon             *backicon;
  FXIcon             *forwicon;
  FXIcon             *bigiconsicon;
  FXIcon             *miniiconsicon;
  FXIcon             *detailsicon;
  FXIcon             *mapicon;
  FXIcon             *unmapicon;
  FXIcon             *propicon;
  FXIcon             *deleteicon;
  FXIcon             *bookmarkicon;
  FXIcon             *addbookicon;
  FXIcon             *delbookicon;
  FXIcon             *clrbooksicon;
  FXIcon             *sortingicon;
  FXIcon             *execicon;
  FXIcon             *newdiricon;
  FXIcon             *workicon;
  FXIcon             *closeicon;
  FXIcon             *locationicon;
  FXIcon             *entericon;
  FXIcon             *rotatelefticon;
  FXIcon             *rotaterighticon;
  FXIcon             *quiticon;
  FXIcon             *configicon;
  FXIcon             *warningicon;
  FXIcon             *shownicon;
  FXIcon             *hiddenicon;
  FXIcon             *searchicon;
  FXString            editor;		        // Editor command
  FXString            execpaths;                // Search in these paths
  FXString            terminal;		        // Terminal command
  FXString            program;		        // Last program
  FXString            visiteddir[10];	        // Visited directories
  FXint               visiting;		        // Currently visited directory
  FXint               totalCount;
  FXlong              totalSpace;
  FXuint              selectedCount;
  FXlong              selectedSpace;
  FXuint              selectedModeBits[16];
  FXString            statusInfo;
  FXbool              preview;		        // Preview mode
  FXbool              blending;		        // Icon blending
  FXbool              scaling;                  // Image scaled
#if defined(DIRWATCH)
  FXDirWatch dirwatch; // FIXME
#endif
protected:
  PathFinderMain();
  FXbool haveSelectedFiles() const;
  void visitDirectory(const FXString& dir);
  FXbool previewImage(const FXString& filename);
  void closePreview();
  FXbool executeCommandline(const FXString& commandline);
  FXString makeCommandline(const FXString& executable) const;
public:
  long onUpdate(FXObject*,FXSelector,void*);
  long onCmdAbout(FXObject*,FXSelector,void*);
  long onCmdClosePreview(FXObject*,FXSelector,void*);
  long onUpdClosePreview(FXObject*,FXSelector,void*);
  long onCmdDirTree(FXObject*,FXSelector,void*);
  long onCmdDirectory(FXObject*,FXSelector,void*);
  long onCmdGotoLocation(FXObject*,FXSelector,void*);
  long onCmdClearLocation(FXObject*,FXSelector,void*);
  long onCmdUpDirectory(FXObject*,FXSelector,void*);
  long onUpdTitle(FXObject*,FXSelector,void*);
  long onUpdFiles(FXObject*,FXSelector,void*);
  long onCmdHomeDirectory(FXObject*,FXSelector,void*);
  long onCmdWorkDirectory(FXObject*,FXSelector,void*);
  long onCmdRecentDirectory(FXObject*,FXSelector,void*);
  long onCmdSaveSettings(FXObject*,FXSelector,void*);
  long onCmdNewPathFinder(FXObject*,FXSelector,void*);
  long onFilePopup(FXObject*,FXSelector,void*);
  long onFileDblClicked(FXObject*,FXSelector,void*);

  long onFileSelected(FXObject*,FXSelector,void*);
  long onFileDeselected(FXObject*,FXSelector,void*);
  long onFileInserted(FXObject*,FXSelector,void*);
  long onFileDeleted(FXObject*,FXSelector,void*);

  long onUpdStatusline(FXObject*,FXSelector,void*);
  long onCmdProperties(FXObject*,FXSelector,void*);
  long onCmdBookmark(FXObject*,FXSelector,void*);
  long onCmdUnBookmark(FXObject*,FXSelector,void*);
  long onCmdBackDirectory(FXObject*,FXSelector,void*);
  long onUpdBackDirectory(FXObject*,FXSelector,void*);
  long onCmdForwardDirectory(FXObject*,FXSelector,void*);
  long onUpdForwardDirectory(FXObject*,FXSelector,void*);
  long onUpdDiskSpace(FXObject*,FXSelector,void*);

  long onCmdNew(FXObject*,FXSelector,void*);
  long onUpdNew(FXObject*,FXSelector,void*);
  long onCmdCopy(FXObject*,FXSelector,void*);
  long onCmdMove(FXObject*,FXSelector,void*);
  long onCmdLink(FXObject*,FXSelector,void*);
  long onCmdRename(FXObject*,FXSelector,void*);
  long onCmdDelete(FXObject*,FXSelector,void*);
  long onUpdSelected(FXObject*,FXSelector,void*);
  long onUpdSelectedVisible(FXObject*,FXSelector,void*);

  long onCmdFilter(FXObject*,FXSelector,void*);
  long onCmdGotoDir(FXObject*,FXSelector,void*);
  long onCmdChmod(FXObject*,FXSelector,void*);
  long onUpdChmod(FXObject*,FXSelector,void*);
  long onUpdOwner(FXObject*,FXSelector,void*);
  long onUpdGroup(FXObject*,FXSelector,void*);
  long onUpdCreateTime(FXObject*,FXSelector,void*);
  long onUpdModifyTime(FXObject*,FXSelector,void*);
  long onUpdAccessTime(FXObject*,FXSelector,void*);
  long onUpdFileType(FXObject*,FXSelector,void*);
  long onUpdFileLocation(FXObject*,FXSelector,void*);
  long onUpdFileSize(FXObject*,FXSelector,void*);
  long onUpdFileDesc(FXObject*,FXSelector,void*);
  long onCmdOpen(FXObject*,FXSelector,void*);
  long onCmdOpenWith(FXObject*,FXSelector,void*);
  long onCmdOpenWithEditor(FXObject*,FXSelector,void*);
  long onCmdRun(FXObject*,FXSelector,void*);
  long onCmdTerminal(FXObject*,FXSelector,void*);
  long onSigHarvest(FXObject*,FXSelector,void*);
  long onClickedImagePreview(FXObject*,FXSelector,void*);
  long onCmdRotateImage(FXObject*,FXSelector,void*);
  long onUpdRotateImage(FXObject*,FXSelector,void*);
  long onCmdImageSize(FXObject*,FXSelector,void*);
  long onUpdImageSize(FXObject*,FXSelector,void*);
  long onCmdToggleHidden(FXObject*,FXSelector,void*);
  long onUpdToggleHidden(FXObject*,FXSelector,void*);
  long onCmdWildcardSelect(FXObject*,FXSelector,void*);
  long onCmdPreferences(FXObject*,FXSelector,void*);
public:
  enum{
    ID_ABOUT=FXMainWindow::ID_LAST,
    ID_FILEFILTER,
    ID_DIRECTORYLIST,
    ID_FILELIST,
    ID_DIRBOX,
    ID_UPDATE_FILES,
    ID_DISKSPACE,
    ID_UPDIRECTORY,
    ID_GOTO_LOCATION,
    ID_CLEAR_LOCATION,
    ID_GO_WORK,
    ID_GO_HOME,
    ID_GO_BACKWARD,
    ID_GO_FORWARD,
    ID_GO_RECENT,
    ID_SAVE_SETTINGS,
    ID_NEW_PATHFINDER,
    ID_STATUSLINE,
    ID_PROPERTIES,
    ID_BOOKMARK,
    ID_UNBOOKMARK,
    ID_RENAME,
    ID_COPY,
    ID_MOVE,
    ID_LINK,
    ID_DELETE,
    ID_NEW,
    ID_GOTO_DIR,
    ID_OPEN,
    ID_OPEN_WITH,
    ID_OPEN_WITH_EDITOR,
    ID_RUN,
    ID_TERMINAL,
    ID_PROP_XOTH,          // File modes (order important)
    ID_PROP_WOTH=ID_PROP_XOTH+1,
    ID_PROP_ROTH=ID_PROP_XOTH+2,
    ID_PROP_XGRP=ID_PROP_XOTH+3,
    ID_PROP_WGRP=ID_PROP_XOTH+4,
    ID_PROP_RGRP=ID_PROP_XOTH+5,
    ID_PROP_XUSR=ID_PROP_XOTH+6,
    ID_PROP_WUSR=ID_PROP_XOTH+7,
    ID_PROP_RUSR=ID_PROP_XOTH+8,
    ID_PROP_SUID=ID_PROP_XOTH+13,
    ID_PROP_SGID=ID_PROP_XOTH+14,
    ID_PROP_SVTX=ID_PROP_XOTH+15,
    ID_PROP_OWNER,         // File ownership
    ID_PROP_GROUP,
    ID_PROP_CREATED,
    ID_PROP_MODIFIED,
    ID_PROP_ACCESSED,
    ID_PROP_TYPE,
    ID_PROP_LOCATION,
    ID_PROP_SIZE,
    ID_PROP_DESC,
    ID_CLOSE_PREVIEW,
    ID_IMAGE_PREVIEW,
    ID_IMAGE_ROTATE_LEFT,
    ID_IMAGE_ROTATE_RIGHT,
    ID_MINI_SIZE,
    ID_NORMAL_SIZE,
    ID_MEDIUM_SIZE,
    ID_GIANT_SIZE,
    ID_TOGGLE_HIDDEN,
    ID_HARVEST,
    ID_PREFERENCES,
    ID_WILDCARD_SELECT,
    ID_LAST
    };
public:

  // Construct window
  PathFinderMain(FXApp* a);

  // Create
  virtual void create();

  // Closed window
  virtual FXbool close(FXbool notify=false);

  // Change file name
  void setFilename(const FXString& path);

  // Return file name, if any
  FXString getFilename() const;

  // Switch current directory
  void setDirectory(const FXString& path);

  // Return current directory
  FXString getDirectory() const;

  // Return selected filenames
  FXString* getSelectedFiles() const;

  // Return file associations
  FXFileAssociations *getAssociations() const { return associations; }

  // Set list of patterns
  void setPatternList(const FXString& patterns);

  // Get list of patterns
  FXString getPatternList() const;

  // Chance current pattern
  void setCurrentPattern(FXint n);

  // Get currently selected pattern
  FXint getCurrentPattern() const;

  // Save settings
  void saveSettings();

  // Load settings
  void loadSettings();

  // Destroy
  virtual ~PathFinderMain();
  };


#endif

