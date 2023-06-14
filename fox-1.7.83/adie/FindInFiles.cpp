/********************************************************************************
*                                                                               *
*                    F i n d   P a t t e r n   I n   F i l e s                  *
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
#include "icons.h"
#include "help.h"
#include "Preferences.h"
#include "Commands.h"
#include "Syntax.h"
#include "Modeline.h"
#include "TextWindow.h"
#include "Adie.h"
#include "FindInFiles.h"

/*
  Notes:

  - Need exclusion clauses: don't scan files with certain extensions, don't
    scan files that exceed certain size, don't scan files whose first chunk
    of data matches certain pattern (e.g. "PNG" or "GIF89a", etc).
  - Probably need to make this standard re-usable widget.
  - Keep registry entry for size & position of dialog.
  - Should remember current file pattern for file filter, instead of having
    to set it each time; filter to search should be different from current
    filter of TextWindow.
  - Remembering current pattern and search text would be nice, for repeated
    invocations.  Maybe some more tweaks.
*/

#define HORZ_PAD      12
#define VERT_PAD      2

/*******************************************************************************/

// Map
FXDEFMAP(FindInFiles) FindInFilesMap[]={
  FXMAPFUNC(SEL_UPDATE,FindInFiles::ID_STOP,FindInFiles::onUpdStop),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_STOP,FindInFiles::onCmdStop),
  FXMAPFUNC(SEL_UPDATE,FindInFiles::ID_PAUSE,FindInFiles::onUpdPause),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_PAUSE,FindInFiles::onCmdPause),
  FXMAPFUNC(SEL_UPDATE,FindInFiles::ID_SEARCH,FindInFiles::onUpdSearch),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_SEARCH,FindInFiles::onCmdSearch),
  FXMAPFUNC(SEL_UPDATE,FindInFiles::ID_DELETE,FindInFiles::onUpdDelete),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_DELETE,FindInFiles::onCmdDelete),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_FOLDER,FindInFiles::onCmdFolder),
  FXMAPFUNC(SEL_UPDATE,FindInFiles::ID_HIST_UP,FindInFiles::onUpdHistoryUp),
  FXMAPFUNC(SEL_UPDATE,FindInFiles::ID_HIST_DN,FindInFiles::onUpdHistoryDn),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_HIST_UP,FindInFiles::onCmdHistoryUp),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_HIST_DN,FindInFiles::onCmdHistoryDn),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_FILTER_TEXT,FindInFiles::onCmdFilter),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_SEARCH_TEXT,FindInFiles::onCmdSearch),
  FXMAPFUNC(SEL_KEYPRESS,FindInFiles::ID_SEARCH_TEXT,FindInFiles::onArrowKey),
  FXMAPFUNC(SEL_MOUSEWHEEL,FindInFiles::ID_SEARCH_TEXT,FindInFiles::onMouseWheel),
  FXMAPFUNC(SEL_DOUBLECLICKED,FindInFiles::ID_FILELIST,FindInFiles::onCmdFileDblClicked),
  FXMAPFUNC(SEL_UPDATE,FindInFiles::ID_FIRST_HIT,FindInFiles::onUpdFirstHit),
  FXMAPFUNC(SEL_COMMAND,FindInFiles::ID_FIRST_HIT,FindInFiles::onCmdFirstHit),
  FXMAPFUNCS(SEL_UPDATE,FindInFiles::ID_ICASE,FindInFiles::ID_HIDDEN,FindInFiles::onUpdFlags),
  FXMAPFUNCS(SEL_COMMAND,FindInFiles::ID_ICASE,FindInFiles::ID_HIDDEN,FindInFiles::onCmdFlags),
  };


// Object implementation
FXIMPLEMENT(FindInFiles,FXDialogBox,FindInFilesMap,ARRAYNUMBER(FindInFilesMap))


// Search and replace dialog registry section name
static const FXchar sectionName[]="Find In Files";


