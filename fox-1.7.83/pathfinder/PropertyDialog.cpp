/********************************************************************************
*                                                                               *
*                  F i l e   P r o p e r t i e s   D i a l o g                  *
*                                                                               *
*********************************************************************************
* Copyright (C) 2000,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#include "PropertyDialog.h"
#include "PathFinder.h"
#include "icons.h"
#include <stdio.h>
#include <stdlib.h>

/*
  Notes:
  - Refer to RFC 2045, 2046, 2047, 2048, and 2077.
    The Internet media type registry is at:
    ftp://ftp.iana.org/in-notes/iana/assignments/media-types/
*/

/**********************************  Preferences  ******************************/



FXIMPLEMENT(PropertyDialog,FXDialogBox,nullptr,0)


// Create properties dialog
PropertyDialog::PropertyDialog(FXWindow *own):FXDialogBox(own,FXString::null,DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE,0,0,0,0, 4,4,4,4, 4,4){

  setTitle(tr("Properties"));

  // Close button
  FXHorizontalFrame *closebox=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH);
  new FXButton(closebox,tr("&Close"),nullptr,this,FXDialogBox::ID_CLOSE,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);

  // Separator
  new FXHorizontalSeparator(this,SEPARATOR_GROOVE|LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);

  FXTabBook* tabbook=new FXTabBook(this,nullptr,0,LAYOUT_SIDE_TOP|TABBOOK_TOPTABS|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  // ===== General Info =====
  new FXTabItem(tabbook,tr("&General\tGeneral Information\tGeneral information about the item."),nullptr,TAB_TOP|ICON_BEFORE_TEXT);
  FXVerticalFrame *generalFrame=new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK);

  FXGroupBox *namegroup=new FXGroupBox(generalFrame,tr("Description"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X);
  filename=new FXLabel(namegroup,FXString::null,nullptr,LAYOUT_CENTER_Y|LAYOUT_SIDE_LEFT|ICON_BEFORE_TEXT|LAYOUT_FIX_HEIGHT,0,0,0,40);
  filename->setTarget(own);
  filename->setSelector(PathFinderMain::ID_PROP_DESC);

  FXGroupBox *attrgroup=new FXGroupBox(generalFrame,tr("Attributes"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X);
  FXMatrix *attrmatrix=new FXMatrix(attrgroup,2,MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(attrmatrix,tr("Type:"),nullptr,LAYOUT_RIGHT);
  filetype=new FXLabel(attrmatrix,FXString::null,nullptr,LAYOUT_LEFT|LAYOUT_FILL_COLUMN);
  filetype->setTarget(own);
  filetype->setSelector(PathFinderMain::ID_PROP_TYPE);
  new FXLabel(attrmatrix,tr("Location:"),nullptr,LAYOUT_RIGHT);
  directory=new FXLabel(attrmatrix,FXString::null,nullptr,LAYOUT_LEFT|LAYOUT_FILL_COLUMN);
  directory->setTarget(own);
  directory->setSelector(PathFinderMain::ID_PROP_LOCATION);
  new FXLabel(attrmatrix,tr("Size:"),nullptr,LAYOUT_RIGHT);
  filesize=new FXLabel(attrmatrix,FXString::null,nullptr,LAYOUT_LEFT|LAYOUT_FILL_COLUMN);
  filesize->setTarget(own);
  filesize->setSelector(PathFinderMain::ID_PROP_SIZE);

  FXGroupBox *timegroup=new FXGroupBox(generalFrame,tr("File Time"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X);
  FXMatrix *timematrix=new FXMatrix(timegroup,2,MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(timematrix,tr("Created:"),nullptr,LAYOUT_RIGHT);
  createtime=new FXLabel(timematrix,FXString::null,nullptr,LAYOUT_LEFT|LAYOUT_FILL_COLUMN);
  createtime->setTarget(own);
  createtime->setSelector(PathFinderMain::ID_PROP_CREATED);
  new FXLabel(timematrix,tr("Modified:"),nullptr,LAYOUT_RIGHT);
  modifytime=new FXLabel(timematrix,FXString::null,nullptr,LAYOUT_LEFT|LAYOUT_FILL_COLUMN);
  modifytime->setTarget(own);
  modifytime->setSelector(PathFinderMain::ID_PROP_MODIFIED);
  new FXLabel(timematrix,tr("Accessed:"),nullptr,LAYOUT_RIGHT);
  accesstime=new FXLabel(timematrix,FXString::null,nullptr,LAYOUT_LEFT|LAYOUT_FILL_COLUMN);
  accesstime->setTarget(own);
  accesstime->setSelector(PathFinderMain::ID_PROP_ACCESSED);

  // ===== Permissions =====
  new FXTabItem(tabbook,tr("&Permissions\tAccess Permissions\tAccess permissions for this item."),nullptr,TAB_TOP|ICON_BEFORE_TEXT);
  FXVerticalFrame *permFrame=new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK);
  FXGroupBox *accessgroup=new FXGroupBox(permFrame,tr("Access Permissions"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X);
  FXMatrix *accessmatrix=new FXMatrix(accessgroup,6,MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXFrame(accessmatrix,LAYOUT_FILL_COLUMN);
  new FXLabel(accessmatrix,tr("Read"),nullptr,LAYOUT_CENTER_X);
  new FXLabel(accessmatrix,tr("Write"),nullptr,LAYOUT_CENTER_X);
  new FXLabel(accessmatrix,tr("Exec"),nullptr,LAYOUT_CENTER_X);
  new FXFrame(accessmatrix,LAYOUT_FILL_COLUMN);
  new FXLabel(accessmatrix,tr("Special"),nullptr,LAYOUT_CENTER_X);

  new FXLabel(accessmatrix,tr("&User"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN);
  filemode[8]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_RUSR,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  filemode[7]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_WUSR,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  filemode[6]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_XUSR,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  new FXLabel(accessmatrix,tr("Set UID"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN);
  filemode[9]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_SUID,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);

  new FXLabel(accessmatrix,tr("&Group"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN);
  filemode[5]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_RGRP,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  filemode[4]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_WGRP,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  filemode[3]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_XGRP,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  new FXLabel(accessmatrix,tr("Set GID"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN);
  filemode[10]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_SGID,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);

  new FXLabel(accessmatrix,tr("&Other"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN);
  filemode[2]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_ROTH,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  filemode[1]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_WOTH,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  filemode[0]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_XOTH,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);
  new FXLabel(accessmatrix,tr("Sticky"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN);
  filemode[11]=new FXCheckButton(accessmatrix,FXString::null,own,PathFinderMain::ID_PROP_SVTX,LAYOUT_CENTER_X|LAYOUT_CENTER_Y);

  FXGroupBox *ownergroup=new FXGroupBox(permFrame,tr("Ownership"),GROUPBOX_TITLE_LEFT|FRAME_GROOVE|LAYOUT_FILL_X);
  FXMatrix *ownermatrix=new FXMatrix(ownergroup,2,MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(ownermatrix,tr("O&wner"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN);
  fileowner=new FXTextField(ownermatrix,20,own,PathFinderMain::ID_PROP_OWNER,TEXTFIELD_READONLY|LAYOUT_FILL_X|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN|FRAME_SUNKEN|FRAME_THICK);
  new FXLabel(ownermatrix,tr("Grou&p"),nullptr,LAYOUT_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN);
  filegroup=new FXTextField(ownermatrix,20,own,PathFinderMain::ID_PROP_GROUP,TEXTFIELD_READONLY|LAYOUT_FILL_X|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN|FRAME_SUNKEN|FRAME_THICK);
  }


// Destroy
PropertyDialog::~PropertyDialog(){
  }
