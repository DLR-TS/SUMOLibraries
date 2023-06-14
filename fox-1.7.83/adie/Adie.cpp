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
#include <signal.h>
#ifndef WIN32
#include <sys/wait.h>
#endif
#include "HelpWindow.h"
#include "Preferences.h"
#include "Commands.h"
#include "Modeline.h"
#include "Syntax.h"
#include "SyntaxParser.h"
#include "TextWindow.h"
#include "FindInFiles.h"
#include "Adie.h"
#include "icons.h"

/*
  Notes:
  - One single collection of icons.
  - Manage list of open windows.
*/

/*******************************************************************************/


// Map
FXDEFMAP(Adie) AdieMap[]={
  FXMAPFUNC(SEL_SIGNAL,Adie::ID_HARVEST,Adie::onSigHarvest),
  FXMAPFUNC(SEL_SIGNAL,Adie::ID_CLOSEALL,Adie::onCmdCloseAll),
  FXMAPFUNC(SEL_COMMAND,Adie::ID_CLOSEALL,Adie::onCmdCloseAll),
  FXMAPFUNC(SEL_COMMAND,Adie::ID_SYNTAXPATHS,Adie::onCmdSyntaxPaths),
  FXMAPFUNC(SEL_UPDATE,Adie::ID_SYNTAXPATHS,Adie::onUpdSyntaxPaths),
  };


// Object implementation
FXIMPLEMENT(Adie,FXApp,AdieMap,ARRAYNUMBER(AdieMap))


// Make some windows
Adie::Adie(const FXString& name):FXApp(name){
  FXTRACE((10,"Adie::Adie(%s)\n",name.text()));

  // File associations, shared between all windows
  associations=new FXFileAssociations(this);

  // Make some icons; these are shared between all text windows
  bigicon=new FXGIFIcon(this,big_gif);
  smallicon=new FXGIFIcon(this,small_gif);
  newicon=new FXGIFIcon(this,new_gif);
  newfileicon=new FXGIFIcon(this,newfile_gif);
  reloadicon=new FXGIFIcon(this,reload_gif);
  openicon=new FXGIFIcon(this,open_gif);
  closeicon=new FXGIFIcon(this,close_gif);
  saveicon=new FXGIFIcon(this,save_gif);
  saveasicon=new FXGIFIcon(this,saveas_gif);
  savetoicon=new FXGIFIcon(this,saveto_gif);
  printicon=new FXGIFIcon(this,print_gif);
  importicon=new FXGIFIcon(this,import_gif);
  exporticon=new FXGIFIcon(this,export_gif);
  cuticon=new FXGIFIcon(this,cut_gif);
  copyicon=new FXGIFIcon(this,copy_gif);
  pasteicon=new FXGIFIcon(this,paste_gif);
  deleteicon=new FXGIFIcon(this,delete_gif,0,IMAGE_THRESGUESS);
  undoicon=new FXGIFIcon(this,undo_gif);
  redoicon=new FXGIFIcon(this,redo_gif);
  fontsicon=new FXGIFIcon(this,fonts_gif);
  helpicon=new FXGIFIcon(this,help_gif);
  quiticon=new FXGIFIcon(this,quit_gif);
  searchicon=new FXGIFIcon(this,search_gif,0,IMAGE_ALPHAGUESS);
  replaceicon=new FXGIFIcon(this,replace_gif,0,IMAGE_ALPHAGUESS);
  expressicon=new FXGIFIcon(this,express_gif);
  searchfilesicon=new FXGIFIcon(this,searchfiles_gif);
  searchnexticon=new FXGIFIcon(this,searchnext_gif,0,IMAGE_ALPHAGUESS);
  searchprevicon=new FXGIFIcon(this,searchprev_gif,0,IMAGE_ALPHAGUESS);
  searchrexicon=new FXGIFIcon(this,search_rex);
  searchnorexicon=new FXGIFIcon(this,search_norex);
  searchcaseicon=new FXGIFIcon(this,search_case);
  searchnocaseicon=new FXGIFIcon(this,search_nocase);
  searchwordicon=new FXGIFIcon(this,search_word);
  searchnowordicon=new FXGIFIcon(this,search_noword);
  searchupicon=new FXGIFIcon(this,search_up);
  searchdnicon=new FXGIFIcon(this,search_dn);
  bookseticon=new FXGIFIcon(this,bookset2_gif);
  booknexticon=new FXGIFIcon(this,booknxt2_gif);
  bookprevicon=new FXGIFIcon(this,bookprv2_gif);
  bookdelicon=new FXGIFIcon(this,bookdel2_gif);
  bookclricon=new FXGIFIcon(this,bookclr2_gif);
  shiftlefticon=new FXGIFIcon(this,shiftleft_gif);
  shiftrighticon=new FXGIFIcon(this,shiftright_gif);
  configicon=new FXGIFIcon(this,config_gif);
  browsericon=new FXGIFIcon(this,browser);
  nobrowsericon=new FXGIFIcon(this,nobrowser);
  loggericon=new FXGIFIcon(this,logger);
  nologgericon=new FXGIFIcon(this,nologger);
  uppercaseicon=new FXGIFIcon(this,uppercase);
  lowercaseicon=new FXGIFIcon(this,lowercase);
  backwardicon=new FXGIFIcon(this,backward_gif);
  forwardicon=new FXGIFIcon(this,forward_gif);
  shownicon=new FXGIFIcon(this,fileshown_gif);
  hiddenicon=new FXGIFIcon(this,filehidden_gif);
  wordwrapicon=new FXGIFIcon(this,wordwrap_gif);
  nowrapicon=new FXGIFIcon(this,nowrap_gif);
  filtericon=new FXGIFIcon(this,filter_gif);
  commandicon=new FXGIFIcon(this,terminal_gif);
  scripticon=new FXGIFIcon(this,script_gif);
  switchicon=new FXGIFIcon(this,switch_gif);
  openselicon=new FXGIFIcon(this,opensel_gif);
  docsicon=new FXGIFIcon(this,docs_gif);
  pointicon=new FXGIFIcon(this,point2_gif);

#ifndef DEBUG
  // If interrupt happens, quit gracefully; we may want to
  // save edit buffer contents w/o asking if display gets
  // disconnected or if hangup signal is received.
  addSignal(SIGINT,this,ID_CLOSEALL);
#ifndef WIN32
  addSignal(SIGQUIT,this,ID_CLOSEALL);
  addSignal(SIGHUP,this,ID_CLOSEALL);
  addSignal(SIGPIPE,this,ID_CLOSEALL);
#endif
#endif

#ifndef WIN32
  // On unix, we need to catch SIGCHLD to harvest zombie child processes.
  //addSignal(SIGCHLD,this,ID_HARVEST,true);
#endif
  }