// Registry keys for search strings
static const FXchar skey[20][3]={
  "SA","SB","SC","SD","SE","SF","SG","SH","SI","SJ","SK","SL","SM","SN","SO","SP","SQ","SR","SS","ST"
  };


// Registry keys for folder strings
static const FXchar fkey[20][3]={
  "FA","FB","FC","FD","FE","FF","FG","FH","FI","FJ","FK","FL","FM","FN","FO","FP","FQ","FR","FS","FT"
  };


// Registry keys for file pattern options
static const FXchar pkey[20][3]={
  "PA","PB","PC","PD","PE","PF","PG","PH","PI","PJ","PK","PL","PM","PN","PO","PP","PQ","PR","PS","PT"
  };


// Registry keys for search mode options
static const FXchar mkey[20][3]={
  "MA","MB","MC","MD","ME","MF","MG","MH","MI","MJ","MK","ML","MM","MN","MO","MP","MQ","MR","MS","MT"
  };


/*******************************************************************************/

// Traverse files under path and search for pattern in each
FXuint SearchVisitor::traverse(const FXString& path,const FXString& pattern,const FXString& wild,FXint mode,FXuint opts,FXint depth){
  FXTRACE((1,"SearchVisitor::traverse(path=%s,pattern=%s,wild=%s,mode=%b,opts=%b,depth=%d)\n",path.text(),pattern.text(),wild.text(),mode,opts,depth));

  // Compile the pattern
  if(rex.parse(pattern,mode)==FXRex::ErrOK){

    // Sensitize controls; in case GUI-update can't get in sideways
    dlg->forceRefresh();

    // Traverse directory and visit the files
    return FXGlobVisitor::traverse(path,wild,opts,depth);
    }
  return 0;
  }


// Set file size limit
void SearchVisitor::setLimit(FXlong size){
  limit=FXCLAMP(1,size,2147483647);
  }


// Visit file
FXuint SearchVisitor::visit(const FXString& path){
  if(!dlg->continueProcessing()) return 2;
  if(FXGlobVisitor::visit(path)){
    if(searchFile(path)) return 1;
    }
  return 0;
  }


// Search file contents for pattern
FXint SearchVisitor::searchFile(const FXString& path) const {
  FXString text;
  FXTRACE((1,"searchFile(path=%s)\n",path.text()));
  if(loadFile(path,text)){
    FXString relpath=FXPath::relative(dlg->getDirectory(),path);
    FXString hit;
    FXint beg[10],end[10],ls,le,p;
    FXint lineno=1;
    FXint column=0;
    FXint pos=0;
    FXTRACE((1,"loadFile(path=%s) -> %d bytes\n",relpath.text(),text.length()));
    dlg->setSearchingText(relpath);
    while(pos<text.length()){
      if(rex.amatch(text,pos,FXRex::Normal,beg,end,10)){
        for(ls=beg[0]; 0<ls && text[ls-1]!='\n'; --ls){ }               // Back up to line start
        for(le=beg[0]; le<text.length() && text[le]!='\n'; ++le){ }     // Advance to line end
        for(p=ls,column=0; p<beg[0]; p=text.inc(p)){                    // Count columns, assuming for now tabs are set at 8
          column+=(text[p]=='\t')?8-column%8:1;
          }
        hit.assign(&text[ls],le-ls);
        hit.trim();
        dlg->appendSearchResult(relpath,hit,lineno,column);
        if(dlg->getFirstHit()) break;
        pos=le;
        }
      lineno+=(text[pos++]=='\n');              // Moved to next line
      }
    }
  return 0;
  }


// Load file contents
FXlong SearchVisitor::loadFile(const FXString& path,FXString& text) const {
  FXFile file(path,FXFile::Reading);
  if(file.isOpen()){
    FXlong size=file.size();
    if(0<size && size<=limit){
      if(text.length(size)){
        return file.readBlock(text.text(),text.length());
        }
      }
    }
  return 0L;
  }

/*******************************************************************************/

