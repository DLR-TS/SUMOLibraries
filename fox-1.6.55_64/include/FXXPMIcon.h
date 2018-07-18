/********************************************************************************
*                                                                               *
*                        X P M   I c o n   O b j e c t                          *
*                                                                               *
*********************************************************************************
* Copyright (C) 2000,2006 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or                 *
* modify it under the terms of the GNU Lesser General Public                    *
* License as published by the Free Software Foundation; either                  *
* version 2.1 of the License, or (at your option) any later version.            *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
* Lesser General Public License for more details.                               *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public              *
* License along with this library; if not, write to the Free Software           *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.    *
*********************************************************************************
* $Id: FXXPMIcon.h,v 1.22 2006/01/22 17:58:12 fox Exp $                         *
********************************************************************************/
#ifndef FXXPMICON_H
#define FXXPMICON_H

#ifndef FXICON_H
#include "FXIcon.h"
#endif

namespace FX {


/// X Pixmap icon
class FXAPI FXXPMIcon : public FXIcon {
  FXDECLARE(FXXPMIcon)
protected:
  FXXPMIcon(){}
private:
  FXXPMIcon(const FXXPMIcon&);
  FXXPMIcon &operator=(const FXXPMIcon&);
public:
  static const FXchar fileExt[];
  static const FXchar mimeType[];
public:

  /// Construct icon from compiled-in X Pixmap format
  FXXPMIcon(FXApp* a,const FXchar **pix=NULL,FXColor clr=FXRGB(192,192,192),FXuint opts=0,FXint w=1,FXint h=1);

  /// Save pixels into stream in X Pixmap format
  virtual bool savePixels(FXStream& store) const;

  /// Load pixels from stream in X Pixmap format
  virtual bool loadPixels(FXStream& store);

  /// Destroy icon
  virtual ~FXXPMIcon();
  };


#ifndef FXLOADXPM
#define FXLOADXPM

/**
* Check if stream contains a XPM, return TRUE if so.
*/
extern FXAPI bool fxcheckXPM(FXStream& store);


/**
* Load an XPM (X Pixmap) from array of strings.
* Upon successful return, the pixel array and size are returned.
* If an error occurred, the pixel array is set to NULL.
*/
extern FXAPI bool fxloadXPM(const FXchar **pix,FXColor*& data,FXint& width,FXint& height);


/**
* Load an XPM (X Pixmap) file from a stream.
* Upon successful return, the pixel array and size are returned.
* If an error occurred, the pixel array is set to NULL.
*/
extern FXAPI bool fxloadXPM(FXStream& store,FXColor*& data,FXint& width,FXint& height);


/**
* Save an XPM (X Pixmap) file to a stream.
*/
extern FXAPI bool fxsaveXPM(FXStream& store,const FXColor *data,FXint width,FXint height,bool fast=true);

#endif

}

#endif