/*******************************************************************************/


// Clean up the mess
Adie::~Adie(){
  FXTRACE((10,"Adie::~Adie()\n"));
  for(int i=0; i<numSyntaxes(); i++) delete getSyntax(i);
  FXASSERT(numWindows()==0);
  delete associations;
  delete bigicon;
  delete smallicon;
  delete newicon;
  delete newfileicon;
  delete reloadicon;
  delete openicon;
  delete closeicon;
  delete saveicon;
  delete saveasicon;
  delete savetoicon;
  delete printicon;
  delete importicon;
  delete exporticon;
  delete cuticon;
  delete copyicon;
  delete pasteicon;
  delete deleteicon;
  delete undoicon;
  delete redoicon;
  delete fontsicon;
  delete helpicon;
  delete quiticon;
  delete searchicon;
  delete replaceicon;
  delete expressicon;
  delete searchfilesicon;
  delete searchnexticon;
  delete searchprevicon;
  delete searchrexicon;
  delete searchnorexicon;
  delete searchcaseicon;
  delete searchnocaseicon;
  delete searchwordicon;
  delete searchnowordicon;
  delete searchupicon;
  delete searchdnicon;
  delete bookseticon;
  delete booknexticon;
  delete bookprevicon;
  delete bookdelicon;
  delete bookclricon;
  delete shiftlefticon;
  delete shiftrighticon;
  delete configicon;
  delete browsericon;
  delete nobrowsericon;
  delete loggericon;
  delete nologgericon;
  delete uppercaseicon;
  delete lowercaseicon;
  delete backwardicon;
  delete forwardicon;
  delete shownicon;
  delete hiddenicon;
  delete wordwrapicon;
  delete nowrapicon;
  delete filtericon;
  delete commandicon;
  delete scripticon;
  delete switchicon;
  delete openselicon;
  delete docsicon;
  delete pointicon;
  }