// For deserialization
FindInFiles::FindInFiles():visitor(this){
  locations=nullptr;
  findstring=nullptr;
  filefolder=nullptr;
  filefilter=nullptr;
  pausebutton=nullptr;
  searching=nullptr;
  clearElms(optionsHistory,20);
  clearElms(patternHistory,20);
  searchmode=SearchExact|SearchRecurse;
  savedsearchmode=0;
  savedcurrentpattern=0;
  index=-1;
  proceed=1;
  firsthit=false;
  }


// Construct file in files dialog
FindInFiles::FindInFiles(Adie *a):FXDialogBox(a,"Find In Files",DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE,0,0,600,400, 10,10,10,10, 10,10),visitor(this){

  // Buttons at bottom
  FXHorizontalFrame* bottomline=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X,0,0,0,0,0,0,0,0);
  new FXLabel(bottomline,tr("Searching: "),nullptr,LAYOUT_CENTER_Y|LAYOUT_LEFT|JUSTIFY_RIGHT,0,0,0,0,2,2,VERT_PAD,VERT_PAD);
  searching=new FXLabel(bottomline,tr("<stopped>"),nullptr,LAYOUT_CENTER_Y|LAYOUT_FILL_X|JUSTIFY_LEFT,0,0,0,0,2,2,VERT_PAD,VERT_PAD);
  FXHorizontalFrame* buttons=new FXHorizontalFrame(bottomline,LAYOUT_RIGHT|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT,0,0,0,0,0,0,0,0);
  new FXButton(buttons,tr("&Close"),nullptr,this,ID_CLOSE,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_RIGHT,0,0,0,0,6,6,VERT_PAD,VERT_PAD);
  new FXButton(buttons,tr("&Delete"),nullptr,this,ID_DELETE,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_RIGHT,0,0,0,0,HORZ_PAD,HORZ_PAD,VERT_PAD,VERT_PAD);
  new FXButton(buttons,tr("Stop"),nullptr,this,ID_STOP,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_RIGHT,0,0,0,0,HORZ_PAD,HORZ_PAD,VERT_PAD,VERT_PAD);
  pausebutton=new FXToggleButton(buttons,tr("Pause"),tr("Resume"),nullptr,nullptr,this,ID_PAUSE,FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_RIGHT,0,0,0,0,HORZ_PAD,HORZ_PAD,VERT_PAD,VERT_PAD);
  new FXButton(buttons,tr("&Search"),nullptr,this,ID_SEARCH,BUTTON_INITIAL|BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_RIGHT,0,0,0,0,HORZ_PAD,HORZ_PAD,VERT_PAD,VERT_PAD);

  // Above button block
  new FXHorizontalSeparator(this,SEPARATOR_GROOVE|LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);

  // Options block
  FXHorizontalFrame* frame=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT,0,0,0,0,0,0,0,0);
  new FXCheckButton(frame,tr("E&xpression\tRegular Expression"),this,ID_REGEX,ICON_BEFORE_TEXT|LAYOUT_CENTER_X);
  new FXCheckButton(frame,tr("W&ords\tWhole Words"),this,ID_WORDS,ICON_BEFORE_TEXT|LAYOUT_CENTER_X);
  new FXCheckButton(frame,tr("&Ignore Case\tCase insensitive"),this,ID_ICASE,ICON_BEFORE_TEXT|LAYOUT_CENTER_X);
  new FXCheckButton(frame,tr("&Recursive\tSearch subdirectories"),this,ID_RECURSIVE,ICON_BEFORE_TEXT|LAYOUT_CENTER_X);
  new FXCheckButton(frame,tr("&Hidden Files\tSearch hidden files"),this,ID_HIDDEN,ICON_BEFORE_TEXT|LAYOUT_CENTER_X);
  new FXCheckButton(frame,tr("First Hi&t\tRecord only first matches for each file"),this,ID_FIRST_HIT,ICON_BEFORE_TEXT|LAYOUT_CENTER_X);

  // Entry block
  FXMatrix *matrix=new FXMatrix(this,3,MATRIX_BY_COLUMNS|LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X,0,0,0,0,0,0,0,0);

  // Text field with history
  new FXLabel(matrix,tr("Search &for:"),nullptr,JUSTIFY_RIGHT|LAYOUT_FILL_X|LAYOUT_CENTER_Y);
  FXHorizontalFrame* searchbox=new FXHorizontalFrame(matrix,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN,0,0,0,0, 0,0,0,0, 0,0);
  findstring=new FXTextField(searchbox,26,this,ID_SEARCH_TEXT,TEXTFIELD_ENTER_ONLY|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  findstring->setTipText(tr("Search Pattern"));
  FXVerticalFrame* searcharrows=new FXVerticalFrame(searchbox,LAYOUT_RIGHT|LAYOUT_FILL_Y|PACK_UNIFORM_HEIGHT,0,0,0,0, 0,0,0,0, 0,0);
  FXArrowButton* ar1=new FXArrowButton(searcharrows,this,ID_HIST_UP,FRAME_RAISED|FRAME_THICK|ARROW_UP|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 1,1,1,1);
  FXArrowButton* ar2=new FXArrowButton(searcharrows,this,ID_HIST_DN,FRAME_RAISED|FRAME_THICK|ARROW_DOWN|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 1,1,1,1);
  ar1->setArrowSize(5);
  ar2->setArrowSize(5);
  new FXFrame(matrix,0);

  // Folder to search
  new FXLabel(matrix,tr("In Fo&lder:"),nullptr,JUSTIFY_RIGHT|LAYOUT_FILL_X|LAYOUT_CENTER_Y);
  filefolder=new FXTextField(matrix,40,this,ID_FOLDER_TEXT,JUSTIFY_LEFT|FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN|LAYOUT_FILL_X);
  filefolder->setTipText(tr("Folder To Search"));
  new FXButton(matrix,"...\tSelect Folder\tSelect folder to search.",nullptr,this,ID_FOLDER,LAYOUT_CENTER_Y|FRAME_RAISED|FRAME_THICK|LAYOUT_FIX_WIDTH,0,0,20,0);

  // Filter for files
  new FXLabel(matrix,tr("Filt&er:"),nullptr,JUSTIFY_RIGHT|LAYOUT_FILL_X|LAYOUT_CENTER_Y);
  filefilter=new FXComboBox(matrix,10,this,ID_FILTER_TEXT,COMBOBOX_STATIC|LAYOUT_FILL_X|LAYOUT_CENTER_Y|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN);
  filefilter->setTipText(tr("Files matching wildcard"));
  filefilter->setNumVisible(4);
  new FXFrame(matrix,0);

  // Matching files
  FXHorizontalFrame* resultbox=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0,0,0,0,0);
  locations=new FXIconList(resultbox,this,ID_FILELIST,LAYOUT_FILL_X|LAYOUT_FILL_Y|ICONLIST_DETAILED|ICONLIST_SINGLESELECT);
  locations->appendHeader(tr("Location"),nullptr,200);
  locations->appendHeader(tr("Context"),nullptr,800);

  // Clean array
  clearElms(optionsHistory,20);
  clearElms(patternHistory,20);

  // Set title
  setTitle(tr("Find In Files"));

  // Set icons
  setIcon(getApp()->bigicon);
  setMiniIcon(getApp()->smallicon);

  // Initial pattern
  setPatternList(tr("All Files (*)"));

  // Start with this
  filePattern="*";

  // Search flags
  searchmode=SearchExact|SearchRecurse;
  savedsearchmode=0;
  savedcurrentpattern=0;
  index=-1;
  proceed=1;
  firsthit=false;
  }


