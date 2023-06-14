/********************************************************************************
*                                                                               *
*                        P r e f e r e n c e s   D i a l o g                    *
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
#ifndef PREFERENCES_H
#define PREFERENCES_H



class TextWindow;
class Adie;
class Syntax;


class Preferences : public FXDialogBox {
  FXDECLARE(Preferences)
protected:
  FXText          *filepattext;
  FXMatrix        *stylemat;
  FXTextField     *searchtext;
  FXTextField     *syntaxtext;
  FXIcon          *pal;
  FXIcon          *ind;
  FXIcon          *pat;
  FXIcon          *sty;
  FXIcon          *mis;
private:
  Preferences(){}
  Preferences(const Preferences&);
  Preferences& operator=(const Preferences&);
public:

  // Create preferences dialog
  Preferences(TextWindow *owner);

  // Owner is text window
  Adie* getApp() const { return (Adie*)FXDialogBox::getApp(); }

  // Set list of filename patterns
  void setPatternList(const FXString& patterns);

  // Get list of filename patterns
  FXString getPatternList() const;

  // Set language syntax
  void setSyntax(Syntax* syn);

    // Clean up
  virtual ~Preferences();
  };

#endif
