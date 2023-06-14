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
#include "xincs.h"
#include "fx.h"
#include "fxkeys.h"
#include "PathFinder.h"
#include "PropertyDialog.h"
#include "Preferences.h"
#include "CopyDialog.h"
#include "icons.h"

// Support for extra image formats
#ifdef HAVE_PNG_H
#include "FXPNGImage.h"
#endif
#ifdef HAVE_JPEG_H
#include "FXJPGImage.h"
#endif
#ifdef HAVE_TIFF_H
#include "FXTIFImage.h"
#endif
#ifdef HAVE_WEBP_H
#include "FXWEBPImage.h"
#endif
#include "FXICOImage.h"
#include "FXTGAImage.h"
#include "FXRGBImage.h"
#include "FXPCXImage.h"



/*
  Notes:

  - Copy / Paste /Cut doesn't seem to work
  - If you drag some files to a certain directory in the dir-list, hilight the directory so
    we're sure we are dropping it in the right directory...
  - Edit menu layout should change:

      Undo
      Redo
      separator
      Cut
      Copy
      Paste
      Delete
      separator
      Select
      DeSelect
      Invert Selection


  - Mount/Unmount functionality....
  - A special bookmark button like home/work for /mnt/cdrom or/and just the /mnt directory
  - Selecting multiple files and clicking Open With only displays the first file ...
    not the whole range you've selected.... mayby we should show the first and the last
    file of the selection: (file1.htm ... file99.htm)
  - Change 'Delete Files' dynamically  depending on the amount of files you've selected:
    so 1 file shows only Delete File....  same thing for the dialog that shows up....
  - Specifications for file commands:
    https://specifications.freedesktop.org/desktop-entry-spec/latest/ar01s06.html
  - Observe t and c option flags in association; for t (terminal) run <terminal> -e command.
*/


// Executable name
#ifdef WIN32
#define PATHFINDEREXEC "PathFinder.exe"
#else
#define PATHFINDEREXEC "PathFinder"
#endif

/*******************************************************************************/

// Map
FXDEFMAP(PathFinderMain) PathFinderMainMap[]={
  FXMAPFUNC(SEL_UPDATE,0,PathFinderMain::onUpdate),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_DIRECTORYLIST,PathFinderMain::onCmdDirectory),
  FXMAPFUNC(SEL_DOUBLECLICKED,PathFinderMain::ID_FILELIST,PathFinderMain::onFileDblClicked),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,PathFinderMain::ID_FILELIST,PathFinderMain::onFilePopup),
  FXMAPFUNC(SEL_SELECTED,PathFinderMain::ID_FILELIST,PathFinderMain::onFileSelected),
  FXMAPFUNC(SEL_DESELECTED,PathFinderMain::ID_FILELIST,PathFinderMain::onFileDeselected),
  FXMAPFUNC(SEL_INSERTED,PathFinderMain::ID_FILELIST,PathFinderMain::onFileInserted),
  FXMAPFUNC(SEL_DELETED,PathFinderMain::ID_FILELIST,PathFinderMain::onFileDeleted),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_ABOUT,PathFinderMain::onCmdAbout),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_DIRBOX,PathFinderMain::onCmdDirTree),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_UPDIRECTORY,PathFinderMain::onCmdUpDirectory),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_CLEAR_LOCATION,PathFinderMain::onCmdClearLocation),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_GOTO_LOCATION,PathFinderMain::onCmdGotoLocation),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_UPDATE_FILES,PathFinderMain::onUpdFiles),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_STATUSLINE,PathFinderMain::onUpdStatusline),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_GO_WORK,PathFinderMain::onCmdWorkDirectory),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_GO_HOME,PathFinderMain::onCmdHomeDirectory),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_GO_RECENT,PathFinderMain::onCmdRecentDirectory),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_GO_BACKWARD,PathFinderMain::onCmdBackDirectory),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_GO_BACKWARD,PathFinderMain::onUpdBackDirectory),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_GO_FORWARD,PathFinderMain::onCmdForwardDirectory),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_GO_FORWARD,PathFinderMain::onUpdForwardDirectory),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_DISKSPACE,PathFinderMain::onUpdDiskSpace),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_SAVE_SETTINGS,PathFinderMain::onCmdSaveSettings),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_NEW_PATHFINDER,PathFinderMain::onCmdNewPathFinder),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_PROPERTIES,PathFinderMain::onCmdProperties),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_BOOKMARK,PathFinderMain::onCmdBookmark),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_UNBOOKMARK,PathFinderMain::onCmdUnBookmark),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_NEW,PathFinderMain::onCmdNew),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_NEW,PathFinderMain::onUpdNew),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_COPY,PathFinderMain::onCmdCopy),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_COPY,PathFinderMain::onUpdSelected),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_MOVE,PathFinderMain::onCmdMove),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_MOVE,PathFinderMain::onUpdSelected),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_LINK,PathFinderMain::onCmdLink),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_LINK,PathFinderMain::onUpdSelected),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_RENAME,PathFinderMain::onCmdRename),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_RENAME,PathFinderMain::onUpdSelected),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_DELETE,PathFinderMain::onCmdDelete),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_DELETE,PathFinderMain::onUpdSelected),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_FILEFILTER,PathFinderMain::onCmdFilter),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_GOTO_DIR,PathFinderMain::onCmdGotoDir),
  FXMAPFUNCS(SEL_COMMAND,PathFinderMain::ID_PROP_XOTH,PathFinderMain::ID_PROP_SVTX,PathFinderMain::onCmdChmod),
  FXMAPFUNCS(SEL_UPDATE,PathFinderMain::ID_PROP_XOTH,PathFinderMain::ID_PROP_SVTX,PathFinderMain::onUpdChmod),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_OWNER,PathFinderMain::onUpdOwner),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_GROUP,PathFinderMain::onUpdGroup),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_CREATED,PathFinderMain::onUpdCreateTime),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_MODIFIED,PathFinderMain::onUpdModifyTime),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_ACCESSED,PathFinderMain::onUpdAccessTime),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_TYPE,PathFinderMain::onUpdFileType),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_LOCATION,PathFinderMain::onUpdFileLocation),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_SIZE,PathFinderMain::onUpdFileSize),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_PROP_DESC,PathFinderMain::onUpdFileDesc),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_OPEN,PathFinderMain::onUpdSelected),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_OPEN,PathFinderMain::onCmdOpen),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_OPEN_WITH,PathFinderMain::onUpdSelected),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_OPEN_WITH,PathFinderMain::onCmdOpenWith),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_OPEN_WITH_EDITOR,PathFinderMain::onUpdSelected),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_OPEN_WITH_EDITOR,PathFinderMain::onCmdOpenWithEditor),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_RUN,PathFinderMain::onCmdRun),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_TERMINAL,PathFinderMain::onCmdTerminal),
  FXMAPFUNC(SEL_SIGNAL,PathFinderMain::ID_HARVEST,PathFinderMain::onSigHarvest),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_CLOSE_PREVIEW,PathFinderMain::onCmdClosePreview),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_CLOSE_PREVIEW,PathFinderMain::onUpdClosePreview),
  FXMAPFUNCS(SEL_COMMAND,PathFinderMain::ID_IMAGE_ROTATE_LEFT,PathFinderMain::ID_IMAGE_ROTATE_RIGHT,PathFinderMain::onCmdRotateImage),
  FXMAPFUNCS(SEL_UPDATE,PathFinderMain::ID_IMAGE_ROTATE_LEFT,PathFinderMain::ID_IMAGE_ROTATE_RIGHT,PathFinderMain::onUpdRotateImage),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_TOGGLE_HIDDEN,PathFinderMain::onCmdToggleHidden),
  FXMAPFUNC(SEL_UPDATE,PathFinderMain::ID_TOGGLE_HIDDEN,PathFinderMain::onUpdToggleHidden),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_PREFERENCES,PathFinderMain::onCmdPreferences),
  FXMAPFUNC(SEL_COMMAND,PathFinderMain::ID_WILDCARD_SELECT,PathFinderMain::onCmdWildcardSelect),
  FXMAPFUNCS(SEL_COMMAND,PathFinderMain::ID_MINI_SIZE,PathFinderMain::ID_GIANT_SIZE,PathFinderMain::onCmdImageSize),
  FXMAPFUNCS(SEL_UPDATE,PathFinderMain::ID_MINI_SIZE,PathFinderMain::ID_GIANT_SIZE,PathFinderMain::onUpdImageSize),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,PathFinderMain::ID_IMAGE_PREVIEW,PathFinderMain::onClickedImagePreview)
  };


// Object implementation
FXIMPLEMENT(PathFinderMain,FXMainWindow,PathFinderMainMap,ARRAYNUMBER(PathFinderMainMap))

/*******************************************************************************/

// Fallback patterns
const FXchar fallbackPatterns[]=
  "All Files (*)\n"
  "Documents (*.ps,*.pdf,*.ps.gz,*.chm,*.doc,*.odt,*.ppt)\n"
  "Web Pages (*.html,*.htm,*.xml)\n"
  "Images (*.png,*.bmp,*.jpg,*.jpeg,*.ico,*.cur,*.gif,*.tif,*.tiff,*.dds,*.tga,*.pcx,*.xpm,*.xbm,*.jp2,*.ras,*.rgb,*.iff,*.ppm,*.pnm,*.pbm,*.pgm)\n"
  "Music (*.ogg,*.mp3,*.wma,*.flac,*.wav)\n"
  "Movies (*.avi,*.mkv,*.mp4,*.mpg,*.qt)\n"
  "Archives (*.tar,*.tar.gz,*.tgz,*.tar.bz2,*.tar.lzma,*.tar.7z,*.tar.xz,*.zip)\n"
  "C/C++ Sources (*.cpp,*.cxx,*.c++,*.cc,*.C,*.c,*.hpp,*.hxx,*.h++,*.hh,*.H,*.h)\n"
  "C++ Source Files (*.cpp,*.cxx,*.c++,*.cc,*.C)\n"
  "C++ Header Files (*.hpp,*.hxx,*.h++,*.hh,*.H,*.h)\n"
  "C Source Files (*.c)\n"
  "C Header Files (*.h)\n"
  "Libraries (*.a,*.so,*.lib)\n"
  "Objects (*.o,*.lo,*.obj)"
  ;


/*******************************************************************************/

// Initialize main window
PathFinderMain::PathFinderMain(){
  }


// Make some windows
PathFinderMain::PathFinderMain(FXApp* a):FXMainWindow(a,"PathFinder",nullptr,nullptr,DECOR_ALL,0,0,800,600,0,0),bookmarkeddirs(a,"Bookmarked Directories")
#if defined(DIRWATCH)
  ,dirwatch(a) // FIXME