// Load registy
void FindInFiles::readRegistry(){
  FXTRACE((1,"FindInFiles::readRegistry()\n"));
  setWidth(getApp()->reg().readIntEntry(sectionName,"width",600));
  setHeight(getApp()->reg().readIntEntry(sectionName,"height",400));
  firsthit=getApp()->reg().readBoolEntry(sectionName,"firsthit",false);
  locations->setHeaderSize(0,getApp()->reg().readIntEntry(sectionName,"filespace",200));
  locations->setHeaderSize(1,getApp()->reg().readIntEntry(sectionName,"matchspace",800));
  setCurrentPattern(getApp()->reg().readIntEntry(sectionName,"searchpattern",0));
  for(FXint i=0; i<20; ++i){
    searchHistory[i]=getApp()->reg().readStringEntry(sectionName,skey[i],FXString::null);
    if(searchHistory[i].empty()) break;
    folderHistory[i]=getApp()->reg().readStringEntry(sectionName,fkey[i],FXString::null);
    patternHistory[i]=getApp()->reg().readUIntEntry(sectionName,pkey[i],0);
    optionsHistory[i]=getApp()->reg().readUIntEntry(sectionName,mkey[i],SearchExact);
    }
  }


// Save registry
void FindInFiles::writeRegistry(){
  FXTRACE((1,"FindInFiles::writeRegistry()\n"));
  getApp()->reg().writeIntEntry(sectionName,"width",getWidth());
  getApp()->reg().writeIntEntry(sectionName,"height",getHeight());
  getApp()->reg().writeBoolEntry(sectionName,"firsthit",firsthit);
  getApp()->reg().writeIntEntry(sectionName,"filespace",locations->getHeaderSize(0));
  getApp()->reg().writeIntEntry(sectionName,"matchspace",locations->getHeaderSize(1));
  getApp()->reg().writeIntEntry(sectionName,"searchpattern",getCurrentPattern());
  for(FXint i=0; i<20; ++i){
    if(!searchHistory[i].empty()){
      getApp()->reg().writeStringEntry(sectionName,skey[i],searchHistory[i].text());
      getApp()->reg().writeStringEntry(sectionName,fkey[i],folderHistory[i].text());
      getApp()->reg().writeUIntEntry(sectionName,pkey[i],patternHistory[i]);
      getApp()->reg().writeUIntEntry(sectionName,mkey[i],optionsHistory[i]);
      }
    else{
      getApp()->reg().deleteEntry(sectionName,skey[i]);
      getApp()->reg().deleteEntry(sectionName,fkey[i]);
      getApp()->reg().deleteEntry(sectionName,pkey[i]);
      getApp()->reg().deleteEntry(sectionName,mkey[i]);
      }
    }
  }


