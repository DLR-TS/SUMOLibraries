/********************************************************************************
*                                                                               *
*                        P r e f e r e n c e s   D i a l o g                    *
*                                                                               *
*********************************************************************************
* Copyright (C) 2003,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#include "PathFinder.h"
#include "Preferences.h"
#include "icons.h"


// Place to look for executables
#if defined(WIN32)
#define SUGGESTED_FOLDER "C:\\Windows\\System32\\"
#else
#define SUGGESTED_FOLDER "/usr/bin/"
#endif

// Executable file extensions
#if defined(WIN32)
#define SUGGESTED_PATTERNS "Executables (*.com,*.exe,*.bat,*.cmd)\nAll Files (*)"
#else
#define SUGGESTED_PATTERNS "All Files (*)"
#endif

/*******************************************************************************/

// Map
FXDEFMAP(Preferences) PreferencesMap[]={
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_ACCEPT,Preferences::onCmdAccept),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_BROWSE_ICONS,Preferences::onCmdBrowseIcons),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_BROWSE_EDITOR,Preferences::onCmdBrowseEditor),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_BROWSE_TERMINAL,Preferences::onCmdBrowseTerminal),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_BROWSE_PATHS,Preferences::onCmdBrowsePaths),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_BROWSE_COMMAND,Preferences::onCmdBrowseCommand),
  FXMAPFUNCS(SEL_UPDATE,Preferences::ID_BROWSE_SMALLICON,Preferences::ID_BROWSE_BIGICONOPEN,Preferences::onUpdSelectExtension),
  FXMAPFUNCS(SEL_COMMAND,Preferences::ID_BROWSE_SMALLICON,Preferences::ID_BROWSE_BIGICONOPEN,Preferences::onCmdBrowseIcon),
  FXMAPFUNC(SEL_UPDATE,Preferences::ID_COMMAND,Preferences::onUpdCommand),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_COMMAND,Preferences::onCmdCommand),
  FXMAPFUNC(SEL_UPDATE,Preferences::ID_DESCRIPTION,Preferences::onUpdDescription),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_DESCRIPTION,Preferences::onCmdDescription),
  FXMAPFUNC(SEL_UPDATE,Preferences::ID_MIMETYPE,Preferences::onUpdMimeType),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_MIMETYPE,Preferences::onCmdMimeType),
  FXMAPFUNC(SEL_SELECTED,Preferences::ID_SELECT_EXTENSION,Preferences::onCmdSelectExtension),
  FXMAPFUNC(SEL_DESELECTED,Preferences::ID_SELECT_EXTENSION,Preferences::onCmdDeselectExtension),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_APPEND_EXTENSION,Preferences::onCmdAppendExtension),
  FXMAPFUNC(SEL_UPDATE,Preferences::ID_CHANGE_EXTENSION,Preferences::onUpdSelectExtension),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_CHANGE_EXTENSION,Preferences::onCmdChangeExtension),
  FXMAPFUNC(SEL_UPDATE,Preferences::ID_REMOVE_EXTENSION,Preferences::onUpdSelectExtension),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_REMOVE_EXTENSION,Preferences::onCmdRemoveExtension),
  FXMAPFUNC(SEL_UPDATE,Preferences::ID_RUN_IN_TERMINAL,Preferences::onUpdRunInTerminal),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_RUN_IN_TERMINAL,Preferences::onCmdRunInTerminal),
  FXMAPFUNC(SEL_UPDATE,Preferences::ID_CHANGE_DIRECTORY,Preferences::onUpdChangeDirectory),
  FXMAPFUNC(SEL_COMMAND,Preferences::ID_CHANGE_DIRECTORY,Preferences::onCmdChangeDirectory),
  };


// Object implementation
FXIMPLEMENT(Preferences,FXDialogBox,PreferencesMap,ARRAYNUMBER(PreferencesMap))