#endif
{

  // Totals
  totalCount=0;
  totalSpace=0L;
  selectedCount=0;
  selectedSpace=0L;

  // Zero out mode bits
  selectedModeBits[0]=0;
  selectedModeBits[1]=0;
  selectedModeBits[2]=0;
  selectedModeBits[3]=0;
  selectedModeBits[4]=0;
  selectedModeBits[5]=0;
  selectedModeBits[6]=0;
  selectedModeBits[7]=0;
  selectedModeBits[8]=0;
  selectedModeBits[9]=0;
  selectedModeBits[10]=0;
  selectedModeBits[11]=0;
  selectedModeBits[12]=0;
  selectedModeBits[13]=0;
  selectedModeBits[14]=0;
  selectedModeBits[15]=0;

  // Make some icons
  foxbigicon=new FXGIFIcon(getApp(),foxbig);
  foxminiicon=new FXGIFIcon(getApp(),foxmini);
  cuticon=new FXGIFIcon(getApp(),cut_gif);
  copyicon=new FXGIFIcon(getApp(),copy_gif);
  moveicon=new FXGIFIcon(getApp(),move_gif);
  linkicon=new FXGIFIcon(getApp(),link_gif);
  renameicon=new FXGIFIcon(getApp(),rename_gif);
  pasteicon=new FXGIFIcon(getApp(),paste);
  upicon=new FXGIFIcon(getApp(),dirup_gif);
  homeicon=new FXGIFIcon(getApp(),home_gif);
  backicon=new FXGIFIcon(getApp(),goback_gif);
  forwicon=new FXGIFIcon(getApp(),goforw_gif);
  bigiconsicon=new FXBMPIcon(getApp(),bigicons,0,IMAGE_ALPHAGUESS);
  miniiconsicon=new FXBMPIcon(getApp(),smallicons,0,IMAGE_ALPHAGUESS);
  detailsicon=new FXBMPIcon(getApp(),details,0,IMAGE_ALPHAGUESS);
  mapicon=new FXBMPIcon(getApp(),maphost,0,IMAGE_ALPHAGUESS);
  unmapicon=new FXBMPIcon(getApp(),unmaphost,0,IMAGE_ALPHAGUESS);
  propicon=new FXGIFIcon(getApp(),property_gif);
  deleteicon=new FXGIFIcon(getApp(),delete_gif,0,IMAGE_THRESGUESS);
  bookmarkicon=new FXGIFIcon(getApp(),bookmrk_gif);
  addbookicon=new FXGIFIcon(getApp(),bookset_gif);
  delbookicon=new FXGIFIcon(getApp(),bookdel_gif);
  clrbooksicon=new FXGIFIcon(getApp(),bookclr_gif);
  sortingicon=new FXBMPIcon(getApp(),sorting,0,IMAGE_ALPHAGUESS);
  execicon=new FXGIFIcon(getApp(),blocks_gif);
  newdiricon=new FXGIFIcon(getApp(),foldernew_gif);
  workicon=new FXGIFIcon(getApp(),work_gif);
  closeicon=new FXGIFIcon(getApp(),closepanel);
  locationicon=new FXGIFIcon(getApp(),location);
  entericon=new FXGIFIcon(getApp(),enter);
  rotatelefticon=new FXGIFIcon(getApp(),rotateleft);
  rotaterighticon=new FXGIFIcon(getApp(),rotateright);
  quiticon=new FXGIFIcon(getApp(),quit_gif);
  configicon=new FXGIFIcon(getApp(),config_gif);
  warningicon=new FXGIFIcon(getApp(),warningicon_gif);
  shownicon=new FXGIFIcon(getApp(),fileshown);
  hiddenicon=new FXGIFIcon(getApp(),filehidden);
  searchicon=new FXGIFIcon(getApp(),search);

  // Set application icons for Window Manager
  setIcon(foxbigicon);
  setMiniIcon(foxminiicon);

  // Site where to dock
  FXDockSite *docksite=new FXDockSite(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);

  // Menu Bar
  dragshell1=new FXToolBarShell(this,FRAME_RAISED);
  FXMenuBar *menubar=new FXMenuBar(docksite,dragshell1,LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
  new FXToolBarGrip(menubar,menubar,FXMenuBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);

  // Tool Bar
  dragshell2=new FXToolBarShell(this,FRAME_RAISED);
  toolbar=new FXToolBar(docksite,dragshell2,LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
  new FXToolBarGrip(toolbar,toolbar,FXToolBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);

  // Location Bar
  dragshell3=new FXToolBarShell(this,FRAME_RAISED);
  locationbar=new FXToolBar(docksite,dragshell3,LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
  new FXToolBarGrip(locationbar,locationbar,FXToolBar::ID_TOOLBARGRIP,TOOLBARGRIP_DOUBLE);


  // Status bar
  statusbar=new FXStatusBar(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|FRAME_RAISED|STATUSBAR_WITH_DRAGCORNER);
  FXStatusLine *statusline=statusbar->getStatusLine();
  statusline->setTarget(this);
  statusline->setSelector(ID_STATUSLINE);

  // Subtle plug for LINUX
  new FXButton(statusbar,tr("\tAbout PathFinder"),foxminiicon,this,ID_ABOUT,LAYOUT_CENTER_Y|LAYOUT_RIGHT);
  new FXToggleButton(statusbar,tr("\tShow hidden files\tShow hidden files and directories."),tr("\tHide Hidden Files\tHide hidden files and directories."),hiddenicon,shownicon,this,ID_TOGGLE_HIDDEN,TOGGLEBUTTON_TOOLBAR|LAYOUT_CENTER_Y|LAYOUT_RIGHT,0,0,0,0, 3,3,3,3);

  // Pattern
  pattern=new FXComboBox(statusbar,25,this,ID_FILEFILTER,COMBOBOX_STATIC|LAYOUT_RIGHT|LAYOUT_FIX_WIDTH|FRAME_SUNKEN|FRAME_THICK, 0,0,200,0, 0,0,1,1);
  pattern->setNumVisible(4);
  pattern->setTipText(tr("File Filter"));
  pattern->setHelpText(tr("Filter displayed files with a wildcard pattern."));

  // Caption before pattern
  new FXLabel(statusbar,tr("Pattern:"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  diskspace=new FXLabel(statusbar,"Free: 1.0GB / 1.0GB",nullptr,FRAME_SUNKEN|JUSTIFY_RIGHT|LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  diskspace->setTarget(this);
  diskspace->setSelector(ID_DISKSPACE);

  // Make file associations object; shared between FXFileList and FXDirList
  associations=new FXFileAssociations(getApp());

  // Main window interior
  FXHorizontalFrame * splitterbox=new FXHorizontalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED,0,0,0,0, 2,2,2,2, 0,0);
  splitter=new FXSplitter(splitterbox,LAYOUT_SIDE_TOP|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_FILL_Y|SPLITTER_TRACKING);
  group1=new FXVerticalFrame(splitter,LAYOUT_FIX_WIDTH|LAYOUT_FILL_Y|FRAME_THICK|FRAME_SUNKEN, 0,0,180,0, 0,0,0,0,0,0);
  group2=new FXVerticalFrame(splitter,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_THICK|FRAME_SUNKEN, 0,0,0,0, 0,0,0,0,0,0);

  // Header above folders
  FXHorizontalFrame *header1=new FXHorizontalFrame(group1,LAYOUT_FILL_X|FRAME_RAISED|FRAME_THICK,0,0,0,0, 0,0,0,0, 0,0);
  new FXLabel(header1,tr("Folders"),nullptr,LAYOUT_FILL_X|JUSTIFY_LEFT);
  new FXButton(header1,"",closeicon,group1,FXWindow::ID_HIDE,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0);

  // Folder List
  dirlist=new FXDirList(group1,this,ID_DIRECTORYLIST,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_RIGHT|TREELIST_SHOWS_LINES|TREELIST_SHOWS_BOXES|TREELIST_BROWSESELECT|DIRLIST_NO_OWN_ASSOC);
  dirlist->setAssociations(associations);
  dirlist->dropEnable();

  // Header above files
  FXHorizontalFrame *header2=new FXHorizontalFrame(group2,LAYOUT_FILL_X|FRAME_RAISED|FRAME_THICK,0,0,0,0, 0,0,0,0, 0,0);
  FXLabel* fileslabel=new FXLabel(header2,tr("Files in: "),nullptr,LAYOUT_FILL_X|JUSTIFY_LEFT);
  fileslabel->setTarget(this);
  fileslabel->setSelector(ID_UPDATE_FILES);
  new FXButton(header2,tr("\tRotate left\tRotate image leftward 90 degrees."),rotatelefticon,this,ID_IMAGE_ROTATE_LEFT,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y,0,0,0,0,0,0,0,0);
  new FXButton(header2,tr("\tRotate right\tRotate image rightward 90 degrees."),rotaterighticon,this,ID_IMAGE_ROTATE_RIGHT,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y,0,0,0,0,0,0,0,0);
  new FXButton(header2,FXString::null,closeicon,this,ID_CLOSE_PREVIEW,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0);

  // Switcher to either image or filelist
  switcher=new FXSwitcher(group2,LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);

  // File List
  filelist=new FXFileList(switcher,this,ID_FILELIST,LAYOUT_FILL_X|LAYOUT_FILL_Y|ICONLIST_MINI_ICONS|ICONLIST_AUTOSIZE|FILELIST_NO_OWN_ASSOC);
  filelist->horizontalScrollBar()->setWheelLines(1);
  filelist->setAssociations(associations,false,true);
  filelist->dropEnable();

  // Image view
  imagepreview=new FXImageView(switcher,nullptr,this,ID_IMAGE_PREVIEW,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  imagepreview->enable(); // enable so we receive mouse messages

  // File menu pane
  filemenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&File"),nullptr,filemenu);
  new FXMenuCommand(filemenu,tr("Open\t\tOpen selected file."),nullptr,this,ID_OPEN);
  new FXMenuCommand(filemenu,tr("Open with...\t\tOpen selected file using program."),nullptr,this,ID_OPEN_WITH);
  new FXMenuCommand(filemenu,tr("Open with editor\t\tOpen selected file in the editor."),nullptr,this,ID_OPEN_WITH_EDITOR);
  new FXMenuSeparator(filemenu);
  new FXMenuCommand(filemenu,tr("&Run...\tCtrl-R\tRun a command."),nullptr,this,ID_RUN);
  new FXMenuCommand(filemenu,tr("&Terminal...\tCtrl-T\tOpen Terminal."),nullptr,this,ID_TERMINAL);
  new FXMenuSeparator(filemenu);
  new FXMenuCommand(filemenu,tr("Re&load\tCtl-L\tReload this directory."),nullptr,filelist,FXFileList::ID_REFRESH);
  new FXMenuSeparator(filemenu);
  new FXMenuCommand(filemenu,tr("New &PathFinder...\tCtrl-P\tStart another PathFinder."),foxminiicon,this,ID_NEW_PATHFINDER);
  new FXMenuCommand(filemenu,tr("&Quit\tCtl-Q\tQuit PathFinder."),quiticon,this,ID_CLOSE);

  // Edit Menu Pane
  editmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Edit"),nullptr,editmenu);
  new FXMenuCommand(editmenu,tr("Cu&t\tCtl-X\tCut to clipboard."),cuticon,filelist,FXFileList::ID_CUT_SEL);
  new FXMenuCommand(editmenu,tr("&Copy\tCtl-C\tCopy to clipboard."),copyicon,filelist,FXFileList::ID_COPY_SEL);
  new FXMenuCommand(editmenu,tr("Delete\t\tDelete Selected files."),deleteicon,this,ID_DELETE);
  new FXMenuCommand(editmenu,tr("&Paste\tCtl-V\tPaste from clipboard."),pasteicon,filelist,FXFileList::ID_PASTE_SEL);
  new FXMenuSeparator(editmenu);
  new FXMenuCommand(editmenu,tr("&Select All\tCtl-A\tSelect all files."),nullptr,filelist,FXFileList::ID_SELECT_ALL);
  new FXMenuCommand(editmenu,tr("Select &Wildcard\tCtl-S\tSelect files matching wildcard."),nullptr,this,ID_WILDCARD_SELECT);
  new FXMenuCommand(editmenu,tr("&Deselect All\t\tDeselect all files."),nullptr,filelist,FXFileList::ID_DESELECT_ALL);
  new FXMenuCommand(editmenu,tr("&Invert Selection\t\tInvert selection."),nullptr,filelist,FXFileList::ID_SELECT_INVERSE);

  // Go Menu Pane
  gomenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Go"),nullptr,gomenu);
  new FXMenuCommand(gomenu,tr("&Up\t\tChange up one level."),upicon,this,ID_UPDIRECTORY);
  new FXMenuCommand(gomenu,tr("&Back\tCtl-B\tChange to previous directory."),backicon,this,ID_GO_BACKWARD);
  new FXMenuCommand(gomenu,tr("&Forward\tCtl-F\tChange to next directory."),forwicon,this,ID_GO_FORWARD);
  new FXMenuCommand(gomenu,tr("&Home\tCtl-H\tChange to home directory."),homeicon,this,ID_GO_HOME);
  new FXMenuCommand(gomenu,tr("&Work\tCtl-W\tChange to current working directory."),workicon,this,ID_GO_WORK);
  new FXMenuCommand(gomenu,tr("&Goto...\tCtl-G\tGoto directory."),nullptr,this,ID_GOTO_DIR);
  new FXMenuSeparator(gomenu);
  new FXMenuCommand(gomenu,tr("Set bookmark\t\tBookmark the current directory."),addbookicon,this,ID_BOOKMARK);
  new FXMenuCommand(gomenu,tr("Unset bookmark\t\tUnset bookmark to current directory."),delbookicon,this,ID_UNBOOKMARK);
  new FXMenuCommand(gomenu,tr("&Clear\t\tClear bookmarks."),clrbooksicon,&bookmarkeddirs,FXRecentFiles::ID_CLEAR);
  FXMenuSeparator* sep1=new FXMenuSeparator(gomenu);
  sep1->setTarget(&bookmarkeddirs);
  sep1->setSelector(FXRecentFiles::ID_ANYFILES);
  new FXMenuCommand(gomenu,"&1",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_1);
  new FXMenuCommand(gomenu,"&2",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_2);
  new FXMenuCommand(gomenu,"&3",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_3);
  new FXMenuCommand(gomenu,"&4",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_4);
  new FXMenuCommand(gomenu,"&5",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_5);
  new FXMenuCommand(gomenu,"&6",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_6);
  new FXMenuCommand(gomenu,"&7",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_7);
  new FXMenuCommand(gomenu,"&8",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_8);
  new FXMenuCommand(gomenu,"&9",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_9);
  new FXMenuCommand(gomenu,"1&0",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_10);
  new FXMenuCommand(gomenu,"11",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_11);
  new FXMenuCommand(gomenu,"12",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_12);
  new FXMenuCommand(gomenu,"13",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_13);
  new FXMenuCommand(gomenu,"14",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_14);
  new FXMenuCommand(gomenu,"15",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_15);
  new FXMenuCommand(gomenu,"16",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_16);
  new FXMenuCommand(gomenu,"17",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_17);
  new FXMenuCommand(gomenu,"18",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_18);
  new FXMenuCommand(gomenu,"19",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_19);
  new FXMenuCommand(gomenu,"20",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_20);

  // Arrange menu
  arrangemenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Arrange"),nullptr,arrangemenu);
  new FXMenuRadio(arrangemenu,tr("&Small Icons\t\tShow small icons."),filelist,FXFileList::ID_SHOW_MINI_ICONS);
  new FXMenuRadio(arrangemenu,tr("&Big Icons\t\tShow big icons."),filelist,FXFileList::ID_SHOW_BIG_ICONS);
  new FXMenuRadio(arrangemenu,tr("&Details\t\tShow detail view."),filelist,FXFileList::ID_SHOW_DETAILS);
  new FXMenuSeparator(arrangemenu);
  new FXMenuRadio(arrangemenu,tr("&Rows\t\tView row-wise."),filelist,FXFileList::ID_ARRANGE_BY_ROWS);
  new FXMenuRadio(arrangemenu,tr("&Columns\t\tView column-wise."),filelist,FXFileList::ID_ARRANGE_BY_COLUMNS);
  new FXMenuSeparator(arrangemenu);
  new FXMenuRadio(arrangemenu,tr("Mini images"),this,ID_MINI_SIZE);
  new FXMenuRadio(arrangemenu,tr("Normal images"),this,ID_NORMAL_SIZE);
  new FXMenuRadio(arrangemenu,tr("Medium images"),this,ID_MEDIUM_SIZE);
  new FXMenuRadio(arrangemenu,tr("Giant images"),this,ID_GIANT_SIZE);

  // Sort menu
  sortmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Sort"),nullptr,sortmenu);
  new FXMenuRadio(sortmenu,tr("&Name\t\tSort by file name."),filelist,FXFileList::ID_SORT_BY_NAME);
  new FXMenuRadio(sortmenu,tr("&Type\t\tSort by file type."),filelist,FXFileList::ID_SORT_BY_TYPE);
  new FXMenuRadio(sortmenu,tr("&Size\t\tSort by file size."),filelist,FXFileList::ID_SORT_BY_SIZE);
  new FXMenuRadio(sortmenu,tr("&Date\t\tSort by modification time."),filelist,FXFileList::ID_SORT_BY_TIME);
  new FXMenuRadio(sortmenu,tr("&User\t\tSort by user name."),filelist,FXFileList::ID_SORT_BY_USER);
  new FXMenuRadio(sortmenu,tr("&Group\t\tSort by group name."),filelist,FXFileList::ID_SORT_BY_GROUP);
  new FXMenuSeparator(sortmenu);
  new FXMenuCheck(sortmenu,tr("&Reverse\t\tReverse sort direction."),filelist,FXFileList::ID_SORT_REVERSE);
  new FXMenuCheck(sortmenu,tr("&Ignore case\t\tIgnore case of names."),filelist,FXFileList::ID_SORT_CASE);

  // Preferences menu
  prefmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Options"),nullptr,prefmenu);
  new FXMenuCommand(prefmenu,tr("&Preferences...\t\tEdit Preferences."),configicon,this,ID_PREFERENCES);
  new FXMenuSeparator(prefmenu);
  new FXMenuCommand(prefmenu,tr("&Save Settings...\t\tSave current settings."),nullptr,this,ID_SAVE_SETTINGS);

  // View Menu Pane
  viewmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&View"),nullptr,viewmenu);
  new FXMenuCheck(viewmenu,tr("Tree list\t\tShow or hide the tree list."),group1,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(viewmenu,tr("Toolbar\t\tShow or hide tool bar."),toolbar,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(viewmenu,tr("Location bar\t\tShow or hide location bar."),locationbar,FXWindow::ID_TOGGLESHOWN);
  new FXMenuCheck(viewmenu,tr("Status bar\t\tShow or hide status bar."),statusbar,FXWindow::ID_TOGGLESHOWN);
  new FXMenuSeparator(viewmenu);
  new FXMenuCheck(viewmenu,tr("Hidden files\t\tShow hidden files."),this,ID_TOGGLE_HIDDEN);
  new FXMenuCheck(viewmenu,tr("Preview images\t\tPreview images as icons."),filelist,FXFileList::ID_TOGGLE_IMAGES);

  // Help menu
  helpmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,tr("&Help"),nullptr,helpmenu,LAYOUT_RIGHT);
  new FXMenuCommand(helpmenu,tr("&About PathFinder...\t\tDisplay PathFinder About Panel."),nullptr,this,ID_ABOUT,0);

  // Book Mark Menu
  bookmarkmenu=new FXMenuPane(this,POPUP_SHRINKWRAP);
  new FXMenuCommand(bookmarkmenu,tr("Set bookmark\t\tBookmark current directory."),addbookicon,this,ID_BOOKMARK);
  new FXMenuCommand(bookmarkmenu,tr("Unset bookmark\t\tUnset bookmark to current directory."),delbookicon,this,ID_UNBOOKMARK);
  new FXMenuCommand(bookmarkmenu,tr("&Clear\t\tClear bookmarks."),clrbooksicon,&bookmarkeddirs,FXRecentFiles::ID_CLEAR);
  FXMenuSeparator* sep3=new FXMenuSeparator(bookmarkmenu);
  sep3->setTarget(&bookmarkeddirs);
  sep3->setSelector(FXRecentFiles::ID_ANYFILES);
  new FXMenuCommand(bookmarkmenu,"&1",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_1);
  new FXMenuCommand(bookmarkmenu,"&2",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_2);
  new FXMenuCommand(bookmarkmenu,"&3",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_3);
  new FXMenuCommand(bookmarkmenu,"&4",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_4);
  new FXMenuCommand(bookmarkmenu,"&5",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_5);
  new FXMenuCommand(bookmarkmenu,"&6",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_6);
  new FXMenuCommand(bookmarkmenu,"&7",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_7);
  new FXMenuCommand(bookmarkmenu,"&8",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_8);
  new FXMenuCommand(bookmarkmenu,"&9",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_9);
  new FXMenuCommand(bookmarkmenu,"1&0",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_10);
  new FXMenuCommand(bookmarkmenu,"11",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_11);
  new FXMenuCommand(bookmarkmenu,"12",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_12);
  new FXMenuCommand(bookmarkmenu,"13",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_13);
  new FXMenuCommand(bookmarkmenu,"14",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_14);
  new FXMenuCommand(bookmarkmenu,"15",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_15);
  new FXMenuCommand(bookmarkmenu,"16",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_16);
  new FXMenuCommand(bookmarkmenu,"17",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_17);
  new FXMenuCommand(bookmarkmenu,"18",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_18);
  new FXMenuCommand(bookmarkmenu,"19",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_19);
  new FXMenuCommand(bookmarkmenu,"20",nullptr,&bookmarkeddirs,FXRecentFiles::ID_FILE_20);

  // Spacer
  new FXFrame(toolbar,LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,2,0);

  // Directory box
  dirbox=new FXDirBox(toolbar,this,ID_DIRBOX,DIRBOX_NO_OWN_ASSOC|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FIX_WIDTH|LAYOUT_CENTER_Y,0,0,180,0, 0,0, 1,1);
  dirbox->setHelpText(tr("Switch to parent folder."));
  dirbox->setAssociations(associations);
  dirbox->setNumVisible(5);

  // Spacer
  new FXFrame(toolbar,LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,2,0);

  // Add some toolbar buttons
  new FXButton(toolbar,tr("\tUp\tChange up one level."),upicon,this,ID_UPDIRECTORY,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tHome\tChange to home directory."),homeicon,this,ID_GO_HOME,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tWork\tChange to current working directory."),workicon,this,ID_GO_WORK,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXMenuButton(toolbar,tr("\tBookmarks\tVisit bookmarked directories."),bookmarkicon,bookmarkmenu,BUTTON_TOOLBAR|MENUBUTTON_NOARROWS|MENUBUTTON_ATTACH_LEFT|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tBack\tChange to previous directory."),backicon,this,ID_GO_BACKWARD,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tForward\tChange to next directory."),forwicon,this,ID_GO_FORWARD,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);

  // Spacer
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);
  new FXFrame(toolbar,FRAME_SUNKEN|LAYOUT_CENTER_Y|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH,0,0,2,22);
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);

  new FXButton(toolbar,tr("\tMount\tMount device."),mapicon,nullptr,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tUnmount\tUnmount device."),unmapicon,nullptr,0,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);

   // Spacer
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);
  new FXFrame(toolbar,FRAME_SUNKEN|LAYOUT_CENTER_Y|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH,0,0,2,22);
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);

  new FXButton(toolbar,tr("\tNew Directory\tCreate new directory."),newdiricon,this,ID_NEW,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);

  new FXButton(toolbar,tr("\tCut\tCut to clipboard."),cuticon,filelist,FXFileList::ID_CUT_SEL,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tCopy\tCopy to clipboard."),copyicon,filelist,FXFileList::ID_COPY_SEL,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tPaste\tPaste from clipboard."),pasteicon,filelist,FXFileList::ID_PASTE_SEL,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
//  new FXButton(toolbar,tr("\tCopy\tCopy file."),copyicon,this,ID_COPY,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
//  new FXButton(toolbar,tr("\tMove\tMove file."),moveicon,this,ID_MOVE,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
//  new FXButton(toolbar,tr("\tLink\tLink file."),linkicon,this,ID_LINK,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
//  new FXButton(toolbar,tr("\tRename\tRename file."),renameicon,this,ID_RENAME,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);

  // Spacer
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);
  new FXFrame(toolbar,FRAME_SUNKEN|LAYOUT_CENTER_Y|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH,0,0,2,22);
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);

  new FXButton(toolbar,tr("\tPreferences\tDisplay preferences panel."),configicon,this,ID_PREFERENCES,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tProperties\tDisplay file properties."),propicon,this,ID_PROPERTIES,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);

  // Spacer
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);
  new FXFrame(toolbar,FRAME_SUNKEN|LAYOUT_CENTER_Y|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH,0,0,2,22);
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);

  // Switch display modes
  new FXButton(toolbar,tr("\tBig Icons\tShow big icons."),bigiconsicon,filelist,FXFileList::ID_SHOW_BIG_ICONS,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tSmall Icons\tShow small icons."),miniiconsicon,filelist,FXFileList::ID_SHOW_MINI_ICONS,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);
  new FXButton(toolbar,tr("\tDetails\tShow detail view."),detailsicon,filelist,FXFileList::ID_SHOW_DETAILS,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);

  // Spacer
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);
  new FXFrame(toolbar,FRAME_SUNKEN|LAYOUT_CENTER_Y|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH,0,0,2,22);
  new FXFrame(toolbar,LAYOUT_CENTER_Y|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,4,0);

  // Delete button far away
  new FXButton(toolbar,tr("\tDelete\tDelete file."),deleteicon,this,ID_DELETE,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_LEFT);


  // Location bar
  new FXLabel(locationbar,tr("&Location:"),nullptr,LAYOUT_CENTER_Y);
  new FXButton(locationbar,tr("\tClear Location bar\tClear Location bar."),locationicon,this,ID_CLEAR_LOCATION,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y);
  address=new FXTextField(locationbar,10,this,ID_GOTO_LOCATION,TEXTFIELD_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_CENTER_Y,0,0,0,0, 1,1,1,1);
  new FXButton(locationbar,tr("\tGo\tGo to location."),entericon,this,ID_GOTO_LOCATION,BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y);

  // Install some accelerators
  getAccelTable()->addAccel("Back",this,FXSEL(SEL_COMMAND,ID_UPDIRECTORY));
  getAccelTable()->addAccel("Del",this,FXSEL(SEL_COMMAND,ID_DELETE));


  // Make a tool tip
  new FXToolTip(getApp(),0);

  // Recent directories
  visiting=0;

  // Image previewing and blending
  preview=true;
  blending=false;
  scaling=true;

  // Bookmarked directories
  bookmarkeddirs.setMaxFiles(20);
  bookmarkeddirs.setTarget(this);
  bookmarkeddirs.setSelector(ID_GO_RECENT);

  // Set patterns
  setPatternList(fallbackPatterns);
  setCurrentPattern(0);
  }

