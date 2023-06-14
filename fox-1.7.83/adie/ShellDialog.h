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
#ifndef SHELLDIALOG_H
#define SHELLDIALOG_H


namespace FX {

/**
* Shell dialog grabs a command to be executed.
* It has history scrollback for convenience.
*/
class FXAPI ShellDialog : public FXDialogBox {
  FXDECLARE(ShellDialog)
protected:
  FXTextField *input;      // Text field widget
  FXIcon      *cmdicon;    // Command icon
  FXString    history[20]; // Command history
  FXint       index;       // History index
protected:
  ShellDialog(){}
private:
  ShellDialog(const ShellDialog&);
  ShellDialog &operator=(const ShellDialog&);
  void appendHistory(const FXString& cmd);
  void loadHistory();
  void saveHistory();
public:
  long onInputKey(FXObject*,FXSelector,void*);
  long onCmdHistUp(FXObject*,FXSelector,void*);
  long onCmdHistDn(FXObject*,FXSelector,void*);
  long onHistWheel(FXObject*,FXSelector,void*);
  long onCmdAccept(FXObject*,FXSelector,void*);
public:
  enum{
    ID_HISTORY_UP=FXDialogBox::ID_LAST,
    ID_HISTORY_DN,
    ID_LAST
    };
public:

  /// Construct input dialog box with given caption, icon, and prompt text
  ShellDialog(FXWindow* owner,const FXString& name,const FXString& label);

  /// Get input string
  FXString getText() const;

  /// Set input string
  void setText(const FXString& text);

  /// Run modal invocation of the dialog
  virtual FXuint execute(FXuint placement=PLACEMENT_CURSOR);

  /// Delete shell dialog
  virtual ~ShellDialog();
  };

}

#endif
