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
#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H


// Property dialog
class PropertyDialog : public FXDialogBox {
  FXDECLARE(PropertyDialog)
protected:
  FXLabel       *filename;
  FXLabel       *filetype;
  FXLabel       *filesize;
  FXLabel       *directory;
  FXLabel       *createtime;
  FXLabel       *modifytime;
  FXLabel       *accesstime;
  FXCheckButton *filemode[12];
  FXTextField   *fileowner;
  FXTextField   *filegroup;
private:
  PropertyDialog(){}
  PropertyDialog(const PropertyDialog&);
public:
  PropertyDialog(FXWindow *own);
  virtual ~PropertyDialog();
  };

#endif