/*******************************************************************************/

// Make application
void PathFinderMain::create(){
  loadSettings();
  FXMainWindow::create();
  show();
  }


// Close the window, saving settings
FXbool PathFinderMain::close(FXbool notify){
  saveSettings();
  return FXMainWindow::close(notify);
  }


// Change file name
void PathFinderMain::setFilename(const FXString& path){
  FXTRACE((10,"PathFinderMain::setFilename(%s)\n",path.text()));
  filelist->setCurrentFile(path,true);
  dirlist->setDirectory(filelist->getDirectory());
  dirbox->setDirectory(filelist->getDirectory());
  address->setText(filelist->getDirectory());
  }


// Return file name, if any
FXString PathFinderMain::getFilename() const {
  return filelist->getCurrentFile();
  }


// Switch to given directory
void PathFinderMain::setDirectory(const FXString& path){
  FXTRACE((10,"PathFinderMain::setDirectory(%s)\n",path.text()));
  filelist->setDirectory(path,true);
  dirlist->setDirectory(filelist->getDirectory());
  dirbox->setDirectory(filelist->getDirectory());
  address->setText(filelist->getDirectory());
  }


// Get current directory
FXString PathFinderMain::getDirectory() const {
  return filelist->getDirectory();
  }


// Change patterns, each pattern separated by newline
void PathFinderMain::setPatternList(const FXString& patterns){
  FXint current=getCurrentPattern();
  pattern->clearItems();
  pattern->fillItems(patterns);
  if(!pattern->getNumItems()) pattern->appendItem("All Files (*)");
  setCurrentPattern(FXCLAMP(0,current,pattern->getNumItems()-1));
  }