/*******************************************************************************/


// Get syntax for language name
Syntax* Adie::getSyntaxByName(const FXString& lang){
  FXTRACE((11,"Adie::getSyntaxByName(%s)\n",lang.text()));
  if(!lang.empty()){
    for(FXint syn=0; syn<numSyntaxes(); syn++){
      if(getSyntax(syn)->getName()==lang){
        FXTRACE((11,"syntaxes[%d]: language: %s matched name: %s!\n",syn,getSyntax(syn)->getName().text(),lang.text()));
        return getSyntax(syn);
        }
      }
    }
  return nullptr;
  }


// Get syntax by consulting registry
Syntax* Adie::getSyntaxByRegistry(const FXString& file){
  FXTRACE((11,"Adie::getSyntaxByRegistry(%s)\n",file.text()));
  if(!file.empty()){
    FXString name=FXPath::name(file);
    FXString lang=reg().readStringEntry("SYNTAX",name);
    return getSyntaxByName(lang);
    }
  return nullptr;
  }


// Get syntax by matching file patterns
Syntax* Adie::getSyntaxByPattern(const FXString& file){
  FXTRACE((11,"Adie::getSyntaxByPattern(%s)\n",file.text()));
  if(!file.empty()){
    for(FXint syn=0; syn<numSyntaxes(); syn++){
      if(getSyntax(syn)->matchFilename(file)){
        FXTRACE((11,"syntaxes[%d]: language: %s matched file: %s!\n",syn,getSyntax(syn)->getName().text(),file.text()));
        return getSyntax(syn);
        }
      }
    }
  return nullptr;
  }


// Get syntax by matching file contents
Syntax* Adie::getSyntaxByContents(const FXString& contents){
  FXTRACE((11,"Adie::getSyntaxByContents(%s)\n",contents.text()));
  if(!contents.empty()){
    for(FXint syn=0; syn<numSyntaxes(); syn++){
      if(getSyntax(syn)->matchContents(contents)){
        FXTRACE((11,"syntaxes[%d]: language: %s matched contents: %s!\n",syn,getSyntax(syn)->getName().text(),contents.text()));
        return getSyntax(syn);
        }
      }
    }
  return nullptr;
  }


// Generate unique name from given path
FXString Adie::unique(const FXString& path) const {
  FXString name="untitled";
  FXString file;
  for(FXint i=1; i<2147483647; i++){
    file=FXPath::absolute(path,name);
    if(!findWindow(file)) break;
    name.format("untitled%d",i);
    }
  return file;
  }


// Find an as yet untitled, unedited window
TextWindow *Adie::findUnused() const {
  for(FXint w=0; w<numWindows(); w++){
    if(!getWindow(w)->isFilenameSet() && !getWindow(w)->isModified()){
      return getWindow(w);
      }
    }
  return nullptr;
  }


// Find window, if any, currently editing the given file
TextWindow* Adie::findWindow(const FXString& file) const {
  for(FXint w=0; w<numWindows(); w++){
    if(getWindow(w)->getFilename()==file){
      return getWindow(w);
      }
    }
  return nullptr;
  }


// Open window on file, creating new one if not already open
TextWindow* Adie::openFileWindow(const FXString& file,FXbool edit){
  FXTRACE((11,"Adie::openFileWindow(%s)\n",file.text()));

  // See if we already have this file
  TextWindow * window=findWindow(file);
  if(!window){

    // Create new one if no unused windows
    window=findUnused();
    if(!window){
      window=new TextWindow(this);
      window->create();
      }

    // Open file in this window, setting syntax, bookmarks, etc.
    if(window->loadFile(file)){
      window->readBookmarks(file);
      window->readView(file);
      window->setEditable(edit);
      window->determineSyntax();
      window->parseModeline();
      window->setBrowserCurrentFile(file);
      }

    // Make empty window with given filename
    else{
      window->setFilename(file);
      window->setFilenameSet(true);
      window->determineSyntax();
      window->setBrowserCurrentFile(file);
      }
    }

  // Bring up the window
  window->raise();
  window->setFocus();
  return window;
  }

/*******************************************************************************/

