/********************************************************************************
*                                                                               *
*                     S h e l l   C o m m a n d   D i a l o g                   *
*                                                                               *
*********************************************************************************
* Copyright (C) 2023 by Jeroen van der Zijp.   All Rights Reserved.             *
*********************************************************************************
* This library is free software; you can redistribute it and/or modify          *
* it under the terms of the GNU Lesser General Public License as published by   *
* the Free Software Foundation; either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU Lesser General Public License for more details.                           *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public License      *
* along with this program.  If not, see <http://www.gnu.org/licenses/>          *
********************************************************************************/
#include "fx.h"
#include "icons.h"
#include "ShellDialog.h"

/*
  Notes:

  - Collect a command from the user, remembering the last few commands for
    next time in the history buffer.
  - If a new command was entered, save it to the history, you can scroll
    back into history and re-execute it again w/o retyping it.
  - Some special sequences are replaced in the command string:

         %f           Replaced by (quoted) current full path name
         %F           Replaced by the (quoted) selected full path names
         %n           Replaced by the (quoted) current file name
         %N           Replaced by the (quoted) selected file names
         %u           Replaced by (quoted) URL-encoded of the current filename
         %U           Replaced by the (quoted) URL-encoded selected filenames
         %d           Replaced by current working directory
         %%           Replaced by '%'

  - This is similar to what PathFinder does; we love consistency...
*/

// Padding for buttons
#define HORZ_PAD 20
#define VERT_PAD 2

using namespace FX;

/*******************************************************************************/

namespace FX {

// Section key
const FXchar hisSection[]="Command History";

// String keys
static const FXchar hisKey[20][3]={
  "HA","HB","HC","HD","HE","HF","HG","HH","HI","HJ",
  "HK","HL","HM","HN","HO","HP","HQ","HR","HH","HT"
  };



// Map
FXDEFMAP(ShellDialog) ShellDialogMap[]={
  FXMAPFUNC(SEL_COMMAND,ShellDialog::ID_ACCEPT,ShellDialog::onCmdAccept),
  FXMAPFUNC(SEL_KEYPRESS,ShellDialog::ID_ACCEPT,ShellDialog::onInputKey),
  FXMAPFUNC(SEL_MOUSEWHEEL,ShellDialog::ID_ACCEPT,ShellDialog::onHistWheel),
  FXMAPFUNC(SEL_COMMAND,ShellDialog::ID_HISTORY_UP,ShellDialog::onCmdHistUp),
  FXMAPFUNC(SEL_COMMAND,ShellDialog::ID_HISTORY_DN,ShellDialog::onCmdHistDn),
  };


// Object implementation
FXIMPLEMENT(ShellDialog,FXDialogBox,ShellDialogMap,ARRAYNUMBER(ShellDialogMap))

// Create input dialog box
ShellDialog::ShellDialog(FXWindow* own,const FXString& name,const FXString& label):FXDialogBox(own,name,DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE,0,0,0,0,10,10,10,10, 10,10){
  FXHorizontalFrame* buttons=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0,0,0,0,0);
  new FXButton(buttons,tr("&OK"),nullptr,this,ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_RIGHT,0,0,0,0,HORZ_PAD,HORZ_PAD,VERT_PAD,VERT_PAD);
  new FXButton(buttons,tr("&Cancel"),nullptr,this,ID_CANCEL,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_RIGHT,0,0,0,0,HORZ_PAD,HORZ_PAD,VERT_PAD,VERT_PAD);
  new FXHorizontalSeparator(this,SEPARATOR_GROOVE|LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
  FXHorizontalFrame* toppart=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0, 10,10);
  cmdicon=new FXBMPIcon(getApp(),command_bmp,0,IMAGE_ALPHAGUESS);
  new FXLabel(toppart,FXString::null,cmdicon,ICON_BEFORE_TEXT|JUSTIFY_CENTER_X|JUSTIFY_BOTTOM|LAYOUT_FILL_Y|LAYOUT_FILL_X);
  FXVerticalFrame* entry=new FXVerticalFrame(toppart,LAYOUT_FILL_X|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0);
  new FXLabel(entry,label,nullptr,JUSTIFY_LEFT|ICON_BEFORE_TEXT|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X);
  FXHorizontalFrame* searchbox=new FXHorizontalFrame(entry,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0, 0,0);
  input=new FXTextField(searchbox,50,this,ID_ACCEPT,TEXTFIELD_ENTER_ONLY|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 3,3,3,3);
  input->setTipText(tr("Command to execute\nCommand line arguments are processed as follows:\n  %f  Replaced by current file;\n  %F  Replaced by currently open files;\n  %n  Replaced by current file's name;\n  %N  Replaced by currently open file names;\n  %u  Replaced by URL encoding of current file;\n  %U  Replaced by URL encoding of currently open files;\n  %d  Replaced by current working directory;\n  %%  Replaced by simply '%'."));
  input->setHelpText(tr("Execute shell command from editor."));
  FXVerticalFrame* historyarrows=new FXVerticalFrame(searchbox,LAYOUT_RIGHT|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);
  FXArrowButton* ar1=new FXArrowButton(historyarrows,this,ID_HISTORY_UP,FRAME_RAISED|FRAME_THICK|ARROW_UP|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 1,1,1,1);
  FXArrowButton* ar2=new FXArrowButton(historyarrows,this,ID_HISTORY_DN,FRAME_RAISED|FRAME_THICK|ARROW_DOWN|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 1,1,1,1);
  ar1->setArrowSize(3);
  ar2->setArrowSize(3);
  loadHistory();
  index=-1;
  }