// Return list of patterns
FXString PathFinderMain::getPatternList() const {
  FXString pat;
  for(FXint i=0; i<pattern->getNumItems(); i++){
    if(!pat.empty()) pat+='\n';
    pat+=pattern->getItemText(i);
    }
  return pat;
  }


// Set current pattern
void PathFinderMain::setCurrentPattern(FXint n){
  pattern->setCurrentItem(FXCLAMP(0,n,pattern->getNumItems()-1),true);
  }


// Return current pattern
FXint PathFinderMain::getCurrentPattern() const {
  return pattern->getCurrentItem();
  }


// Visit directory
void PathFinderMain::visitDirectory(const FXString& dir){
  FXint i;
  if(visiting==0){
    for(i=9; i; i--) visiteddir[i]=visiteddir[i-1];
    }
  else{
    for(i=1; i+visiting-1<=9; i++) visiteddir[i]=visiteddir[i+visiting-1];
    for( ; i<10; i++) visiteddir[i]=FXString::null;
    }
  visiteddir[0]=dir;
  visiting=0;
  }


// Have we selected files?
FXbool PathFinderMain::haveSelectedFiles() const {
  if(filelist->getNumItems()){
    for(FXint i=0; i<filelist->getNumItems(); i++){
      if(filelist->isItemSelected(i) && !filelist->isItemNavigational(i)) return true;
      }
    }
  return false;
  }


// Return selected filenames, not including "." and ".."
FXString* PathFinderMain::getSelectedFiles() const {
  FXString* result=nullptr;
  FXint total=0;
  FXint count=0;
  if(filelist->getNumItems()){
    for(FXint i=0; i<filelist->getNumItems(); i++){
      if(filelist->isItemSelected(i) && !filelist->isItemNavigational(i)){
        total++;
        }
      }
    if(0<total){
      result=new FXString [total+1];
      for(FXint i=0; i<filelist->getNumItems(); i++){
        if(filelist->isItemSelected(i) && !filelist->isItemNavigational(i)){
          result[count++]=filelist->getItemPathname(i);
          }
        }
      result[count]=FXString::null;
      }
    }
  return result;
  }

/*******************************************************************************/

// Sort functions
static const FXIconListSortFunc sortfuncs[]={
  FXFileList::ascendingCase,
  FXFileList::descendingCase,
  FXFileList::ascending,
  FXFileList::descending,
  FXFileList::ascendingType,
  FXFileList::descendingType,
  FXFileList::ascendingSize,
  FXFileList::descendingSize,
  FXFileList::ascendingTime,
  FXFileList::descendingTime,
  FXFileList::ascendingUser,
  FXFileList::descendingUser,
  FXFileList::ascendingGroup,
  FXFileList::descendingGroup
  };

/*******************************************************************************/

// Save settings
void PathFinderMain::saveSettings(){
  FXIconListSortFunc sortfunc;
  FXString filter;
  FXString path;
  FXString imagepath;
  FXbool   hiddenfiles;
  FXbool   thumbnails;
  FXuint   iconview;
  FXint    itemspace;
  FXint    thumbnailsize;
  FXuint   index;

  // Save icon search path
  imagepath=associations->getIconPath();
  getApp()->reg().writeStringEntry("SETTINGS","iconpath",imagepath.text());

  // Save file list mode
  iconview=filelist->getListStyle();
  getApp()->reg().writeUIntEntry("SETTINGS","iconview",iconview);

  // Save file list item space
  itemspace=filelist->getItemSpace();
  getApp()->reg().writeIntEntry("SETTINGS","itemspace",itemspace);

  // Showing hidden files...
  hiddenfiles=filelist->showHiddenFiles();
  getApp()->reg().writeBoolEntry("SETTINGS","hiddenfiles",hiddenfiles);

  // Showing thumbnails...
  thumbnails=filelist->showImages();
  getApp()->reg().writeBoolEntry("SETTINGS","thumbnails",thumbnails);

  // Save file list item space
  thumbnailsize=filelist->getImageSize();
  getApp()->reg().writeIntEntry("SETTINGS","thumbnailsize",thumbnailsize);

  // Write new window size back to registry
  getApp()->reg().writeIntEntry("SETTINGS","x",getX());
  getApp()->reg().writeIntEntry("SETTINGS","y",getY());
  getApp()->reg().writeIntEntry("SETTINGS","width",getWidth());
  getApp()->reg().writeIntEntry("SETTINGS","height",getHeight());

  // Width of tree
  getApp()->reg().writeIntEntry("SETTINGS","dirwidth",splitter->getSplit(0));

  // Header sizes
  getApp()->reg().writeIntEntry("SETTINGS","nameheader",filelist->getHeaderSize(0));
  getApp()->reg().writeIntEntry("SETTINGS","typeheader",filelist->getHeaderSize(1));
  getApp()->reg().writeIntEntry("SETTINGS","sizeheader",filelist->getHeaderSize(2));
  getApp()->reg().writeIntEntry("SETTINGS","dateheader",filelist->getHeaderSize(3));
  getApp()->reg().writeIntEntry("SETTINGS","userheader",filelist->getHeaderSize(4));
  getApp()->reg().writeIntEntry("SETTINGS","attrheader",filelist->getHeaderSize(5));

  // Visited directories
  getApp()->reg().writeStringEntry("Visited Directories","0",visiteddir[0].text());
  getApp()->reg().writeStringEntry("Visited Directories","1",visiteddir[1].text());
  getApp()->reg().writeStringEntry("Visited Directories","2",visiteddir[2].text());
  getApp()->reg().writeStringEntry("Visited Directories","3",visiteddir[3].text());
  getApp()->reg().writeStringEntry("Visited Directories","4",visiteddir[4].text());
  getApp()->reg().writeStringEntry("Visited Directories","5",visiteddir[5].text());
  getApp()->reg().writeStringEntry("Visited Directories","6",visiteddir[6].text());
  getApp()->reg().writeStringEntry("Visited Directories","7",visiteddir[7].text());
  getApp()->reg().writeStringEntry("Visited Directories","8",visiteddir[8].text());
  getApp()->reg().writeStringEntry("Visited Directories","9",visiteddir[9].text());

  // Visiting
  getApp()->reg().writeIntEntry("Visited Directories","visiting",visiting);

  // Editor command
  getApp()->reg().writeStringEntry("SETTINGS","editor",editor.text());


  // Exec paths
  getApp()->reg().writeStringEntry("SETTINGS","execpaths",execpaths.text());

  // Terminal command
  getApp()->reg().writeStringEntry("SETTINGS","terminal",terminal.text());

  // Program last ran
  getApp()->reg().writeStringEntry("SETTINGS","program",program.text());

  // Preview
  getApp()->reg().writeBoolEntry("SETTINGS","preview",preview);
  getApp()->reg().writeBoolEntry("SETTINGS","blending",blending);
  getApp()->reg().writeBoolEntry("SETTINGS","scaling",scaling);

  // Toolbar, location bar, status bar shown
  getApp()->reg().writeBoolEntry("SETTINGS","toolbar",toolbar->shown());
  getApp()->reg().writeBoolEntry("SETTINGS","locationbar",locationbar->shown());
  getApp()->reg().writeBoolEntry("SETTINGS","statusbar",statusbar->shown());

  // File patterns
  getApp()->reg().writeIntEntry("SETTINGS","filepatternno",getCurrentPattern());
  getApp()->reg().writeStringEntry("SETTINGS","filepatterns",getPatternList().text());

  // Sort function
  sortfunc=filelist->getSortFunc();
  for(index=ARRAYNUMBER(sortfuncs)-1; index; index--){ if(sortfuncs[index]==sortfunc) break; }
  getApp()->reg().writeIntEntry("SETTINGS","sorting",index);

  // Save pathfinder directory
  path=filelist->getDirectory();
  getApp()->reg().writeStringEntry("SETTINGS","directory",path.text());
  }

/*******************************************************************************/