// Add string to history buffer
void FindInFiles::appendHistory(const FXString& text,const FXString& dir,FXuint patt,FXuint opts){
  if(!text.empty()){
    if(text!=searchHistory[0]){
      for(FXint i=19; i>0; i--){
        swap(searchHistory[i],searchHistory[i-1]);
        swap(folderHistory[i],folderHistory[i-1]);
        swap(patternHistory[i],patternHistory[i-1]);
        swap(optionsHistory[i],optionsHistory[i-1]);
        }
      }
    searchHistory[0]=text;
    folderHistory[0]=dir;
    patternHistory[0]=patt;
    optionsHistory[0]=opts;
    index=0;
    }
  }


// Create server-side resources
void FindInFiles::create(){
  readRegistry();
  FXDialogBox::create();
  findstring->setFocus();
  }


// Close window
FXbool FindInFiles::close(FXbool notify){
  writeRegistry();
  return FXDialogBox::close(notify);
  }


// Called by visitor to see if we should continue processing
FXbool FindInFiles::continueProcessing(){
  getApp()->refresh();
  getApp()->runModalWhileEvents(pausebutton,1000000L);
  getApp()->runUntil(proceed);
  return (proceed!=2);
  }


// Called by visitor to deposit new search result
// List will show filename relativized to base name, and location of where string was found
void FindInFiles::appendSearchResult(const FXString& relpath,const FXString& text,FXint lineno,FXint column){
  FXString string;
  string.format("%s:%d:%d\t%s",relpath.text(),lineno,column,text.text());
  locations->appendItem(string);
  }