// Harvest the zombies :-)
long Adie::onSigHarvest(FXObject*,FXSelector,void*){
#ifndef WIN32
  while(waitpid(-1,nullptr,WNOHANG)>0){ }
#endif
  return 1;
  }


// Change syntax paths
long Adie::onCmdSyntaxPaths(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_GETSTRINGVALUE),(void*)&syntaxpaths);
  reg().writeStringEntry("SETTINGS","syntaxpaths",syntaxpaths.text());
  return 1;
  }


// Update syntax paths
long Adie::onUpdSyntaxPaths(FXObject* sender,FXSelector,void*){
  sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&syntaxpaths);
  return 1;
  }


// Close all windows
long Adie::onCmdCloseAll(FXObject*,FXSelector,void*){
  while(0<numWindows() && getWindow(0)->close(true)){}
  return 1;
  }

/*******************************************************************************/

// Print command line help
static void printusage(){
  fxmessage("Usage: adie [options] files...\n");
  fxmessage("  options:\n");
  fxmessage("  --help, -h               Print help.\n");
  fxmessage("  --version                Print version number.\n");
  fxmessage("  --view                   Start in view-only mode.\n");
  fxmessage("  --edit                   Start in edit-mode.\n");
  fxmessage("  --line=NUM               Jump cursor position to line number.\n");
  fxmessage("  --column=NUM             Jump cursor position to column.\n");
  fxmessage("  --syntax=SYNTAXFILE      Load given syntax file.\n");
  fxmessage("  --lang=LANGUAGE          Force language mode.\n");
  fxmessage("  --size=WxH+X+Y           Force window size and placement.\n");
  }