// Load settings
void PathFinderMain::loadSettings(){
  FXint    ww,hh,xx,yy;
  FXString path;
  FXString filter;
  FXString imagepath;
  FXuint   iconview;
  FXint    thumbnailsize;
  FXint    itemspace;
  FXbool   hiddenfiles;
  FXbool   thumbnails;
  FXbool   tbshown;
  FXbool   lbshown;
  FXbool   sbshown;
  FXuint   sortfunc;

  // Get icon search path
  imagepath=getApp()->reg().readStringEntry("SETTINGS","iconpath",FXIconCache::defaultIconPath);
  associations->setIconPath(imagepath);

  // Read icon view mode
  iconview=getApp()->reg().readUIntEntry("SETTINGS","iconview",ICONLIST_MINI_ICONS|ICONLIST_AUTOSIZE);
  filelist->setListStyle(iconview);

  // Icon space
  itemspace=getApp()->reg().readIntEntry("SETTINGS","itemspace",200);
  filelist->setItemSpace(itemspace);

  // Showing hidden files...
  hiddenfiles=getApp()->reg().readBoolEntry("SETTINGS","hiddenfiles",false);
  filelist->showHiddenFiles(hiddenfiles,true);
  dirlist->showHiddenFiles(hiddenfiles);

  // Showing thumbnails...
  thumbnails=getApp()->reg().readBoolEntry("SETTINGS","thumbnails",false);
  filelist->showImages(thumbnails,true);

  // Showing thumbnails...
  thumbnailsize=getApp()->reg().readIntEntry("SETTINGS","thumbnailsize",32);
  filelist->setImageSize(thumbnailsize,true);

  // Get size
  xx=getApp()->reg().readIntEntry("SETTINGS","x",100);
  yy=getApp()->reg().readIntEntry("SETTINGS","y",100);
  ww=getApp()->reg().readIntEntry("SETTINGS","width",800);
  hh=getApp()->reg().readIntEntry("SETTINGS","height",600);

  setX(xx);
  setY(yy);
  setWidth(ww);
  setHeight(hh);

  // Set tree width
  splitter->setSplit(0,getApp()->reg().readIntEntry("SETTINGS","dirwidth",200));

  // Header sizes
  filelist->setHeaderSize(0,getApp()->reg().readIntEntry("SETTINGS","nameheader",200));
  filelist->setHeaderSize(1,getApp()->reg().readIntEntry("SETTINGS","typeheader",100));
  filelist->setHeaderSize(2,getApp()->reg().readIntEntry("SETTINGS","sizeheader",60));
  filelist->setHeaderSize(3,getApp()->reg().readIntEntry("SETTINGS","dateheader",150));
  filelist->setHeaderSize(4,getApp()->reg().readIntEntry("SETTINGS","userheader",50));
  filelist->setHeaderSize(5,getApp()->reg().readIntEntry("SETTINGS","attrheader",60));

  // Visited directories
  visiteddir[0]=getApp()->reg().readStringEntry("Visited Directories","0",FXString::null);
  visiteddir[1]=getApp()->reg().readStringEntry("Visited Directories","1",FXString::null);
  visiteddir[2]=getApp()->reg().readStringEntry("Visited Directories","2",FXString::null);
  visiteddir[3]=getApp()->reg().readStringEntry("Visited Directories","3",FXString::null);
  visiteddir[4]=getApp()->reg().readStringEntry("Visited Directories","4",FXString::null);
  visiteddir[5]=getApp()->reg().readStringEntry("Visited Directories","5",FXString::null);
  visiteddir[6]=getApp()->reg().readStringEntry("Visited Directories","6",FXString::null);
  visiteddir[7]=getApp()->reg().readStringEntry("Visited Directories","7",FXString::null);
  visiteddir[8]=getApp()->reg().readStringEntry("Visited Directories","8",FXString::null);
  visiteddir[9]=getApp()->reg().readStringEntry("Visited Directories","9",FXString::null);

  // Visiting
  visiting=getApp()->reg().readIntEntry("Visited Directories","visiting",0);

  // Editor command
  editor=getApp()->reg().readStringEntry("SETTINGS","editor","adie");

  // Exec paths; if not set, use the system-provided
  execpaths=getApp()->reg().readStringEntry("SETTINGS","execpaths",FXSystem::getExecPath().text());

  // Terminal command
  terminal=getApp()->reg().readStringEntry("SETTINGS","terminal","xterm");

  // Program last ran
  program=getApp()->reg().readStringEntry("SETTINGS","program","adie");

  // Preview
  preview=getApp()->reg().readBoolEntry("SETTINGS","preview",true);
  blending=getApp()->reg().readBoolEntry("SETTINGS","blending",false);
  scaling=getApp()->reg().readBoolEntry("SETTINGS","scaling",true);

  // Toolbar, location bar, status bar shown
  tbshown=getApp()->reg().readBoolEntry("SETTINGS","toolbar",true);
  lbshown=getApp()->reg().readBoolEntry("SETTINGS","locationbar",false);
  sbshown=getApp()->reg().readBoolEntry("SETTINGS","statusbar",true);
  if(!tbshown) toolbar->hide();
  if(!lbshown) locationbar->hide();
  if(!sbshown) statusbar->hide();

  // File patterns
  setPatternList(getApp()->reg().readStringEntry("SETTINGS","filepatterns",fallbackPatterns));
  setCurrentPattern(getApp()->reg().readIntEntry("SETTINGS","filepatternno",0));

  // Sort function
  sortfunc=getApp()->reg().readIntEntry("SETTINGS","sorting",0);
  if(sortfunc>=ARRAYNUMBER(sortfuncs)) sortfunc=0;
  filelist->setSortFunc(sortfuncs[sortfunc]);

  // Read last path setting
  path=getApp()->reg().readStringEntry("SETTINGS","directory","~");

  // Jump to there
  setDirectory(FXPath::expand(path));
  }


/*******************************************************************************/

