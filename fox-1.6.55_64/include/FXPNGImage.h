/********************************************************************************
*                                                                               *
*                          P N G   I m a g e   O b j e c t                      *
*                                                                               *
*********************************************************************************
* Copyright (C) 1999,2006 by Jeroen van der Zijp.   All Rights Reserved.        *
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
* $Id: FXPNGImage.h,v 1.22 2006/01/22 17:58:06 fox Exp $                        *
********************************************************************************/
#ifndef FXPNGIMAGE_H
#define FXPNGIMAGE_H

#ifndef FXIMAGE_H
#include "FXImage.h"
#endif

namespace FX {


/// Portable Network Graphics (PNG) Image class
class FXAPI FXPNGImage : public FXImage {
  FXDECLARE(FXPNGImage)
protected:
  FXPNGImage(){}
private:
  FXPNGImage(const FXPNGImage&);
  FXPNGImage &operator=(const FXPNGImage&);
public:
  static const FXchar fileExt[];
  static const FXchar mimeType[];
public:

  /// Construct an image from memory stream formatted in PNG format
  FXPNGImage(FXApp *a,const void *pix=NULL,FXuint opts=0,FXint w=1,FXint h=1);

  /// True if format is supported
  static const bool supported;

  /// Save pixels into stream in PNG format
  virtual bool loadPixels(FXStream& store);

  /// Load pixels from stream in PNG format
  virtual bool savePixels(FXStream& store) const;

  /// Destroy
  virtual ~FXPNGImage();
  };


/**
* Check if stream contains a PNG, return TRUE if so.
*/
extern FXAPI bool fxcheckPNG(FXStream& store);


/**
* Load an PNG (Portable Network Graphics) file from a stream.
* Upon successful return, the pixel array and size are returned.
* If an error occurred, the pixel array is set to NULL.
*/
extern FXAPI bool fxloadPNG(FXStream& store,FXColor*& data,FXint& width,FXint& height);


/**
* Save an PNG (Portable Network Graphics) file to a stream.
*/
extern FXAPI bool fxsavePNG(FXStream& store,const FXColor* data,FXint width,FXint height);

}

#endif