// Clear search results
void FindInFiles::clearSearchResults(){
  locations->clearItems();
  }


// Change directory
void FindInFiles::setDirectory(const FXString& path){
  filefolder->setText(path);
  }


// Return directory
FXString FindInFiles::getDirectory() const {
  return filefolder->getText();
  }


// Set text or pattern to search for
void FindInFiles::setSearchText(const FXString& text){
  findstring->setText(text);
  }


// Return text or pattern the user has entered
FXString FindInFiles::getSearchText() const {
  return findstring->getText();
  }


// Change file pattern
void FindInFiles::setPattern(const FXString& ptrn){
  filefilter->setText(ptrn);
  filePattern=ptrn;
  }


// Set list of patterns
void FindInFiles::setPatternList(const FXString& patterns){
  filefilter->clearItems();
  filefilter->fillItems(patterns);
  if(!filefilter->getNumItems()) filefilter->appendItem(tr("All Files (*)"));
  filefilter->setNumVisible(Math::imin(filefilter->getNumItems(),12));
  setCurrentPattern(0);
  }


// Return list of patterns
FXString FindInFiles::getPatternList() const {
  FXString pat;
  for(FXint i=0; i<filefilter->getNumItems(); i++){
    if(!pat.empty()) pat+='\n';
    pat+=filefilter->getItemText(i);
    }
  return pat;
  }


// Set currently selected pattern
void FindInFiles::setCurrentPattern(FXint patno){
  patno=FXCLAMP(0,patno,filefilter->getNumItems()-1);
  filefilter->setCurrentItem(patno);
  filePattern=FXFileSelector::patternFromText(filefilter->getItemText(patno));
  }


// Return current pattern number
FXint FindInFiles::getCurrentPattern() const {
  return filefilter->getCurrentItem();
  }


// Change pattern text for pattern number
void FindInFiles::setPatternText(FXint patno,const FXString& text){
  if(patno<0 || patno>=filefilter->getNumItems()){ fxerror("%s::setPatternText: index out of range.\n",getClassName()); }
  filefilter->setItemText(patno,text);
  if(patno==filefilter->getCurrentItem()){
    filePattern=FXFileSelector::patternFromText(text);
    }
  }


// Get pattern text for given pattern number
FXString FindInFiles::getPatternText(FXint patno) const {
  if(patno<0 || patno>=filefilter->getNumItems()){ fxerror("%s::getPatternText: index out of range.\n",getClassName()); }
  return filefilter->getItemText(patno);
  }


// Return number of patterns
FXint FindInFiles::getNumPatterns() const {
  return filefilter->getNumItems();
  }


// Allow pattern entry
void FindInFiles::allowPatternEntry(FXbool flag){
  filefilter->setComboStyle(flag?COMBOBOX_NORMAL:COMBOBOX_STATIC);
  }


// Return true if pattern entry is allowed
FXbool FindInFiles::allowPatternEntry() const {
  return (filefilter->getComboStyle()!=COMBOBOX_STATIC);
  }


// Update file being scanned
void FindInFiles::setSearchingText(const FXString& fn){
  searching->setText(fn);
  searching->repaint();
  getApp()->flush();
  }


// Read it back
FXString FindInFiles::getSearchingText() const {
  return searching->getText();
  }

/*******************************************************************************/

// Update stop button
long FindInFiles::onUpdStop(FXObject* sender,FXSelector,void*){
  sender->handle(this,proceed?FXSEL(SEL_COMMAND,ID_DISABLE):FXSEL(SEL_COMMAND,ID_ENABLE),nullptr);
  return 1;
  }


// Stop scanning disk
long FindInFiles::onCmdStop(FXObject*,FXSelector,void*){
  proceed=2;
  return 1;
  }