// Enable sender when items have been selected
long PathFinderMain::onUpdSelected(FXObject* sender,FXSelector,void*){
  sender->handle(this,haveSelectedFiles()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Make sender visible when items have been selected
long PathFinderMain::onUpdSelectedVisible(FXObject* sender,FXSelector,void*){
  sender->handle(this,haveSelectedFiles()?FXSEL(SEL_COMMAND,ID_SHOW):FXSEL(SEL_COMMAND,ID_HIDE),nullptr);
  return 1;
  }


// Run program from given command line
FXbool PathFinderMain::executeCommandline(const FXString& commandline){
  FXchar **argv=nullptr;

  FXTRACE((10,"PathFinderMain::executeCommandline(%s)\n",commandline.text()));

  // Parse commandline into argv
  if(FXPath::parseArgs(argv,commandline)){
    FXbool result=false;

    // Find the executable path in the list
    FXString command=FXPath::search(execpaths,argv[0]);
    if(!command.empty()){

      // Old directory
      FXString cwd=FXSystem::getCurrentDirectory();

      // Switch to new directory
      if(FXSystem::setCurrentDirectory(getDirectory())){
        FXProcess process;

        // Start the program with arguments
        result=process.start(command.text(),argv,nullptr);

        // Failed to start the program
        if(!result){
          FXMessageBox::error(this,MBOX_OK,tr("Unable to Start"),tr("Unable to start program: '%s'.\n"),commandline.text());
          }

        // Switch back to old directory
        FXSystem::setCurrentDirectory(cwd);
        }

      // Couldn't change working directory
      else{
        FXMessageBox::error(this,MBOX_OK,tr("Unable to Change Directory"),tr("Unable to change directories.\n"));
        }
      }

    // Could not find executable path
    else{
      FXMessageBox::error(this,MBOX_OK,tr("Unknown Program"),tr("Unable to find program: '%s'.\n"),argv[0]);
      }

    // Done with that
    freeElms(argv);
    return result;
    }

  // Could not parse commandline
  FXMessageBox::error(this,MBOX_OK,tr("Bad Command or Filename"),tr("Syntax error in commandline: '%s'.\n"),commandline.text());
  return false;
  }


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
FXString PathFinderMain::makeCommandline(const FXString& cmd) const {
  FXString *files=getSelectedFiles();
  FXString result;
  FXint s=0,q=0,f;
  if(files){
    if(!files[0].empty()){
      while(q<cmd.length()){
        if(cmd[q++]=='%'){
          result.append(&cmd[s],q-s-1);
          switch(cmd[q++]){
          case 'f':         // One file
            result.append(' ');
            result.append(FXPath::enquote(files[0]));
            break;
          case 'F':         // Multiple files
            f=0;
            while(!files[f].empty()){
              result.append(' ');
              result.append(FXPath::enquote(files[f]));
              f++;
              }
            break;
          case 'n':         // Name only
            result.append(' ');
            result.append(FXPath::enquote(FXPath::name(files[0])));
            break;
          case 'N':         // Multiple files, names only
            f=0;
            while(!files[f].empty()){
              result.append(' ');
              result.append(FXPath::enquote(FXPath::name(files[f])));
              f++;
              }
            break;
          case 'u':         // URL-ified filename
            result.append(' ');
            result.append(FXPath::enquote(FXURL::fileToURL(files[0])));
            break;
          case 'U':         // URL-ified multiple filenames
            f=0;
            while(!files[f].empty()){
              result.append(' ');
              result.append(FXPath::enquote(FXURL::fileToURL(files[f])));
              f++;
              }
            break;
          case 'd':         // Current working directory
            result.append(' ');
            result.append(FXPath::enquote(getDirectory()));
            break;
          case '%':         // We wanted a '%'
            result.append('%');
            break;
            }
          s=q;
          }
        }
      result.append(&cmd[s],q-s);
      }
    delete [] files;
    }
  return result;
  }


// Open
long PathFinderMain::onCmdOpen(FXObject*,FXSelector,void*){
  FXint index=filelist->getCurrentItem();
  if(0<=index){

    // Pathname of item
    FXString pathname=filelist->getItemPathname(index);

    // If directory, open the directory
    if(filelist->isItemDirectory(index)){
      setDirectory(pathname);
      visitDirectory(pathname);
      }

    // If executable, execute it!
    else if(filelist->isItemExecutable(index)){
      executeCommandline(pathname);
      }

    // If regular file return as the selected file
    else if(filelist->isItemFile(index)){
      FXFileAssoc *association=filelist->getItemAssoc(index);
      if(association){
        if(!association->command.empty()){
          FXString commandline=makeCommandline(association->command);
          executeCommandline(commandline);
          }
        else{
          FXMessageBox::information(this,MBOX_OK,tr("Unknown Command"),tr("No command defined for file: %s."),pathname.text());
          }
        }
      else{
        FXMessageBox::information(this,MBOX_OK,tr("Unknown File Type"),tr("No association has been set for file: %s."),pathname.text());
        }
      }
    }
  return 1;
  }


// File Item was double-clicked
long PathFinderMain::onFileDblClicked(FXObject*,FXSelector,void* ptr){
  FXint index=(FXint)(FXival)ptr;
  if(0<=index){

    // Pathname of item
    FXString pathname=filelist->getItemPathname(index);

    // If directory, open the directory
    if(filelist->isItemDirectory(index)){
      setDirectory(pathname);
      visitDirectory(pathname);
      }

    // If executable, execute it!
    else if(filelist->isItemExecutable(index)){
      executeCommandline(pathname);
      }

    // If regular file return as the selected file
    else if(filelist->isItemFile(index)){

      // Load image if preview mode on
      if(preview){
        if(previewImage(pathname)) return 1;
        }

      // Get association
      FXFileAssoc *association=filelist->getItemAssoc(index);
      if(association){
        if(!association->command.empty()){
          FXString commandline=makeCommandline(association->command);
          executeCommandline(commandline);
          }
        else{
          FXMessageBox::information(this,MBOX_OK,tr("Unknown Command"),tr("No command defined for file: %s."),pathname.text());
          }
        }
      else{
        FXMessageBox::information(this,MBOX_OK,tr("Unknown File Type"),tr("No association has been set for file: %s."),pathname.text());
        }
      }
    }
  return 1;
  }


// Selected an item
long PathFinderMain::onFileSelected(FXObject*,FXSelector,void* ptr){
  FXint index=(FXint)(FXival)ptr;
  FXuint mode=filelist->getItemMode(index);
  FXlong size=filelist->getItemSize(index);
  selectedModeBits[ 0]+=FXBIT(mode,0);
  selectedModeBits[ 1]+=FXBIT(mode,1);
  selectedModeBits[ 2]+=FXBIT(mode,2);
  selectedModeBits[ 3]+=FXBIT(mode,3);
  selectedModeBits[ 4]+=FXBIT(mode,4);
  selectedModeBits[ 5]+=FXBIT(mode,5);
  selectedModeBits[ 6]+=FXBIT(mode,6);
  selectedModeBits[ 7]+=FXBIT(mode,7);
  selectedModeBits[ 8]+=FXBIT(mode,8);
  selectedModeBits[13]+=FXBIT(mode,13); // SUID
  selectedModeBits[14]+=FXBIT(mode,14); // SGID
  selectedModeBits[15]+=FXBIT(mode,15); // SVTX
  selectedSpace+=size;
  selectedCount+=1;
  FXTRACE((10,"selected  : %3d %7lld [%3u %7lld] %s\n",index,size,selectedCount,selectedSpace,filelist->getItemFilename(index).text()));
  return 1;
  }


// Deselected an item
long PathFinderMain::onFileDeselected(FXObject*,FXSelector,void* ptr){
  FXint index=(FXint)(FXival)ptr;
  FXuint mode=filelist->getItemMode(index);
  FXlong size=filelist->getItemSize(index);
  selectedModeBits[ 0]-=FXBIT(mode,0);
  selectedModeBits[ 1]-=FXBIT(mode,1);
  selectedModeBits[ 2]-=FXBIT(mode,2);
  selectedModeBits[ 3]-=FXBIT(mode,3);
  selectedModeBits[ 4]-=FXBIT(mode,4);
  selectedModeBits[ 5]-=FXBIT(mode,5);
  selectedModeBits[ 6]-=FXBIT(mode,6);
  selectedModeBits[ 7]-=FXBIT(mode,7);
  selectedModeBits[ 8]-=FXBIT(mode,8);
  selectedModeBits[13]-=FXBIT(mode,13); // SUID
  selectedModeBits[14]-=FXBIT(mode,14); // SGID
  selectedModeBits[15]-=FXBIT(mode,15); // SVTX
  selectedSpace-=size;
  selectedCount-=1;
  FXTRACE((10,"deselected: %3d %7lld [%3d %7lld] %s\n",index,size,selectedCount,selectedSpace,filelist->getItemFilename(index).text()));
  return 1;
  }


// Inserted new item
long PathFinderMain::onFileInserted(FXObject* sender,FXSelector sel,void* ptr){
  FXint index=(FXint)(FXival)ptr;
  if(filelist->isItemSelected(index)){
    onFileSelected(sender,sel,ptr);
    }
  totalSpace+=filelist->getItemSize(index);
  totalCount+=1;
  FXTRACE((10,"inserted  : %3d %7lld [%3d %7lld] %s\n",index,filelist->getItemSize(index),totalCount,totalSpace,filelist->getItemFilename(index).text()));
  return 1;
  }


// Deleted an item
long PathFinderMain::onFileDeleted(FXObject* sender,FXSelector sel,void* ptr){
  FXint index=(FXint)(FXival)ptr;
  if(filelist->isItemSelected(index)){
    onFileDeselected(sender,sel,ptr);
    }
  totalSpace-=filelist->getItemSize(index);
  totalCount-=1;
  FXTRACE((10,"deleted   : %3d %7lld [%3d %7lld] %s\n",index,filelist->getItemSize(index),totalCount,totalSpace,filelist->getItemFilename(index).text()));
  return 1;
  }


// Goto location entered into the text field; a relative path or
// a path containing environment variable expansions is good too.
long PathFinderMain::onCmdGotoLocation(FXObject*,FXSelector,void*){

  // Get given path, relative to current directory if not absolute
  FXString path=FXPath::absolute(getDirectory(),FXPath::expand(address->getText()));

  // Move to this existing directory
  setDirectory(path);

  // Mark this directory
  visitDirectory(path);

  // Close preview
  closePreview();
  return 1;
  }


// Clear location bar
long PathFinderMain::onCmdClearLocation(FXObject*,FXSelector,void*){
  address->setText(FXString::null);
  return 1;
  }


// Popup menu for item in file list
long PathFinderMain::onFilePopup(FXObject*,FXSelector,void* ptr){
  FXEvent *event=(FXEvent*)ptr;
  if(!event->moved){
    FXMenuPane pane(this);
    new FXMenuCommand(&pane,tr("&Up\t\tChange up one level."),upicon,this,ID_UPDIRECTORY);
    new FXMenuCommand(&pane,tr("&Home\t\tChange to home directory."),homeicon,this,ID_GO_HOME);
    new FXMenuCommand(&pane,tr("&Work\t\tChange to current working directory."),workicon,this,ID_GO_WORK);
    new FXMenuCommand(&pane,tr("&Back\t\tChange to previous directory."),backicon,this,ID_GO_BACKWARD);
    new FXMenuCommand(&pane,tr("&Forward\t\tChange to next directory."),forwicon,this,ID_GO_FORWARD);
    new FXMenuSeparator(&pane);
    FXMenuPane openmenu(this);
    new FXMenuCascade(&pane,tr("Open with"),execicon,&openmenu);
    new FXMenuCommand(&openmenu,tr("Open..."),nullptr,this,ID_OPEN);
    new FXMenuCommand(&openmenu,tr("Open with..."),nullptr,this,ID_OPEN_WITH);
    new FXMenuCommand(&openmenu,tr("Open with editor"),nullptr,this,ID_OPEN_WITH_EDITOR);
    new FXMenuCascade(&pane,tr("Bookmarks"),bookmarkicon,bookmarkmenu);
    new FXMenuCascade(&pane,tr("Sort by"),sortingicon,sortmenu);
    new FXMenuCascade(&pane,tr("View"),bigiconsicon,arrangemenu);
    new FXMenuSeparator(&pane);
    new FXMenuCommand(&pane,tr("New directory..."),newdiricon,this,ID_NEW);
    new FXMenuCommand(&pane,tr("Rename"),renameicon,this,ID_RENAME);
    new FXMenuCommand(&pane,tr("Copy"),copyicon,this,ID_COPY);
    new FXMenuCommand(&pane,tr("Move"),moveicon,this,ID_MOVE);
    new FXMenuCommand(&pane,tr("Link"),linkicon,this,ID_LINK);
    new FXMenuCommand(&pane,tr("Delete"),deleteicon,this,ID_DELETE);
    pane.create();
    pane.popup(nullptr,event->root_x,event->root_y);
    getApp()->runModalWhileShown(&pane);
    }
  return 1;
  }


// About
long PathFinderMain::onCmdAbout(FXObject*,FXSelector,void*){
  FXDialogBox about(this,tr("About PathFinder"),DECOR_TITLE|DECOR_BORDER,0,0,0,0, 0,0,0,0, 0,0);
  FXGIFIcon picture(getApp(),foxbig);
  new FXLabel(&about,FXString::null,&picture,FRAME_GROOVE|LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y|JUSTIFY_CENTER_X|JUSTIFY_CENTER_Y,0,0,0,0, 0,0,0,0);
  FXVerticalFrame* side=new FXVerticalFrame(&about,LAYOUT_SIDE_RIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 10,10,10,10, 0,0);
  new FXLabel(side,"PathFinder",nullptr,JUSTIFY_LEFT|ICON_BEFORE_TEXT|LAYOUT_FILL_X);
  new FXHorizontalSeparator(side,SEPARATOR_LINE|LAYOUT_FILL_X);
  new FXLabel(side,FXString::value(tr("\nPathFinder File Manager, version %d.%d.%d.\n\nPathFinder is a simple and speedy file manager with drag and drop support.\n\nUsing The FOX Toolkit (www.fox-toolkit.org), version %d.%d.%d (%s).\nCopyright (C) 2000,2023 Jeroen van der Zijp (jeroen@fox-toolkit.net).\n "),VERSION_MAJOR,VERSION_MINOR,VERSION_PATCH,FOX_MAJOR,FOX_MINOR,FOX_LEVEL,__DATE__),nullptr,JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXButton *button=new FXButton(side,tr("&OK"),nullptr,&about,FXDialogBox::ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT,0,0,0,0,32,32,2,2);
  button->setFocus();
  about.execute(PLACEMENT_OWNER);
  return 1;
  }


// Change the directory from the FXDirList
long PathFinderMain::onCmdDirectory(FXObject*,FXSelector,void* ptr){
  FXTreeItem *item=(FXTreeItem*)ptr;
  FXString path=dirlist->getItemPathname(item);

#if defined(DIRWATCH)
dirwatch.remWatch(filelist->getDirectory());           // FIXME
#endif

  filelist->setDirectory(path,true);

#if defined(DIRWATCH)
dirwatch.addWatch(filelist->getDirectory());           // FIXME
#endif

  dirbox->setDirectory(filelist->getDirectory());
  address->setText(filelist->getDirectory());
  visitDirectory(filelist->getDirectory());
  closePreview();
  return 1;
  }


// Change the directory from the FXDirBox
long PathFinderMain::onCmdDirTree(FXObject*,FXSelector,void*){
  FXString path=dirbox->getDirectory();
  filelist->setDirectory(path,true);
  dirlist->setDirectory(filelist->getDirectory());
  address->setText(filelist->getDirectory());
  visitDirectory(filelist->getDirectory());
  closePreview();
  return 1;
  }


// User clicked up directory button; we move to the next higher directory,
// and select the directory we just came from in that directory; this allows
// a quick jump back into the original directory in case we went up too far.
long PathFinderMain::onCmdUpDirectory(FXObject*,FXSelector,void*){
  FXString path=getDirectory();
  setFilename(path);
  visitDirectory(getDirectory());
  closePreview();
  return 1;
  }


// Move to home directory
long PathFinderMain::onCmdHomeDirectory(FXObject*,FXSelector,void*){
  FXString path=FXSystem::getHomeDirectory();
  setDirectory(path);
  visitDirectory(path);
  closePreview();
  return 1;
  }


// Move to work directory
long PathFinderMain::onCmdWorkDirectory(FXObject*,FXSelector,void*){
  FXString path=FXSystem::getCurrentDirectory();
  setDirectory(path);
  visitDirectory(path);
  closePreview();
  return 1;
  }


// Bookmark this directory
long PathFinderMain::onCmdBookmark(FXObject*,FXSelector,void*){
  bookmarkeddirs.appendFile(filelist->getDirectory());
  return 1;
  }


// Bookmark this directory
long PathFinderMain::onCmdUnBookmark(FXObject*,FXSelector,void*){
  bookmarkeddirs.removeFile(filelist->getDirectory());
  return 1;
  }


// Move to recent directory
long PathFinderMain::onCmdRecentDirectory(FXObject*,FXSelector,void* ptr){
  FXString path((const FXchar*)ptr);
  if(FXStat::exists(path)){
    setDirectory(path);
    visitDirectory(path);
    closePreview();
    return 1;
    }
  bookmarkeddirs.removeFile(path);      // No longer exists; remove it!
  getApp()->beep();
  return 1;
  }


// Move to previous directory
long PathFinderMain::onCmdBackDirectory(FXObject*,FXSelector,void*){
  if(visiting<9 && !visiteddir[visiting+1].empty()){
    setDirectory(visiteddir[++visiting]);
    closePreview();
    }
  return 1;
  }


// Update move to previous directory
long PathFinderMain::onUpdBackDirectory(FXObject* sender,FXSelector,void*){
  sender->handle(this,(visiting<9 && !visiteddir[visiting+1].empty())?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Move to next directory
long PathFinderMain::onCmdForwardDirectory(FXObject*,FXSelector,void*){
  if(0<visiting){
    setDirectory(visiteddir[--visiting]);
    closePreview();
    }
  return 1;
  }


// Update move to next directory
long PathFinderMain::onUpdForwardDirectory(FXObject* sender,FXSelector,void*){
  sender->handle(this,(0<visiting)?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Update files heading
long PathFinderMain::onUpdFiles(FXObject* sender,FXSelector,void*){
  FXString string=tr("Files in ") + filelist->getDirectory();
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&string);
  return 1;
  }


// Update main window title and other stuff
long PathFinderMain::onUpdate(FXObject* sender,FXSelector sel,void* ptr){
  FXMainWindow::onUpdate(sender,sel,ptr);
  FXString ttl="PathFinder:- " + filelist->getDirectory();
  setTitle(ttl);
  return 1;
  }

/*******************************************************************************/

// Update diskspace
long PathFinderMain::onUpdDiskSpace(FXObject* sender,FXSelector,void*){
  FXulong totalspace,availspace;
  if(FXStat::getTotalDiskSpace(getDirectory(),totalspace) && FXStat::getAvailableDiskSpace(getDirectory(),availspace)){
    FXString space;
    space.format("Free: %.4lgGB / %.4lgGB",1.0E-9*availspace,1.0E-9*totalspace);
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SHOW),nullptr);
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&space);
    space.format("Free: %'llu / Total: %'llu Bytes",availspace,totalspace);
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETTIPSTRING),(void*)&space);
    return 1;
    }
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_HIDE),nullptr);
  return 1;
  }


// Update status line to show some info about the icon the cursor is over
long PathFinderMain::onUpdStatusline(FXObject* sender,FXSelector,void*){
  FXint currentitem=filelist->getCurrentItem();
  FXString info;
  if(1==selectedCount && 0<=currentitem && filelist->isItemSelected(currentitem)){
    if(filelist->isItemDirectory(currentitem)){
      if(filelist->isItemSymlink(currentitem)){
        info.format(tr("Folder \"%s\" -> \"%s\"."),filelist->getItemFilename(currentitem).text(),filelist->getItemText(currentitem).section('\t',7).text());
        }
      else{
        info.format(tr("Folder \"%s\"."),filelist->getItemFilename(currentitem).text());
        }
      }
    else{
      if(filelist->isItemSymlink(currentitem)){
        info.format(tr("File \"%s\" -> \"%s\"."),filelist->getItemFilename(currentitem).text(),filelist->getItemText(currentitem).section('\t',7).text());
        }
      else{
        info.format(tr("File \"%s\" [%s] (%'lld bytes)."),filelist->getItemFilename(currentitem).text(),filelist->getItemText(currentitem).section('\t',1).text(),filelist->getItemSize(currentitem));
        }
      }
    }
  else if(1<=selectedCount){
    info.format(tr("Selected %d items (%'lld bytes)."),selectedCount,selectedSpace);
    }
  else{
    info.format(tr("Total %d items (%'lld bytes)."),totalCount,totalSpace);
    }
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&info);
  return 1;
  }

/*******************************************************************************/

// Save settings to disk
long PathFinderMain::onCmdSaveSettings(FXObject*,FXSelector,void*){
  saveSettings();
  getApp()->reg().write();
  return 1;
  }


// Spawn new PathFinder
long PathFinderMain::onCmdNewPathFinder(FXObject*,FXSelector,void*){
  saveSettings();
  getApp()->reg().write();
  executeCommandline(PATHFINDEREXEC);
  return 1;
  }


/*******************************************************************************/

// Create new directory
long PathFinderMain::onCmdNew(FXObject*,FXSelector,void*){
  FXBMPIcon newfoldericon(getApp(),newfolder,0,IMAGE_ALPHAGUESS);
  FXString name(tr("folder"));
  if(FXInputDialog::getString(name,this,tr("Create New Directory"),tr("Create new directory with name: "),&newfoldericon)){
    FXString dirname=FXPath::absolute(getDirectory(),name);
    if(FXStat::exists(dirname)){
      FXMessageBox::error(this,MBOX_OK,tr("Already Exists"),tr("File or directory %s already exists.\n"),dirname.text());
      return 1;
      }
    if(!FXDir::create(dirname)){
      FXMessageBox::error(this,MBOX_OK,tr("Cannot Create"),tr("Cannot create directory %s.\n"),dirname.text());
      return 1;
      }
    }
  return 1;
  }