// Construct
Preferences::Preferences(PathFinderMain *own):FXDialogBox(own,"PathFinder Preferences",DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE,0,0,0,0, 0,0,0,0, 4,4){

  FXVerticalFrame *vertical=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXHorizontalFrame *horizontal=new FXHorizontalFrame(vertical,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXVerticalFrame *buttons=new FXVerticalFrame(horizontal,LAYOUT_LEFT|LAYOUT_FILL_Y|FRAME_SUNKEN|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT,0,0,0,0, 0,0,0,0, 0,0);
  FXSwitcher *switcher=new FXSwitcher(horizontal,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

  buttons->setBackColor(getApp()->getShadowColor());

  // Icons
  pat=new FXGIFIcon(getApp(),pattern_gif);
  brw=new FXGIFIcon(getApp(),file_gif);
  mim=new FXGIFIcon(getApp(),miscellaneous_gif);
  dir=new FXGIFIcon(getApp(),setdir);
  run=new FXBMPIcon(getApp(),run_bmp,0,IMAGE_ALPHAGUESS);


  ///////////////////////////  Browser settings pane  ///////////////////////////
  FXVerticalFrame* browserpane=new FXVerticalFrame(switcher,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);
  new FXLabel(browserpane,tr("PathFinder settings"),nullptr,LAYOUT_LEFT);
  new FXHorizontalSeparator(browserpane,SEPARATOR_LINE|LAYOUT_FILL_X);
  FXMatrix *matrix2=new FXMatrix(browserpane,3,MATRIX_BY_COLUMNS|PACK_UNIFORM_HEIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,10,0, 6, 6);

  new FXLabel(matrix2,tr("Icon search paths:"),nullptr,LAYOUT_LEFT);
  icondirs=new FXTextField(matrix2,6,nullptr,0,FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN,0,0,0,0, 2,2,2,2);
  icondirs->setTipText(tr("List of directories to search for icons."));
  new FXButton(matrix2,tr("\tBrowse..."),dir,this,ID_BROWSE_ICONS,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y);

  new FXLabel(matrix2,tr("Editor command:"),nullptr,JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X);
  editor=new FXTextField(matrix2,6,nullptr,0,FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN,0,0,0,0, 2,2,2,2);
  editor->setTipText(tr("Path to text editor program\nCommand line arguments are assembled from the selected file(s) as follows:\n  %f  Replaced by current pathname;\n  %F  Replaced by selected pathnames;\n  %n  Replaced by current filename;\n  %N  Replaced by selected filenames;\n  %u  Replaced by URL encoding of current filename;\n  %U  Replaced by URL encoding of selected files;\n  %d  Replaced by current working directory;\n  %%  Replaced by simply '%'."));
  new FXButton(matrix2,tr("\tBrowse..."),dir,this,ID_BROWSE_EDITOR,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y);

  new FXLabel(matrix2,tr("Terminal command:"),nullptr,JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X);
  terminal=new FXTextField(matrix2,6,nullptr,0,FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN,0,0,0,0, 2,2,2,2);
  terminal->setTipText(tr("Path to terminal program."));
  new FXButton(matrix2,tr("\tBrowse..."),dir,this,ID_BROWSE_TERMINAL,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y);

  new FXLabel(matrix2,tr("Executable paths:"),nullptr,JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X);
  execpaths=new FXTextField(matrix2,6,nullptr,0,FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN,0,0,0,0, 2,2,2,2);
  execpaths->setTipText(tr("List of directories to search for executable programs."));
  new FXButton(matrix2,tr("\tBrowse..."),dir,this,ID_BROWSE_PATHS,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y);

  new FXLabel(matrix2,tr("Preview images:"),nullptr,JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X);
  preview=new FXCheckButton(matrix2,FXString::null,nullptr,0,LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN,0,0,0,0, 0,0,0,0);
  new FXFrame(matrix2,0);

  new FXLabel(matrix2,tr("Image blending:"),nullptr,JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X);
  blending=new FXCheckButton(matrix2,FXString::null,nullptr,0,LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN,0,0,0,0, 0,0,0,0);
  new FXFrame(matrix2,0);

  new FXLabel(matrix2,tr("Image scaling:"),nullptr,JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X);
  scaling=new FXCheckButton(matrix2,FXString::null,nullptr,0,LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN,0,0,0,0, 0,0,0,0);
  new FXFrame(matrix2,0);

  //// Browser settings button
  new FXButton(buttons,tr("Browser\tFile browser settings\tChange browser settings and other things."),brw,switcher,FXSwitcher::ID_OPEN_FIRST,FRAME_RAISED|ICON_ABOVE_TEXT|LAYOUT_FILL_Y);


  /////////////////////////  Mime type settings pane  ///////////////////////////
  FXPacker* mimetypepane=new FXPacker(switcher,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  new FXLabel(mimetypepane,tr("Extensions Setup"),nullptr,LAYOUT_LEFT|LAYOUT_SIDE_TOP);
  new FXHorizontalSeparator(mimetypepane,SEPARATOR_LINE|LAYOUT_FILL_X|LAYOUT_SIDE_TOP);

  // List of possible extensions of this file type
  FXGroupBox *extensiongroup=new FXGroupBox(mimetypepane,tr("File Extensions"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_Y|LAYOUT_SIDE_LEFT);
  FXHorizontalFrame *filetypebuttons=new FXHorizontalFrame(extensiongroup,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0, 0,0,0,0);
  new FXButton(filetypebuttons,tr("Add"),nullptr,this,ID_APPEND_EXTENSION,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  new FXButton(filetypebuttons,tr("Edit"),nullptr,this,ID_CHANGE_EXTENSION,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  new FXButton(filetypebuttons,tr("Delete"),nullptr,this,ID_REMOVE_EXTENSION,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  FXVerticalFrame *extensionFrame=new FXVerticalFrame(extensiongroup,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0, 0,0,0,0);
  extensions=new FXList(extensionFrame,this,ID_SELECT_EXTENSION,LIST_BROWSESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y|HSCROLLER_NEVER);
  extensions->setSortFunc(FXList::ascendingCase);
  extensions->setNumVisible(4);

  // Description
  FXGroupBox *descgroup=new FXGroupBox(mimetypepane,tr("Description of extension"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_SIDE_TOP);
  description=new FXTextField(descgroup,30,this,ID_DESCRIPTION,LAYOUT_FILL_X|LAYOUT_CENTER_Y|FRAME_SUNKEN|FRAME_THICK);
  description->setTipText(tr("Short description of file type."));

  // Command
  FXGroupBox *commandgroup=new FXGroupBox(mimetypepane,tr("Command"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_SIDE_TOP);
  FXHorizontalFrame *commandset=new FXHorizontalFrame(commandgroup,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
  new FXButton(commandset,tr("\tBrowse..."),dir,this,ID_BROWSE_COMMAND,LAYOUT_RIGHT|LAYOUT_CENTER_Y|FRAME_RAISED|FRAME_THICK);
  command=new FXTextField(commandset,40,this,ID_COMMAND,LAYOUT_FILL_X|LAYOUT_CENTER_Y|FRAME_SUNKEN|FRAME_THICK);
  command->setTipText(tr("Path to program associated with the file\nCommand line arguments are assembled from the selected file(s) as follows:\n  %f  Replaced by current pathname;\n  %F  Replaced by selected pathnames;\n  %n  Replaced by current filename;\n  %N  Replaced by selected filenames;\n  %u  Replaced by URL encoding of current filename;\n  %U  Replaced by URL encoding of selected files;\n  %d  Replaced by current working directory;\n  %%  Replaced by simply '%'."));
  runinterminal=new FXCheckButton(commandgroup,tr("Run in terminal\t\tRun command in terminal."),this,ID_RUN_IN_TERMINAL,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);
  changedirectory=new FXCheckButton(commandgroup,tr("Change directory\t\tChange directory before running command."),this,ID_CHANGE_DIRECTORY,ICON_BEFORE_TEXT|LAYOUT_SIDE_LEFT);

  // Mime type
  FXGroupBox *mimegroup=new FXGroupBox(mimetypepane,tr("Mime Types"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_SIDE_TOP);
  FXVerticalFrame *mimeFrame=new FXVerticalFrame(mimegroup,LAYOUT_SIDE_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0, 0,0,0,0);
  mimetypes=new FXComboBox(mimeFrame,10,this,ID_MIMETYPE,COMBOBOX_NO_REPLACE|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  mimetypes->setSortFunc(FXList::ascendingCase);
  mimetypes->setNumVisible(10);
  mimetypes->setTipText(tr("Select existing mime-type or type in new one\nMime type is used to identify format of item during drag and drop and clipboard operations."));

  // Various icons for this extension
  FXGroupBox *iconsgroup=new FXGroupBox(mimetypepane,tr("Icons"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X|LAYOUT_SIDE_TOP);
  FXMatrix *iconsmatrix=new FXMatrix(iconsgroup,4,MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y|PACK_UNIFORM_WIDTH,0,0,0,0, 0,0,0,0);

  // Icon labels
  new FXLabel(iconsmatrix,tr("Small"),nullptr,JUSTIFY_CENTER_X|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_COLUMN);
  new FXLabel(iconsmatrix,tr("Big"),nullptr,JUSTIFY_CENTER_X|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_COLUMN);
  new FXLabel(iconsmatrix,tr("Small Open"),nullptr,JUSTIFY_CENTER_X|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_COLUMN);
  new FXLabel(iconsmatrix,tr("Big Open"),nullptr,JUSTIFY_CENTER_X|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_COLUMN);

  // Icons
  iconbutton[0]=new FXButton(iconsmatrix,tr("\tChange small icon."),nullptr,this,ID_BROWSE_SMALLICON,FRAME_RAISED|LAYOUT_CENTER_X|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|JUSTIFY_CENTER_X|JUSTIFY_CENTER_Y|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW,0,0,56,56, 1,1,1,1);
  iconbutton[1]=new FXButton(iconsmatrix,tr("\tChange big icon."),nullptr,this,ID_BROWSE_BIGICON,FRAME_RAISED|LAYOUT_CENTER_X|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|JUSTIFY_CENTER_X|JUSTIFY_CENTER_Y|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW,0,0,56,56, 1,1,1,1);
  iconbutton[2]=new FXButton(iconsmatrix,tr("\tChange small open icon."),nullptr,this,ID_BROWSE_SMALLICONOPEN,FRAME_RAISED|LAYOUT_CENTER_X|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|JUSTIFY_CENTER_X|JUSTIFY_CENTER_Y|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW,0,0,56,56, 1,1,1,1);
  iconbutton[3]=new FXButton(iconsmatrix,tr("\tChange big open icon."),nullptr,this,ID_BROWSE_BIGICONOPEN,FRAME_RAISED|LAYOUT_CENTER_X|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|JUSTIFY_CENTER_X|JUSTIFY_CENTER_Y|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW,0,0,56,56, 1,1,1,1);
  iconimage[0]=nullptr;
  iconimage[1]=nullptr;
  iconimage[2]=nullptr;
  iconimage[3]=nullptr;

  //// Mime type settings button
  new FXButton(buttons,tr("Extensions\tFile extensions setup\tChange associations for file extensions."),mim,switcher,FXSwitcher::ID_OPEN_SECOND,FRAME_RAISED|ICON_ABOVE_TEXT|LAYOUT_FILL_Y);


  ///////////////////////  File pattern settings pane  //////////////////////////
  FXVerticalFrame* filepatpane=new FXVerticalFrame(switcher,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);
  new FXLabel(filepatpane,tr("Filename patterns"),nullptr,LAYOUT_LEFT);
  new FXHorizontalSeparator(filepatpane,SEPARATOR_LINE|LAYOUT_FILL_X);
  FXVerticalFrame *sub3=new FXVerticalFrame(filepatpane,LAYOUT_FILL_Y|LAYOUT_FILL_X,0,0,0,0, 0,0,10,0, 0,0);
  new FXLabel(sub3,tr("Filename patterns, one per line:"),nullptr,JUSTIFY_LEFT);
  FXVerticalFrame* textwell=new FXVerticalFrame(sub3,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0, 0,0,0,0);
  pattern=new FXText(textwell,nullptr,0,LAYOUT_FILL_X|LAYOUT_FILL_Y);

  //// File pattern settings button
  new FXButton(buttons,tr("Patterns\tFilename patterns\tChange wildcard patterns for filenames."),pat,switcher,FXSwitcher::ID_OPEN_THIRD,FRAME_RAISED|ICON_ABOVE_TEXT|LAYOUT_FILL_Y);



  ///////////////////////////  Other settings pane  /////////////////////////////
  FXVerticalFrame* otherpane=new FXVerticalFrame(switcher,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);
  new FXLabel(otherpane,tr("Other Settings"),nullptr,LAYOUT_LEFT);
  new FXHorizontalSeparator(otherpane,SEPARATOR_LINE|LAYOUT_FILL_X);
  FXMatrix *matrix3=new FXMatrix(otherpane,3,MATRIX_BY_COLUMNS|PACK_UNIFORM_HEIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,10,0, 6, 6);

  new FXLabel(matrix3,tr("Auto size lists:"),nullptr,LAYOUT_LEFT|LAYOUT_CENTER_Y);
  autosize=new FXCheckButton(matrix3,FXString::null,nullptr,0,LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN,0,0,0,0, 0,0,0,0);
  new FXFrame(matrix3,0);

  new FXLabel(matrix3,tr("Item space:"),nullptr,LAYOUT_LEFT|LAYOUT_CENTER_Y);
  itemspace=new FXTextField(matrix3,6,nullptr,0,TEXTFIELD_INTEGER|JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y,0,0,0,0, 2,2,2,2);
  new FXFrame(matrix3,0);

  //// File pattern settings button
  new FXButton(buttons,tr("Other\tOther settings\tChange other settings."),run,switcher,FXSwitcher::ID_OPEN_FOURTH,FRAME_RAISED|ICON_ABOVE_TEXT|LAYOUT_FILL_Y);


  // Bottom part
  new FXHorizontalSeparator(vertical,SEPARATOR_RIDGE|LAYOUT_FILL_X);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(vertical,LAYOUT_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH);
  new FXButton(closebox,tr("&Accept"),nullptr,this,FXDialogBox::ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  new FXButton(closebox,tr("&Cancel"),nullptr,this,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);

  // Populate bindings
  setupFileBindings();
  }


// Set item space
void Preferences::setItemSpace(FXint s){
  itemspace->setText(FXString::value(s));
  }


// Get item space
FXint Preferences::getItemSpace() const {
  return itemspace->getText().toInt();
  }


// Populate file bindings and mime types
void Preferences::setupFileBindings(){
  const FXStringDictionary& section=getApp()->reg().at("FILETYPES");
  extensions->clearItems();
  mimetypes->clearItems();
  if(!section.empty()){
    FXString mime;
    for(FXint entry=0; entry<section.no(); ++entry){
      if(section.empty(entry)) continue;
      extensions->appendItem(section.key(entry),nullptr,nullptr,true);
      mime=section.data(entry).section(';',4);
      if(!mime.empty() && (mimetypes->findItem(mime)==-1)){
        mimetypes->appendItem(mime);
        }
      extensions->sortItems();
      mimetypes->sortItems();
      }
    }
  }


// Search iconpath for given name and load the icon
FXIcon *Preferences::createIconFromName(const FXString& name) const {
  FXString iconfilename=FXPath::search(getIconPath(),name);
  if(!iconfilename.empty()){
    FXIcon *ico=FXIconSource::defaultIconSource.loadIconFile(getApp(),iconfilename);
    if(ico){
      ico->blend(getApp()->getBaseColor());
      ico->create();
      return ico;
      }
    }
  return nullptr;
  }


// Change icon on button
void Preferences::changeIconButton(const FXString& name,FXint index){
  FXASSERT(0<=index && index<=4);
  if(iconimage[index]){
    delete iconimage[index];
    iconbutton[index]->setIcon(nullptr);
    iconimage[index]=nullptr;
    }
  iconimage[index]=createIconFromName(name);
  if(iconimage[index]){
    iconbutton[index]->setIcon(iconimage[index]);
    }
  }


// Read file extension from registry
void Preferences::readFileExtension(const FXString& ext){
  if(!ext.empty()){
    FXString association=getApp()->reg().readStringEntry("FILETYPES",ext.text());
    FXString string;
    FXint index;

    FXTRACE((1,"association=%s\n",association.text()));

    // Get command and description names
    filecommand=association.section(';',0);
    filedesc=association.section(';',1);

    // Big icon closed and open
    string=association.section(';',2);
    fileicons[1]=string.section(':',0);
    fileicons[3]=string.section(':',1);

    // Small icon closed and open
    string=association.section(';',3);
    fileicons[0]=string.section(':',0);
    fileicons[2]=string.section(':',1);

    // Mime type name
    filemime=association.section(';',4);
    if(!filemime.empty()){
      index=mimetypes->findItem(filemime);
      mimetypes->setCurrentItem(index);
      }
    else{
      index=mimetypes->findItem(" ");           // FIXME
      mimetypes->setCurrentItem(index);
      }

    // Flags
    string=association.section(';',5);
    fileflags=0;
    if(string.contains("c")) fileflags|=1;
    if(string.contains("t")) fileflags|=2;

    // Change icons
    changeIconButton(fileicons[0],0);
    changeIconButton(fileicons[1],1);
    changeIconButton(fileicons[2],2);
    changeIconButton(fileicons[3],3);
    }
  }


// Save file extension to registry
void Preferences::writeFileExtension(const FXString& ext){
  if(!ext.empty()){
    FXString association;

    // Command line
    association=filecommand;
    association+=";";

    // Description
    association+=filedesc;
    association+=";";

    // Big icon(s)
    association+=fileicons[1];
    if(!fileicons[3].empty()){
      association+=":";
      association+=fileicons[3];
      }
    association+=";";

    // Mini icon(s)
    association+=fileicons[0];
    if(!fileicons[2].empty()){
      association+=":";
      association+=fileicons[2];
      }
    association+=";";

    // Mime type
    association+=filemime;

    // Flags, if any
    if(fileflags){
      association+=";";
      if(fileflags&1){
        association+="c";
        }
      if(fileflags&2){
        association+="t";
        }
      }

    FXTRACE((1,"association=%s\n",association.text()));

    // Write to filetypes key
    getApp()->reg().writeStringEntry("FILETYPES",ext.text(),association.text());
    }
  }


// Run in terminal update
long Preferences::onUpdRunInTerminal(FXObject* sender,FXSelector,void*){
  sender->handle(this,(fileflags&2) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Run in terminal
long Preferences::onCmdRunInTerminal(FXObject*,FXSelector,void*){
  fileflags^=2;
  return 1;
  }


// Change directory update
long Preferences::onUpdChangeDirectory(FXObject* sender,FXSelector,void*){
  sender->handle(this,(fileflags&1) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Change directory
long Preferences::onCmdChangeDirectory(FXObject*,FXSelector,void*){
  fileflags^=1;
  return 1;
  }


// Select extension
long Preferences::onCmdSelectExtension(FXObject*,FXSelector,void* ptr){
  readFileExtension(extensions->getItemText((FXint)(FXival)ptr));
  return 1;
  }


// Deselect extension
long Preferences::onCmdDeselectExtension(FXObject*,FXSelector,void* ptr){
  writeFileExtension(extensions->getItemText((FXint)(FXival)ptr));
  return 1;
  }


// Gray out things if no extension selected
long Preferences::onUpdSelectExtension(FXObject* sender,FXSelector,void*){
  sender->handle(this,(0<=extensions->getCurrentItem())?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Append new extension name
long Preferences::onCmdAppendExtension(FXObject*,FXSelector,void*){
  FXString ext;
  if(FXInputDialog::getString(ext,this,tr("New File Extension"),tr("Please enter file extension:"),nullptr)){
    if(ext.empty()) return 1;
    if(0<=extensions->findItem(ext)){
      FXMessageBox::question(this,MBOX_OK,tr("Duplicate Extension"),tr("The given extension: %s already exists!"),ext.text());
      return 1;
      }
    FXint index=extensions->appendItem(ext);
    extensions->setCurrentItem(index,true);
    extensions->sortItems();
    extensions->makeItemVisible(extensions->getCurrentItem());
    }
  return 1;
  }


// Change extension name
long Preferences::onCmdChangeExtension(FXObject*,FXSelector,void*){
  FXint index=extensions->getCurrentItem();
  if(0<=index){
    FXString ext=extensions->getItemText(index);
    FXString old=ext;
    if(FXInputDialog::getString(ext,this,tr("Rename File Extension"),tr("Rename file extension:"),nullptr)){
      if(ext==old) return 1;
      if(0<=extensions->findItem(ext)){
        FXMessageBox::question(this,MBOX_OK,tr("Duplicate Extension"),tr("The given extension: %s already exists!"),ext.text());
        return 1;
        }
      getApp()->reg().deleteEntry("FILETYPES",old);
      extensions->setItemText(index,ext);
      }
    }
  return 1;
  }


// Remove extension name
long Preferences::onCmdRemoveExtension(FXObject*,FXSelector,void*){
  FXint index=extensions->getCurrentItem();
  if(0<=index){
    FXString ext=extensions->getItemText(index);
    if(FXMessageBox::question(this,MBOX_OK_CANCEL,tr("Delete File Extension?"),tr("Are you sure you want to delete the file extension: %s?"),ext.text())==MBOX_CLICKED_OK){
      getApp()->reg().deleteEntry("FILETYPES",ext);
      extensions->removeItem(extensions->getCurrentItem(),true);
      }
    }
  return 1;
  }


// Change mime type
long Preferences::onCmdMimeType(FXObject*,FXSelector,void* ptr){
  FXString mime=(const FXchar*)ptr;
  if(!mime.empty() && (mimetypes->findItem(mime)==-1)){
    mimetypes->appendItem(mime);
    mimetypes->sortItems();
    }
  filemime=mime;
  return 1;
  }


// Update mime type
long Preferences::onUpdMimeType(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&filemime);
  return 1;
  }


// Browse icon path
long Preferences::onCmdBrowseIcons(FXObject*,FXSelector,void*){
  FXString path=getIconPath();
  path=FXFileDialog::getOpenDirectory(this,tr("Icon Paths"),path);
  if(!path.empty()) setIconPath(path);
  return 1;
  }


// Set browser editor
long Preferences::onCmdBrowseEditor(FXObject*,FXSelector,void*){
  FXString neweditor=getEditor();
  neweditor=FXFileDialog::getOpenFilename(this,tr("Editor Program"),neweditor);
  if(!neweditor.empty()) setEditor(neweditor);
  return 1;
  }


// Select command
long Preferences::onCmdBrowseCommand(FXObject*,FXSelector,void*){
  FXString oldcommand=filecommand.empty() ? SUGGESTED_FOLDER : FXPath::dequote(filecommand);
  FXString newcommand=FXFileDialog::getOpenFilename(this,tr("Select Command"),oldcommand,SUGGESTED_PATTERNS);
  if(!newcommand.empty()){
    filecommand=FXPath::enquote(newcommand)+" %f";
    }
  return 1;
  }


// Set terminal
long Preferences::onCmdBrowseTerminal(FXObject*,FXSelector,void*){
  FXString newterminal=getTerminal();
  newterminal=FXFileDialog::getOpenFilename(this,tr("Terminal Program"),newterminal);
  if(!newterminal.empty()) setTerminal(newterminal);
  return 1;
  }


// Set paths
long Preferences::onCmdBrowsePaths(FXObject*,FXSelector,void*){
  FXString path=getExecPaths();
  path=FXFileDialog::getOpenDirectory(this,tr("Executable Paths"),path);
  if(!path.empty()) setExecPaths(path);
  return 1;
  }


// Set icon path
long Preferences::onCmdBrowseIcon(FXObject*,FXSelector sel,void*){
  FXFileDialog opendialog(this,tr("Select Icon"));
  FXint index=FXSELID(sel)-ID_BROWSE_SMALLICON;
  FXString iconfilename=FXPath::search(getIconPath(),fileicons[index]);
  if(iconfilename.empty()) iconfilename=FXPath::search(getIconPath(),".");
  opendialog.setSelectMode(SELECTFILE_EXISTING);
  opendialog.showImages(true);
  opendialog.setFilename(iconfilename);
  opendialog.setItemSpace(100);
  opendialog.setFileBoxStyle(ICONLIST_BIG_ICONS|ICONLIST_BROWSESELECT);
  if(opendialog.execute()){
    iconfilename=opendialog.getFilename();
    fileicons[index]=FXPath::relativize(getIconPath(),iconfilename);
    changeIconButton(fileicons[index],index);
    }
  return 1;
  }


// Change description
long Preferences::onCmdDescription(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_GETSTRINGVALUE),(void*)&filedesc);
  return 1;
  }


// Update description
long Preferences::onUpdDescription(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&filedesc);
  return 1;
  }


// Change command
long Preferences::onCmdCommand(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_GETSTRINGVALUE),(void*)&filecommand);
  return 1;
  }


// Update command
long Preferences::onUpdCommand(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&filecommand);
  return 1;
  }


// Close the application, return TRUE if actually closed
long Preferences::onCmdAccept(FXObject* sender,FXSelector sel,void* ptr){
  if(FXDialogBox::onCmdAccept(sender,sel,ptr)){
    FXint index=extensions->getCurrentItem();
    if(0<=index){ writeFileExtension(extensions->getItemText(index)); }
    }
  return 1;
  }


// Clean up
Preferences::~Preferences(){
  delete iconimage[0];
  delete iconimage[1];
  delete iconimage[2];
  delete iconimage[3];
  delete pat;
  delete brw;
  delete mim;
  delete run;
  delete dir;
  }
