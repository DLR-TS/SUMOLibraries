/********************************************************************************
*                                                                               *
*                        C h a r t    D e f i n i t i o n s                     *
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
#ifndef CHARTDEFS_H
#define CHARTDEFS_H


/********************************  Definitions  ********************************/

// Define FXCHARTAPI for DLL builds
#ifdef FOXDLL
#ifdef CHARTDLL_EXPORTS
#define FXCHARTAPI FXEXPORT
#else
#define FXCHARTAPI FXIMPORT
#endif
#else
#define FXCHARTAPI
#endif


namespace FXCHART {


/// Line styles
enum {
  LINESTYLE_NONE,               /// No line
  LINESTYLE_SOLID,              /// Solid line
  LINESTYLE_DOTTED,             /// Dotted line
  LINESTYLE_SHORTDASHED,        /// Short dashed line
  LINESTYLE_LONGDASHED,         /// Long dashed line
  LINESTYLE_DOTDASHED           /// Dot-dashed line
  };


/// Line style definition
struct LineStyle {
  FXColor  color;               /// Line color
  FXuchar  weight;              /// Line weight
  FXuchar  style;               /// Line style flags
  };


/// Marker styles
enum {
  MARKER_NONE          = 0,     /// Draw nothing
  MARKER_SQUARE        = 1,     /// Draw (solid) square
  MARKER_CIRCLE        = 2,     /// Draw (solid) circle
  MARKER_DIAMOND       = 3,     /// Draw (solid) diamond
  MARKER_TRIANGLE_UP   = 4,     /// Draw (solid) upward triangle
  MARKER_TRIANGLE_DN   = 5,     /// Draw (solid) downward triangle
  MARKER_TRIANGLE_LT   = 6,     /// Draw (solid) leftward triangle
  MARKER_TRIANGLE_RT   = 7,     /// Draw (solid) rightward triangle
  MARKER_SOLID         = 8      /// Fill shape
  };


/// Marker definition
struct Marker {
  FXColor color;                /// Color of markers
  FXuchar size;                 /// How big to draw markers
  FXuchar style;                /// Marker style
  };


/// Fill styles
enum {
  FILLSTYLE_OFF,                /// Not filled
  FILLSTYLE_SOLID,              /// Solid color
  FILLSTYLE_HATCH,              /// Hatch pattern
  FILLSTYLE_TEXTURE,            /// Repeating texture
  FILLSTYLE_IMAGE,              /// Fill with an image
  FILLSTYLE_HORIZONTAL,         /// Horizontal gradient
  FILLSTYLE_VERTICAL            /// Vertical gradient
  };


/// Fill style definition
struct FillStyle {
  FXImage  *image;              /// Image used for texture or image fill
  FXColor   forecolor;          /// Fill color
  FXColor   backcolor;          /// Back color when hatching (may be clear)
  FXColor   lower;              /// Lower gradient color
  FXColor   upper;              /// Upper gradient color
  FXuchar   hatch;              /// Hatch pattern if hatch style
  FXuchar   style;              /// Fill style
  };


/// Range description
struct Range {
  FXdouble minimum;             /// Minumum value
  FXdouble maximum;             /// Maximum value
  };


/// Text justification and attachment styles
enum {
  TEXT_JUSTIFY_CENTER  = 0,     /// Justify in center of text-box
  TEXT_JUSTIFY_LEFT    = 1,     /// Justify to left of text-box
  TEXT_JUSTIFY_RIGHT   = 2,     /// Justify to right of text-box
  TEXT_ATTACH_CENTER   = 0,     /// Attach to center of text-box
  TEXT_ATTACH_LEFT     = 4,     /// Attach to left of text-box
  TEXT_ATTACH_RIGHT    = 8,     /// Attach to right of text-box
  TEXT_ATTACH_TOP      = 16,    /// Attach to top of text-box
  TEXT_ATTACH_BOTTOM   = 32     /// Attach to bottom of text-box
  };

}

#endif