// Update create new directory
long PathFinderMain::onUpdNew(FXObject* sender,FXSelector,void*){
  FXString path=getDirectory();
  if(FXStat::isAccessible(path,FXIO::Writing))
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),nullptr);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Copy file or directory
long PathFinderMain::onCmdCopy(FXObject*,FXSelector,void*){
  FXString *filenamelist=getSelectedFiles();
  if(filenamelist){
    CopyDialog copydialog(this,tr("Copy File"));
    FXString newname;
    for(FXint i=0; !filenamelist[i].empty(); i++){
      copydialog.setOldName(filenamelist[i]);
      copydialog.setNewName(FXPath::absolute(FXPath::directory(filenamelist[i]),"CopyOf"+FXPath::name(filenamelist[i])));
      if(!copydialog.execute()) break;
      newname=copydialog.getNewName();
      if(!FXFile::copyFiles(filenamelist[i],newname,false)){
        if(FXMessageBox::error(this,MBOX_YES_NO,tr("Error Copying File"),tr("Unable to copy file:\n\n%s  to:  %s\n\nContinue with operation?"),filenamelist[i].text(),newname.text())==MBOX_CLICKED_NO) break;
        }
      }
    delete [] filenamelist;
    }
  return 1;
  }


// Move file or directory
long PathFinderMain::onCmdMove(FXObject*,FXSelector,void*){
  FXString *filenamelist=getSelectedFiles();
  if(filenamelist){
    CopyDialog copydialog(this,tr("Move File"));
    FXString newname;
    for(FXint i=0; !filenamelist[i].empty(); i++){
      copydialog.setOldName(filenamelist[i]);
      copydialog.setNewName(filenamelist[i]);
      if(!copydialog.execute()) break;
      newname=copydialog.getNewName();
      if(!FXFile::moveFiles(filenamelist[i],newname,false)){
	if(FXMessageBox::error(this,MBOX_YES_NO,tr("Error Moving File"),tr("Unable to move file:\n\n%s  to:  %s\n\nContinue with operation?"),filenamelist[i].text(),newname.text())==MBOX_CLICKED_NO) break;
	}
      }
    delete [] filenamelist;
    }
  return 1;
  }


// Link file
long PathFinderMain::onCmdLink(FXObject*,FXSelector,void*){
  FXString *filenamelist=getSelectedFiles();
  if(filenamelist){
    CopyDialog copydialog(this,tr("Link File"));
    FXString newname;
    for(FXint i=0; !filenamelist[i].empty(); i++){
      copydialog.setOldName(filenamelist[i]);
      copydialog.setNewName(FXPath::absolute(FXPath::directory(filenamelist[i]),"LinkTo"+FXPath::name(filenamelist[i])));
      if(!copydialog.execute()) break;
      newname=copydialog.getNewName();
      if(!FXFile::symlink(filenamelist[i],newname)){
	if(FXMessageBox::error(this,MBOX_YES_NO,tr("Error Linking File"),tr("Unable to link file:\n\n%s  to:  %s\n\nContinue with operation?"),filenamelist[i].text(),newname.text())==MBOX_CLICKED_NO) break;
	}
      }
    delete [] filenamelist;
    }
  return 1;
  }


// Rename file or directory
long PathFinderMain::onCmdRename(FXObject*,FXSelector,void*){
  FXString *filenamelist=getSelectedFiles();
  if(filenamelist){
    CopyDialog copydialog(this,tr("Rename File"));
    FXString newname;
    for(FXint i=0; !filenamelist[i].empty(); i++){
      copydialog.setOldName(FXPath::name(filenamelist[i]));
      copydialog.setNewName(FXPath::name(filenamelist[i]));
      if(!copydialog.execute()) break;
      newname=copydialog.getNewName();
      if(!FXFile::moveFiles(filenamelist[i],FXPath::absolute(FXPath::directory(filenamelist[i]),newname),false)){
	if(FXMessageBox::error(this,MBOX_YES_NO,tr("Error Renaming File"),tr("Unable to rename file:\n\n%s  to:  %s\n\nContinue with operation?"),filenamelist[i].text(),newname.text())==MBOX_CLICKED_NO) break;
	}
      }
    delete [] filenamelist;
    }
  return 1;
  }


// Delete file or directory
long PathFinderMain::onCmdDelete(FXObject*,FXSelector,void*){
  FXString *filenamelist=getSelectedFiles();
  if(filenamelist){
    FXDialogBox deletedialog(this,tr("Deleting Files"),DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE,0,0,500,0);
    FXString filename;
    FXHorizontalFrame* buttons=new FXHorizontalFrame(&deletedialog,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0,0,0,0,0);
    new FXButton(buttons,tr("&Delete"),nullptr,&deletedialog,FXDialogBox::ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_RIGHT,0,0,0,0,20,20,2,2);
    new FXButton(buttons,tr("&Cancel"),nullptr,&deletedialog,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_RIGHT,0,0,0,0,20,20,2,2);
    new FXHorizontalSeparator(&deletedialog,SEPARATOR_GROOVE|LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
    FXHorizontalFrame* toppart=new FXHorizontalFrame(&deletedialog,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    new FXLabel(toppart,FXString::null,warningicon,ICON_BEFORE_TEXT|JUSTIFY_CENTER_X|JUSTIFY_CENTER_Y|LAYOUT_FILL_Y|LAYOUT_FILL_X);
    FXVerticalFrame *vframe=new FXVerticalFrame(toppart,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
    new FXLabel(vframe,tr("You're about to delete the following files:"),nullptr,LAYOUT_FILL_X|JUSTIFY_LEFT|LAYOUT_FILL_ROW);
    FXHorizontalFrame *listframe=new FXHorizontalFrame(vframe,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0, 0,0,0,0);
    FXList *list=new FXList(listframe,nullptr,0,LAYOUT_FILL_X|LAYOUT_FILL_Y|LIST_MULTIPLESELECT|HSCROLLING_OFF);
    list->setNumVisible(5);
    list->fillItems(filenamelist);
    list->selectAll();
    delete [] filenamelist;
    deletedialog.create();
    if(deletedialog.execute()){
      for(FXint i=0; i<list->getNumItems(); i++){
        if(list->isItemSelected(i)){
          filename=list->getItemText(i);
          FXTRACE((10,"filetoremove=%s\n",filename.text()));
          if(!FXFile::removeFiles(filename,true)){
            if(MBOX_CLICKED_NO==FXMessageBox::error(this,MBOX_YES_NO,tr("Error Deleting File"),tr("Unable to delete file: %s\nContinue with operation?"),filename.text())){
              break;
              }
            }
          }
        }
      }
    }
  return 1;
  }


// Change the pattern
long PathFinderMain::onCmdFilter(FXObject*,FXSelector,void* ptr){
  filelist->setPattern(FXFileSelector::patternFromText((FXchar*)ptr),true);
  return 1;
  }


// Goto directory dialog
long PathFinderMain::onCmdGotoDir(FXObject*,FXSelector,void*){
  FXBMPIcon gotoicon(getApp(),gotodir,0,IMAGE_ALPHAGUESS);
  FXString dir;
  if(FXInputDialog::getString(dir,this,tr("Goto Directory"),tr("&Goto directory:"),&gotoicon)){
    setDirectory(FXPath::validPath(FXPath::absolute(getDirectory(),FXPath::expand(dir))));
    closePreview();
    }
  return 1;
  }


// Open with program
long PathFinderMain::onCmdOpenWith(FXObject*,FXSelector,void*){
  FXString progname=getApp()->reg().readStringEntry("SETTINGS","command","adie");
  FXString filename=filelist->getCurrentFile();
  if(FXInputDialog::getString(progname,this,tr("Open File With"),tr("Open file(s): '") + FXPath::name(filename) + tr("' with:"))){
    FXString commandline=makeCommandline(progname+" %F");
    if(executeCommandline(commandline)){
      getApp()->reg().writeStringEntry("SETTINGS","command",progname.text());
      }
    }
  return 1;
  }


// Open this file with the editor
long PathFinderMain::onCmdOpenWithEditor(FXObject*,FXSelector,void*){
  FXString commandline=makeCommandline(editor);
  executeCommandline(commandline);
  return 1;
  }


// Run a program, possibly with arguments
long PathFinderMain::onCmdRun(FXObject*,FXSelector,void*){
  FXString newprogram=program;
  if(FXInputDialog::getString(newprogram,this,tr("Run Program"),tr("Run a program:"))){
    if(executeCommandline(newprogram)){
      program=newprogram;
      }
    }
  return 1;
  }


// Run terminal in this directory
long PathFinderMain::onCmdTerminal(FXObject*,FXSelector,void*){
  executeCommandline(terminal);
  return 1;
  }


// Harvest the zombies :-)
long PathFinderMain::onSigHarvest(FXObject*,FXSelector,void*){
#ifndef WIN32
  FXTRACE((10,"onSigHarvest\n"));
  while(waitpid(-1,nullptr,WNOHANG)>0){ }
#endif
  return 1;
  }


// Toggle hidden files display
long PathFinderMain::onCmdToggleHidden(FXObject*,FXSelector,void*){
  dirlist->showHiddenFiles(!filelist->showHiddenFiles());
  filelist->showHiddenFiles(!filelist->showHiddenFiles(),true);
  if(dirlist->getCurrentItem()){
    dirlist->makeItemVisible(dirlist->getCurrentItem());
    }
  if(filelist->getCurrentItem()>=0){
    filelist->makeItemVisible(filelist->getCurrentItem());
    }
  return 1;
  }


// Update toggle hidden files widget
long PathFinderMain::onUpdToggleHidden(FXObject* sender,FXSelector,void*){
  sender->handle(this,filelist->showHiddenFiles()?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change image size
long PathFinderMain::onCmdImageSize(FXObject*,FXSelector sel,void*){
  switch(FXSELID(sel)){
    case ID_MINI_SIZE: filelist->setImageSize(16,true); break;
    case ID_NORMAL_SIZE: filelist->setImageSize(32,true); break;
    case ID_MEDIUM_SIZE: filelist->setImageSize(64,true); break;
    case ID_GIANT_SIZE: filelist->setImageSize(128,true); break;
    }
  return 1;
  }


// Update image size
long PathFinderMain::onUpdImageSize(FXObject* sender,FXSelector sel,void*){
  FXbool check=false;
  switch(FXSELID(sel)){
    case ID_MINI_SIZE: check=(filelist->getImageSize()==16); break;
    case ID_NORMAL_SIZE: check=(filelist->getImageSize()==32); break;
    case ID_MEDIUM_SIZE: check=(filelist->getImageSize()==64); break;
    case ID_GIANT_SIZE: check=(filelist->getImageSize()==128); break;
    }
  sender->handle(this,check?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Show preferences dialog
long PathFinderMain::onCmdPreferences(FXObject*,FXSelector,void*){
  Preferences preferences(this);
  FXuint liststyle=filelist->getListStyle();
  preferences.setPatterns(getPatternList());
  preferences.setEditor(editor);
  preferences.setTerminal(terminal);
  preferences.setPreview(preview);
  preferences.setPreviewBlend(blending);
  preferences.setPreviewScale(scaling);
  preferences.setIconPath(associations->getIconPath());
  preferences.setAutoSize((liststyle&ICONLIST_AUTOSIZE)!=0);
  preferences.setItemSpace(filelist->getItemSpace());
  preferences.setExecPaths(execpaths);
  if(preferences.execute(PLACEMENT_OWNER)){
    setPatternList(preferences.getPatterns());
    terminal=preferences.getTerminal();
    editor=preferences.getEditor();
    execpaths=preferences.getExecPaths();
    preview=preferences.getPreview();
    blending=preferences.getPreviewBlend();
    scaling=preferences.getPreviewScale();
    associations->setIconPath(preferences.getIconPath());
    filelist->setItemSpace(preferences.getItemSpace());
    filelist->setListStyle(liststyle^(((0-preferences.getAutoSize())^liststyle)&ICONLIST_AUTOSIZE));
    filelist->setItemSpace(preferences.getItemSpace());
    }
  return 1;
  }


// Select files matching wildcard
long PathFinderMain::onCmdWildcardSelect(FXObject*,FXSelector,void*){
  FXChoiceBox choices(this,tr("Select Files Matching"),tr("Select files matching wildcard pattern"),nullptr,getPatternList(),DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE,0,0,400,300);
  FXint pick=choices.execute(PLACEMENT_OWNER);
  if(0<=pick && filelist->getNumItems()){
    FXString wildcard=FXFileSelector::patternFromText(pattern->getItemText(pick));
#if defined(WIN32)
    filelist->selectMatching(wildcard,(FXPath::PathName|FXPath::NoEscape|FXPath::CaseFold),true);
#else
    filelist->selectMatching(wildcard,(FXPath::PathName|FXPath::NoEscape),true);
#endif
    }
  return 1;
  }

/*******************************************************************************/

// Pop up properties panel
long PathFinderMain::onCmdProperties(FXObject*,FXSelector,void*){
  PropertyDialog* property=new PropertyDialog(this);
  property->create();
  property->show(PLACEMENT_OWNER);
  return 1;
  }


// Change mode
long PathFinderMain::onCmdChmod(FXObject*,FXSelector sel,void* ptr){
  FXuint bit=1<<(FXSELID(sel)-ID_PROP_XOTH);
  FXuint state=(ptr!=0);
  FXString filename;
  FXuint mode;
  for(FXint i=0; i<filelist->getNumItems(); ++i){
    if(filelist->isItemSelected(i) && !filelist->isItemNavigational(i)){
      filename=filelist->getItemPathname(i);
      if((mode=FXStat::mode(filename))==0){
        if(FXMessageBox::error(this,MBOX_YES_NO,tr("Error Reading Permissions"),tr("Unable to read permissions on file:\n\n%s\n\nContinue with operation?"),filename.text())==MBOX_CLICKED_YES) continue;
        break;
        }
      mode^=((0-state)^mode)&bit;       // Make bit equal to state
      if(!FXStat::mode(filename,mode)){
        if(FXMessageBox::error(this,MBOX_YES_NO,tr("Error Changing Permissions"),tr("Unable to change permissions on file:\n\n%s\n\nContinue with operation?"),filename.text())==MBOX_CLICKED_YES) continue;
        break;
        }
      }
    }
  return 1;
  }


// Update change mode
long PathFinderMain::onUpdChmod(FXObject* sender,FXSelector sel,void*){
  FXuint bits=selectedModeBits[FXSELID(sel)-ID_PROP_XOTH];
  sender->handle(this,(bits==0)?FXSEL(SEL_COMMAND,ID_UNCHECK):(bits==selectedCount)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,FXWindow::ID_UNKNOWN),nullptr);
  return 1;
  }


// Update owner
long PathFinderMain::onUpdOwner(FXObject* sender,FXSelector,void*){
  FXStat info;
  FXStat::statFile(filelist->getCurrentFile(),info);
  FXString ownername=FXSystem::userName(info.user());
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&ownername);
  return 1;
  }


// Update group
long PathFinderMain::onUpdGroup(FXObject* sender,FXSelector,void*){
  FXStat info;
  FXStat::statFile(filelist->getCurrentFile(),info);
  FXString group=FXSystem::groupName(info.group());
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&group);
  return 1;
  }


// Update create time
long PathFinderMain::onUpdCreateTime(FXObject* sender,FXSelector,void*){
  FXString time=FXSystem::localTime(FXStat::created(filelist->getCurrentFile()));
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&time);
  return 1;
  }


// Update modified time
long PathFinderMain::onUpdModifyTime(FXObject* sender,FXSelector,void*){
  FXString time=FXSystem::localTime(FXStat::modified(filelist->getCurrentFile()));
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&time);
  return 1;
  }


// Update access time
long PathFinderMain::onUpdAccessTime(FXObject* sender,FXSelector,void*){
  FXString time=FXSystem::localTime(FXStat::accessed(filelist->getCurrentFile()));
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&time);
  return 1;
  }


// Update file location
long PathFinderMain::onUpdFileLocation(FXObject* sender,FXSelector,void*){
  FXString string=filelist->getCurrentFile();
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&string);
  return 1;
  }