// Append command to history, shifting older commands down.
void ShellDialog::appendHistory(const FXString& cmd){
  if(!cmd.empty()){
    if(history[0]!=cmd){
      for(FXuval i=ARRAYNUMBER(history)-1; i>0; --i){
        swap(history[i],history[i-1]);
        }
      }
    history[0]=cmd;
    index=-1;
    }
  }


// Load registy
void ShellDialog::loadHistory(){
  for(FXuval i=0; i<ARRAYNUMBER(history); ++i){
    history[i]=getApp()->reg().readStringEntry(hisSection,hisKey[i],FXString::null);
    }
  }


// Save registry
void ShellDialog::saveHistory(){
  for(FXuval i=0; i<ARRAYNUMBER(history); ++i){
    getApp()->reg().writeStringEntry(hisSection,hisKey[i],history[i].text());
    }
  }


// Accept the input, adding it to the history
long ShellDialog::onCmdAccept(FXObject*,FXSelector,void*){
  appendHistory(getText());
  saveHistory();
  getApp()->stopModal(this,true);
  hide();
  return 1;
  }


// Up and down arrow keys scroll history
long ShellDialog::onInputKey(FXObject*,FXSelector,void* ptr){
  switch(((FXEvent*)ptr)->code){
    case KEY_Up:
    case KEY_KP_Up:
      return onCmdHistUp(this,FXSEL(SEL_COMMAND,ID_HISTORY_UP),nullptr);
    case KEY_Down:
    case KEY_KP_Down:
      return onCmdHistDn(this,FXSEL(SEL_COMMAND,ID_HISTORY_DN),nullptr);
    }
  return 0;
  }


// Scroll back in history
long ShellDialog::onCmdHistUp(FXObject*,FXSelector,void*){
  if(index+1<(FXint)ARRAYNUMBER(history) && !history[index+1].empty()){
    index++;
    setText(history[index]);
    }
  else{
    getApp()->beep();
    }
  return 1;
  }


// Scroll back in history
long ShellDialog::onCmdHistDn(FXObject*,FXSelector,void*){
  if(0<index){
    index--;
    setText(history[index]);
    }
  else{
    index=-1;
    setText(FXString::null);
    }
  return 1;
  }


// Wheeled in history
long ShellDialog::onHistWheel(FXObject*,FXSelector,void* ptr){
  if(((FXEvent*)ptr)->code>0){
    return onCmdHistUp(this,FXSEL(SEL_COMMAND,ID_HISTORY_UP),nullptr);
    }
  if(((FXEvent*)ptr)->code<0){
    return onCmdHistDn(this,FXSEL(SEL_COMMAND,ID_HISTORY_DN),nullptr);
    }
  return 1;
  }


// Get input string
FXString ShellDialog::getText() const {
  return input->getText();
  }


// Set input string
void ShellDialog::setText(const FXString& text){
  input->setText(text);
  }


// We have to do this so as to force the initial text to be seleced
FXuint ShellDialog::execute(FXuint placement){
  create();
  show(placement);
  input->setFocus();
  input->selectAll();
  return getApp()->runModalFor(this);
  }


// Delete stuff
ShellDialog::~ShellDialog(){
  delete cmdicon;
  }
}