// Update pause/resume button
long FindInFiles::onUpdPause(FXObject* sender,FXSelector,void*){
  sender->handle(this,(proceed==0)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  sender->handle(this,visitor.visiting()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Pause/resume
long FindInFiles::onCmdPause(FXObject*,FXSelector,void*){
  proceed=(proceed==1)?0:1;
  return 1;
  }


// Grey out delete if no results
long FindInFiles::onUpdDelete(FXObject* sender,FXSelector,void*){
  sender->handle(this,locations->getNumItems()?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Delete results
long FindInFiles::onCmdDelete(FXObject*,FXSelector,void*){
  clearSearchResults();
  return 1;
  }


// Grey out buttons if no search text
long FindInFiles::onUpdSearch(FXObject* sender,FXSelector,void*){
  FXbool enabled=!visitor.visiting() && !findstring->getText().empty();
  sender->handle(this,enabled?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Start search
long FindInFiles::onCmdSearch(FXObject*,FXSelector,void*){
  FXuint opts=FXDir::AllDirs;
  FXint rexmode=FXRex::Capture;
  FXint limit=1000;
  if(getSearchMode()&SearchCaseFold) rexmode|=FXRex::IgnoreCase;                // Case insensitivity
  if(getSearchMode()&SearchWords) rexmode|=FXRex::Words;                         // Whole Words
  if(!(getSearchMode()&SearchRegex)) rexmode|=FXRex::Verbatim;                  // Verbatim match
  if(getSearchMode()&SeachHidden) opts|=FXDir::HiddenFiles|FXDir::HiddenDirs;   // Visit hidden files and directories
  if(!(getSearchMode()&SearchRecurse)) limit=2;                                 // Don't recurse
  appendHistory(getSearchText(),getDirectory(),getCurrentPattern(),getSearchMode());
  proceed=1;
  visitor.traverse(getDirectory(),getSearchText(),getPattern(),rexmode,opts,limit);
  setSearchingText(tr("<stopped>"));
  getApp()->refresh();
  return 1;
  }

/*******************************************************************************/

// Change the pattern; change the filename to the suggested extension
long FindInFiles::onCmdFilter(FXObject*,FXSelector,void* ptr){
  filePattern=FXFileSelector::patternFromText((FXchar*)ptr);
  return 1;
  }


// Update flags
long FindInFiles::onUpdFlags(FXObject* sender,FXSelector sel,void*){
  FXuint value=0;
  switch(FXSELID(sel)){
    case ID_ICASE: value=(searchmode&SearchCaseFold); break;
    case ID_REGEX: value=(searchmode&SearchRegex); break;
    case ID_WORDS: value=(searchmode&SearchWords); break;
    case ID_RECURSIVE: value=(searchmode&SearchRecurse); break;
    case ID_HIDDEN: value=(searchmode&SeachHidden); break;
    }
  sender->handle(this,value?FXSEL(SEL_COMMAND,FXWindow::ID_CHECK):FXSEL(SEL_COMMAND,FXWindow::ID_UNCHECK),nullptr);
  return 1;
  }


// Handle flags
long FindInFiles::onCmdFlags(FXObject*,FXSelector sel,void*){
  switch(FXSELID(sel)){
    case ID_ICASE: searchmode^=SearchCaseFold; break;
    case ID_REGEX: searchmode^=SearchRegex; break;
    case ID_WORDS: searchmode^=SearchWords; break;
    case ID_RECURSIVE: searchmode^=SearchRecurse; break;
    case ID_HIDDEN: searchmode^=SeachHidden; break;
    }
  return 1;
  }


// Update all hits check button
long FindInFiles::onUpdFirstHit(FXObject* sender,FXSelector,void*){
  sender->handle(this,firsthit?FXSEL(SEL_COMMAND,FXWindow::ID_CHECK):FXSEL(SEL_COMMAND,FXWindow::ID_UNCHECK),nullptr);
  return 1;
  }


// Change all hits flag
long FindInFiles::onCmdFirstHit(FXObject*,FXSelector,void*){
  firsthit=!firsthit;
  return 1;
  }


// Set directory to search in
long FindInFiles::onCmdFolder(FXObject*,FXSelector,void*){
  FXString path=FXFileDialog::getOpenDirectory(this,tr("Search In Folder"),getDirectory());
  if(!path.empty()){
    setDirectory(path);
    }
  return 1;
  }


// Update arrows
long FindInFiles::onUpdHistoryUp(FXObject* sender,FXSelector,void*){
  sender->handle(this,(index<19 && !searchHistory[index+1].empty())?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Update arrows
long FindInFiles::onUpdHistoryDn(FXObject* sender,FXSelector,void*){
  sender->handle(this,(-1<index)?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  return 1;
  }


// Scroll back in search history
long FindInFiles::onCmdHistoryUp(FXObject*,FXSelector,void*){
  if(index<19 && !searchHistory[index+1].empty()){
    ++index;
    if(index==0){
      savedsearchtext=getSearchText();
      savedsearchfolder=getDirectory();
      savedsearchmode=getSearchMode();
      savedcurrentpattern=getCurrentPattern();
      }
    setSearchText(searchHistory[index]);
    setDirectory(folderHistory[index]);
    setCurrentPattern(patternHistory[index]);
    setSearchMode(optionsHistory[index]);
    }
  else{
    getApp()->beep();
    }
  return 1;
  }


// Scroll back in search history
long FindInFiles::onCmdHistoryDn(FXObject*,FXSelector,void*){
  if(0<=index){
    --index;
    if(0<=index){
      setSearchText(searchHistory[index]);
      setDirectory(folderHistory[index]);
      setCurrentPattern(patternHistory[index]);
      setSearchMode(optionsHistory[index]);
      }
    else{
      setSearchText(savedsearchtext);
      setDirectory(savedsearchfolder);
      setSearchMode(savedsearchmode);
      setCurrentPattern(savedcurrentpattern);
      }
    }
  return 1;
  }


// Hitting arrows in text field scrolls through history
long FindInFiles::onArrowKey(FXObject*,FXSelector,void* ptr){
  switch(((FXEvent*)ptr)->code){
    case KEY_Up:
    case KEY_KP_Up:
      return onCmdHistoryUp(this,FXSEL(SEL_COMMAND,ID_HIST_UP),nullptr);
    case KEY_Down:
    case KEY_KP_Down:
      return onCmdHistoryDn(this,FXSEL(SEL_COMMAND,ID_HIST_DN),nullptr);
    }
  return 0;
  }


// Wheeling in text field scrolls through history
long FindInFiles::onMouseWheel(FXObject*,FXSelector,void* ptr){
  if(((FXEvent*)ptr)->code>0){
    return onCmdHistoryUp(this,FXSEL(SEL_COMMAND,ID_HIST_UP),nullptr);
    }
  if(((FXEvent*)ptr)->code<0){
    return onCmdHistoryDn(this,FXSEL(SEL_COMMAND,ID_HIST_DN),nullptr);
    }
  return 1;
  }


// File list double clicked
long FindInFiles::onCmdFileDblClicked(FXObject*,FXSelector,void* ptr){
  FXint which=(FXint)(FXival)ptr;
  if(0<=which){
    FXchar name[1024];
    FXint  lineno=0;
    FXint  column=0;
    if(locations->getItem(which)->getText().scan("%1023[^:]:%d:%d",name,&lineno,&column)==3){
      FXString filename=FXPath::absolute(getDirectory(),name);
      TextWindow* window=getApp()->openFileWindow(filename);
      window->visitLine(lineno,column);
      window->show(PLACEMENT_DEFAULT);
      }
    }
  return 1;
  }

/*******************************************************************************/

// Clean up
FindInFiles::~FindInFiles(){
  locations=(FXIconList*)-1L;
  findstring=(FXTextField*)-1L;
  filefolder=(FXTextField*)-1L;
  filefilter=(FXComboBox*)-1L;
  pausebutton=(FXToggleButton*)-1L;
  searching=(FXLabel*)-1L;
  }
