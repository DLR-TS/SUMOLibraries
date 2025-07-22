/********************************************************************************
*                                                                               *
*                     T A R G A   I c o n   O b j e c t                         *
*                                                                               *
*********************************************************************************
* Copyright (C) 2001,2006 by Janusz Ganczarski.   All Rights Reserved.          *
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
* $Id: FXTGAIcon.h,v 1.18 2006/01/22 17:58:10 fox Exp $                         *
********************************************************************************/
#ifndef FXTGAICON_H
#define FXTGAICON_H

#ifndef FXICON_H
#include "FXIcon.h"
#endif

namespace FX {


/// TARGA image format icon
class FXAPI FXTGAIcon : public FXIcon {
  FXDECLARE(FXTGAIcon)
protected:
  FXTGAIcon(){}
private:
  FXTGAIcon(const FXTGAIcon&);
  FXTGAIcon &operator=(const FXTGAIcon&);
public:
  static const FXchar fileExt[];
  static const FXchar mimeType[];
public:

  /// Construct icon from memory stream formatted in TARGA format
  FXTGAIcon(FXApp* a,const void *pix=NULL,FXColor clr=FXRGB(192,192,192),FXuint opts=0,FXint w=1,FXint h=1);

  /// Save pixels into stream in TARGA format
  virtual bool savePixels(FXStream& store) const;

  /// Load pixels from stream in TARGA format
  virtual bool loadPixels(FXStream& store);

  /// Destroy icon
  virtual ~FXTGAIcon();
  };


/**
* Check if stream contains a TARGA, return TRUE if so.
*/
extern FXAPI bool fxcheckTGA(FXStream& store);


/**
* Load an TARGA file from a stream.
* Upon successful return, the pixel array and size are returned.
* If an error occurred, the pixel array is set to NULL.
*/
extern FXAPI bool fxloadTGA(FXStream& store,FXColor*& data,FXint& width,FXint& height);


/**
* Save an TARGA file to a stream.
*/
extern FXAPI bool fxsaveTGA(FXStream& store,const FXColor *data,FXint width,FXint height);

}

#endif
