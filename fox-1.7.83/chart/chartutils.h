/********************************************************************************
*                                                                               *
*                          U t i l i t y - F u n c t i o n s                    *
*                                                                               *
*********************************************************************************
* Copyright (C) 2003,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef CHARTUTILS_H
#define CHARTUTILS_H


namespace FXCHART {

// Helper functions
extern FXCHARTAPI FXint textWidth(FXFont *font,const FXString& string);
extern FXCHARTAPI FXint textHeight(FXFont *font,const FXString& string);
extern FXCHARTAPI void drawText(FXDC& dc,const FXString& string,FXint x,FXint y,FXuint style);
extern FXCHARTAPI void drawRectangle(FXDC& dc,const FillStyle& fs,FXint x,FXint y,FXint w,FXint h);
extern FXCHARTAPI void drawMarker(FXDC& dc,const Marker& ms,FXint x,FXint y);
extern FXCHARTAPI void getTickSpacing(FXdouble& major,FXdouble& minor,FXdouble min,FXdouble max);

}

#endif
