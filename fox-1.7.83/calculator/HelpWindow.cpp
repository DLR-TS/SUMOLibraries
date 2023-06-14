/********************************************************************************
*                                                                               *
*                            H e l p   W i n d o w                              *
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
#include "HelpWindow.h"


/*******************************************************************************/


FXIMPLEMENT(HelpWindow,FXDialogBox,nullptr,0)


// Construct help dialog box
HelpWindow::HelpWindow(FXWindow *own,const FXString& ttl):FXDialogBox(own,ttl,DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE,0,0,0,0, 6,6,6,6, 4,4){

  // Bottom part
  FXHorizontalFrame *closebox=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH);
  FXButton *button=new FXButton(closebox,tr("&Close"),nullptr,this,FXDialogBox::ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20,5,5);

  // Text part
  FXHorizontalFrame *editbox=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0, 0,0,0,0);
  helptext=new FXText(editbox,nullptr,0,TEXT_READONLY|TEXT_WORDWRAP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  helptext->setVisibleRows(50);
  helptext->setVisibleColumns(90);
  button->setFocus();
  }


// Set help text
void HelpWindow::setHelp(const FXString& help){
  helptext->setText(help);
  }


// Obtain help text
FXString HelpWindow::getHelp() const {
  return helptext->getText();
  }


// Clean up
HelpWindow::~HelpWindow(){
  helptext=(FXText*)-1;
  }
