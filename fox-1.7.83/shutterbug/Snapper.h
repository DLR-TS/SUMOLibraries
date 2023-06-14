/********************************************************************************
*                                                                               *
*                           S n a p p e r   W i d g e t                         *
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
#ifndef SNAPPER_H
#define SNAPPER_H


// Red snapper
class Snapper : public FXShell {
  FXDECLARE(Snapper)
protected:
  Snapper();
  virtual FXbool doesOverrideRedirect() const;
private:
  Snapper(const Snapper&);
  Snapper& operator=(const Snapper&);
public:
  Snapper(FXApp* a,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);
  virtual FXint getDefaultWidth();
  virtual FXint getDefaultHeight();
  virtual FXbool doesSaveUnder() const;
  };


#endif