// Update file type
long PathFinderMain::onUpdFileSize(FXObject* sender,FXSelector,void*){
  FXString size;
  size.format("%'lld",FXStat::size(filelist->getCurrentFile()));
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&size);
  return 1;
  }


// Update file type
long PathFinderMain::onUpdFileType(FXObject* sender,FXSelector,void*){
  FXString filename=filelist->getCurrentFile();
  FXFileAssoc *fileassoc=nullptr;
  FXString type;
  if(FXStat::isDirectory(filename)){
    fileassoc=associations->findDirBinding(filename.text());
    type="Folder";
    }
  else if(FXStat::isExecutable(filename)){
    fileassoc=associations->findExecBinding(filename.text());
    type="Application";
    }
  else{
    fileassoc=associations->findFileBinding(filename.text());
    type="Document";
    }
  if(fileassoc) type=fileassoc->extension;
  sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&type);
  return 1;
  }


// Update file type
long PathFinderMain::onUpdFileDesc(FXObject* sender,FXSelector,void*){
  FXString filename=filelist->getCurrentFile();
  FXLabel *label=(FXLabel*)sender;
  FXFileAssoc *fileassoc=nullptr;
  if(FXStat::isDirectory(filename)){
    fileassoc=associations->findDirBinding(filename.text());
    }
  else if(FXStat::isExecutable(filename)){
    fileassoc=associations->findExecBinding(filename.text());
    }
  else{
    fileassoc=associations->findFileBinding(filename.text());
    }
  label->setText(FXPath::name(filename));
  if(fileassoc){
    if(fileassoc->bigicon) fileassoc->bigicon->create();
    label->setIcon(fileassoc->bigicon);
    }
  else{
    label->setIcon(nullptr); // FIXME need a default suggestion here
    }
  return 1;
  }

/*******************************************************************************/

// Rotate image
long PathFinderMain::onCmdRotateImage(FXObject*,FXSelector sel,void*){
  FXImage * image=imagepreview->getImage();
  image->rotate((FXSELID(sel)==ID_IMAGE_ROTATE_LEFT)?90:-90);
  imagepreview->setImage(image);
  return 1;
  }


long PathFinderMain::onUpdRotateImage(FXObject* sender,FXSelector,void*){
  sender->handle(this,imagepreview->getImage()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Close image previous panel
long PathFinderMain::onCmdClosePreview(FXObject*,FXSelector,void*){
  closePreview();
  return 1;
  }


// Close image preview
long PathFinderMain::onClickedImagePreview(FXObject*,FXSelector,void *ptr){
  if(((FXEvent*)ptr)->click_count==2) closePreview();
  return 1;
  }


// Update close image previous panel
long PathFinderMain::onUpdClosePreview(FXObject* sender,FXSelector,void*){
  sender->handle(this,switcher->getCurrent()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Load image for preview
FXbool PathFinderMain::previewImage(const FXString& filename){
  FXString ext=FXPath::extension(filename);
  FXImage *img=nullptr;
  FXImage *old=nullptr;

  // Determine type of image
  if(FXString::comparecase(ext,"gif")==0){
    img=new FXGIFImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"bmp")==0){
    img=new FXBMPImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"xpm")==0){
    img=new FXXPMImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"pcx")==0){
    img=new FXPCXImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"ico")==0 || FXString::comparecase(ext,"cur")==0){
    img=new FXICOImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"tga")==0){
    img=new FXTGAImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"rgb")==0){
    img=new FXRGBImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"pbm")==0 || FXString::comparecase(ext,"pgm")==0 || FXString::comparecase(ext,"pnm")==0 || FXString::comparecase(ext,"ppm")==0){
    img=new FXPPMImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"xbm")==0){
    img=new FXXBMImage(getApp(),nullptr,nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"ppm")==0){
    img=new FXPPMImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"iff")==0 || FXString::comparecase(ext,"lbm")==0){
    img=new FXIFFImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"ras")==0){
    img=new FXRASImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
  else if(FXString::comparecase(ext,"dds")==0){
    img=new FXDDSImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
#ifdef HAVE_PNG_H
  else if(FXString::comparecase(ext,"png")==0){
    img=new FXPNGImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
#endif
#ifdef HAVE_JPEG_H
  else if(FXString::comparecase(ext,"jpg")==0){
    img=new FXJPGImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
#endif
#ifdef HAVE_TIFF_H
  else if(FXString::comparecase(ext,"tif")==0 || FXString::comparecase(ext,"tiff")==0){
    img=new FXTIFImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
#endif
#ifdef HAVE_WEBP_H
  else if(FXString::comparecase(ext,"webp")==0){
    img=new FXWEBPImage(getApp(),nullptr,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    }
#endif

  // Perhaps failed
  if(img){
    FXFileStream stream;

    // Load image
    if(stream.open(filename,FXStreamLoad)){
      getApp()->beginWaitCursor();
      img->loadPixels(stream);
      stream.close();

      // Scale to fit
      if(scaling){
        FXint pw=imagepreview->getWidth();
        FXint ph=imagepreview->getHeight();
        if((img->getWidth()>pw) || (img->getHeight()>ph)){
          FXdouble aspect=(FXdouble)img->getHeight() / (FXdouble)img->getWidth();
          if(aspect*pw<ph){
            img->scale(pw,(pw*img->getHeight())/img->getWidth(),1);
            }
          else{
            img->scale((ph*img->getWidth())/img->getHeight(),ph,1);
            }
          }
        }

      // Blend over background
      if(blending){
        img->blend(imagepreview->getBackColor());
        }

      // Create image
      img->create();

      // Set new image, deleting old
      old=imagepreview->getImage();
      imagepreview->setImage(img);
      delete old;
      getApp()->endWaitCursor();

      // Switch to preview
      switcher->setCurrent(1);
      return true;
      }
    }
  return false;
  }


// Close preview
void PathFinderMain::closePreview(){
  delete imagepreview->getImage();
  imagepreview->setImage(nullptr);
  switcher->setCurrent(0);
  }

/*******************************************************************************/

// Clean up
PathFinderMain::~PathFinderMain(){
  getAccelTable()->removeAccel("Back");
  getAccelTable()->removeAccel("Del");
  delete dragshell1;
  delete dragshell2;
  delete dragshell3;
  delete imagepreview->getImage();
  delete associations;
  delete filemenu;
  delete editmenu;
  delete viewmenu;
  delete gomenu;
  delete arrangemenu;
  delete sortmenu;
  delete prefmenu;
  delete helpmenu;
  delete bookmarkmenu;
  delete foxbigicon;
  delete foxminiicon;
  delete cuticon;
  delete copyicon;
  delete moveicon;
  delete linkicon;
  delete renameicon;
  delete pasteicon;
  delete upicon;
  delete homeicon;
  delete backicon;
  delete forwicon;
  delete bigiconsicon;
  delete miniiconsicon;
  delete detailsicon;
  delete mapicon;
  delete unmapicon;
  delete propicon;
  delete deleteicon;
  delete bookmarkicon;
  delete addbookicon;
  delete delbookicon;
  delete clrbooksicon;
  delete sortingicon;
  delete execicon;
  delete newdiricon;
  delete workicon;
  delete closeicon;
  delete locationicon;
  delete entericon;
  delete rotatelefticon;
  delete rotaterighticon;
  delete quiticon;
  delete configicon;
  delete warningicon;
  delete shownicon;
  delete hiddenicon;
  delete searchicon;
  }


/*******************************************************************************/


// Start the whole thing
int main(int argc,char *argv[]){

  // Make sure  we're linked against the right library version
  if(fxversion[0]!=FOX_MAJOR || fxversion[1]!=FOX_MINOR || fxversion[2]!=FOX_LEVEL){
    fxerror("FOX Library mismatch; expected version: %d.%d.%d, but found version: %d.%d.%d.\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL,fxversion[0],fxversion[1],fxversion[2]);
    }

  // Create application
  FXApp application("PathFinder");

  // Initialize application
  application.init(argc,argv);

  // Build GUI
  PathFinderMain* window=new PathFinderMain(&application);

  // On unix, we need to catch SIGCHLD to harvest zombie child processes.
#ifndef WIN32
  application.addSignal(SIGCHLD,window,PathFinderMain::ID_HARVEST,true);
#endif

  // Also catch interrupt so we can gracefully terminate
  application.addSignal(SIGINT,window,PathFinderMain::ID_CLOSE);

  // Create window
  application.create();

  // If given, start in indicated directory
  if(argc==2) window->setDirectory(FXPath::absolute(argv[1]));

  // Run the app now...
  return application.run();
  }