// Print verson info
static void printversion(){
  fxmessage("A.d.i.e. - ADvanced Interactive Editor %d.%d.%d.\n",VERSION_MAJOR,VERSION_MINOR,VERSION_PATCH);
  fxmessage("Copyright (C) 2000,2023 Jeroen van der Zijp.  All Rights Reserved.\n\n");
  fxmessage("Please visit: http://www.fox-toolkit.org for further information.\n");
  fxmessage("\n");
  fxmessage("This program is free software: you can redistribute it and/or modify\n");
  fxmessage("it under the terms of the GNU General Public License as published by\n");
  fxmessage("the Free Software Foundation, either version 3 of the License, or\n");
  fxmessage("(at your option) any later version.\n");
  fxmessage("\n");
  fxmessage("This program is distributed in the hope that it will be useful,\n");
  fxmessage("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fxmessage("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fxmessage("GNU General Public License for more details.\n");
  fxmessage("\n");
  fxmessage("You should have received a copy of the GNU General Public License\n");
  fxmessage("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
  }


// Parse line and column following filename, if any
static FXString parseFileAndLocation(const FXString& string,FXint& line,FXint& col){
  FXString result('\0',1024);
  line=col=0;
  if(string.scan("%1023[^:]:%d:%d",result.text(),&line,&col)==3){
    return result;
    }
  if(string.scan("%1023[^:]:%d",result.text(),&line)==2){
    return result;
    }
  return string;
  }


// Start the application
FXint Adie::start(int argc,char** argv){
  FXString    name,execpath,iconpath,syntaxfile;
  FXString    file(FXPath::absolute("untitled"));
  TextWindow *window=nullptr;
  Syntax     *syntax=nullptr;
  FXbool      edit=true;
  FXint       geom=0;
  FXint       winx=0;
  FXint       winy=0;
  FXint       winw=0;
  FXint       winh=0;
  FXint       line=0;
  FXint       col=0;
  FXint       arg=1;

  // The registry has been loaded after this
  init(argc,argv);

  // Make a tool tip
  new FXToolTip(this,0);

  // Create it
  create();

  // Exec path is default for syntax path
  execpath=FXSystem::getExecPath();

  FXTRACE((11,"execpath=%s\n",execpath.text()));

  // Get icon search path
  iconpath=reg().readStringEntry("SETTINGS","iconpath",FXIconCache::defaultIconPath);

  FXTRACE((11,"iconpath=%s\n",iconpath.text()));

  // Change icon search path
  associations->setIconPath(iconpath);

  // See if override paths are provided in the registry
  syntaxpaths=reg().readStringEntry("SETTINGS","syntaxpaths",execpath.text());

  FXTRACE((11,"syntaxpaths=%s\n",syntaxpaths.text()));

  // Look for syntax file in the syntax paths
  syntaxfile=FXPath::search(syntaxpaths,"Adie.stx");

  // Global options
  while(arg<argc){
    if(FXString::compare(argv[arg],"-h")==0){
      printusage();
      return 0;
      }
    if(FXString::compare(argv[arg],"--help")==0){
      printusage();
      return 0;
      }
    if(FXString::compare(argv[arg],"--version")==0){
      printversion();
      return 0;
      }
    if(FXString::compare(argv[arg],"--syntax=",9)==0){
      syntaxfile=argv[arg]+9;
      arg++;
      continue;
      }
    break;
    }

  FXTRACE((11,"syntaxfile=%s\n",syntaxfile.text()));

  // Load syntax file
  if(!syntaxfile.empty()){
    if(!SyntaxParser::parseFile(syntaxes,syntaxfile)){
      fxwarning("Adie: unable to parse syntax file: %s.\n",syntaxfile.text());
      }
    }

  // Load filenames
  while(arg<argc){

    // File viewing only
    if(FXString::compare(argv[arg],"--view")==0){
      edit=false;
      arg++;
      continue;
      }

    // File viewing and editing
    if(FXString::compare(argv[arg],"--edit")==0){
      edit=true;
      arg++;
      continue;
      }

    // Get language
    if(FXString::compare(argv[arg],"--lang=",7)==0){
      syntax=getSyntaxByName(argv[arg]+7);
      arg++;
      continue;
      }

    // Window size
    if(FXString::compare(argv[arg],"--size=",7)==0){
      geom=fxparsegeometry(argv[arg]+7,winx,winy,winw,winh);
      arg++;
      continue;
      }

    // Get line number
    if(FXString::compare(argv[arg],"--line=",7)==0){
      sscanf(argv[arg]+7,"%d",&line);
      arg++;
      continue;
      }

    // Get column number
    if(FXString::compare(argv[arg],"--column=",9)==0){
      sscanf(argv[arg]+9,"%d",&col);
      arg++;
      continue;
      }

    // Parse optional line and column from end of filename
    name=parseFileAndLocation(argv[arg],line,col);

    FXTRACE((11,"name=%s:%d:%d\n",file.text(),line,col));

    // Get absolute filename
    file=FXPath::absolute(name);

    FXTRACE((11,"file=%s\n",file.text()));

    // Open window
    window=openFileWindow(file,edit);

    // Force position and/or size
    if(geom){
      if(!(geom&1)) winx=window->getX();
      if(!(geom&2)) winy=window->getY();
      if(!(geom&4)) winw=window->getWidth();
      if(!(geom&8)) winh=window->getHeight();
      if(winx<0) winx+=window->getRoot()->getWidth()-winw;
      if(winy<0) winy+=window->getRoot()->getHeight()-winh;
      window->position(winx,winy,winw,winh);
      }

    // Override language mode?
    if(syntax){
      window->setSyntax(syntax);
      }

    // Jump to desired line:column
    if(line){
      window->visitLine(line,col);
      }

    // Pop it up
    window->show(PLACEMENT_DEFAULT);

    // Reset for next
    edit=true;
    geom=0;
    winx=0;
    winy=0;
    winw=0;
    winh=0;
    line=0;
    col=0;
    arg++;
    }

  // If no window loaded, make empty one
  if(!window){

    // New window
    window=new TextWindow(this);
    window->create();

    // Compute absolute path
    window->setFilename(file);
    window->setFilenameSet(false);
    window->setBrowserCurrentFile(file);

    // Force position and/or size
    if(geom){
      if(!(geom&1)) winx=window->getX();
      if(!(geom&2)) winy=window->getY();
      if(!(geom&4)) winw=window->getWidth();
      if(!(geom&8)) winh=window->getHeight();
      if(winx<0) winx+=window->getRoot()->getWidth()-winw;
      if(winy<0) winy+=window->getRoot()->getHeight()-winh;
      window->position(winx,winy,winw,winh);
      }

    // Override language mode?
    if(syntax){
      window->setSyntax(syntax);
      }

    // Pop it up
    window->show(PLACEMENT_DEFAULT);
    }

  // Now run
  return run();
  }

