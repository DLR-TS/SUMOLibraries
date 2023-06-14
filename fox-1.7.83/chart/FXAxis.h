/********************************************************************************
*                                                                               *
*                          A x i s - D e f i n i t i o n                        *
*                                                                               *
*********************************************************************************
* Copyright (C) 2007,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXAXIS_H
#define FXAXIS_H


namespace FXCHART {


class FXChart;


/// Axis styles
enum {
  AXIS_OFF            = 0,              /// Nothing drawn on axis
  AXIS_LOG            = 0x0001,         /// Logarithmic scale
  AXIS_ROUND          = 0x0002,         /// Round range to nearest nice number
  AXIS_LINE           = 0x0004,         /// Axis line
  AXIS_LABEL          = 0x0008,         /// Axis label drawn
  AXIS_NUMBERS        = 0x0010,         /// Draw numbers on major ticks
  AXIS_NUMBERS_INSIDE = 0x0020,         /// Draw numbers inside
  AXIS_TICKS_MAJOR    = 0x0040,         /// Units display (. . .  N/m^2)
  AXIS_TICKS_MINOR    = 0x0080,         /// Units display (. . .  N/m^2)
  AXIS_TICKS_AUTO     = 0x0100,         /// Automatically compute ticks
  AXIS_TICKS_INSIDE   = 0x0200,         /// Tickmarks inside box
  AXIS_TICKS_OUTSIDE  = 0x0400,         /// Tickmarks outside box
  AXIS_TICKS_CROSS    = 0x0600,         /// Tickmarks inside and outside box
  AXIS_GRID_MAJOR     = 0x0800,         /// Grid lines drawn on major ticks
  AXIS_GRID_MINOR     = 0x1000,         /// Grid lines drawn on major ticks
  AXIS_GRID_FRONT     = 0x2000          /// Grid in front of data
  };


// Describe a plot axis
class FXCHARTAPI FXAxis {
private:
  FXChart    *chart;            // Backlink to chart
protected:
  FXString    label;            // Axis label
  FXFont     *labelfont;        // Label font
  FXFont     *numberfont;       // Number font
  Range       datarange;        // Full range of plotted data
  Range       axisrange;        // Subrange of data plotted
  FXdouble    majorspace;       // Major tick space
  FXdouble    minorspace;       // Minor tick space
  FXColor     labelcolor;       // Label color
  FXColor     numbercolor;      // Number color
  FXColor     axiscolor;        // Axis line color
  FXColor     majortickcolor;   // Major tickmark color
  FXColor     minortickcolor;   // Major tickmark color
  FXColor     majorgridcolor;   // Major gridline color
  FXColor     minorgridcolor;   // Major gridline color
  FXuchar     majorticksize;    // Major tick size
  FXuchar     minorticksize;    // Minor tick size
  FXuchar     numberoffset;     // Offset between ticks and numbers
  FXuchar     labeloffset;      // Offset between plot and axis label
  FXuchar     precision;        // Number precision
  FXuchar     labelstyle;       // Label style flags
  FXuchar     numberstyle;      // Number style flags
  FXuint      style;            // Axis style flags
private:
  FXAxis(const FXAxis&);
  FXAxis &operator=(const FXAxis&);
public:

  /// Create an axis
  FXAxis(FXChart* ch,const FXString& nm,FXuint lblsty=TEXT_ATTACH_TOP,FXuint numsty=TEXT_ATTACH_TOP,FXuint sty=AXIS_LINE|AXIS_TICKS_MAJOR|AXIS_NUMBERS|AXIS_LABEL);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Draw axis
  virtual void drawAxis(FXDC& dc,FXint fx,FXint fy,FXint tx,FXint ty,FXint ex,FXint ey) const;

  /// Set axis style
  void setStyle(FXuint sty);

  /// Get axis style
  FXuint getStyle() const { return style; }

  /// Change axis label
  void setLabel(const FXString& text);

  /// Get axis label
  const FXString& getLabel() const { return label; }

  /// Change label text style
  void setLabelStyle(FXuint sty);

  /// Get label text style
  FXuint getLabelStyle() const { return labelstyle; }

  /// Change label text color
  void setLabelColor(FXColor clr);

  /// Get label text color
  FXColor getLabelColor() const { return labelcolor; }

  /// Change label text style
  void setLabelFont(FXFont* font);

  /// Get label text style
  FXFont* getLabelFont() const { return labelfont; }

  /// Change number text style
  void setNumberStyle(FXuint sty);

  /// Get number text style
  FXuint getNumberStyle() const { return numberstyle; }

  /// Change number color
  void setNumberColor(FXColor clr);

  /// Get number color
  FXColor getNumberColor() const { return numbercolor; }

  /// Change label text style
  void setNumberFont(FXFont* font);

  /// Get label text style
  FXFont* getNumberFont() const { return numberfont; }

  /// Set data range
  void setDataRange(FXdouble min,FXdouble max);

  /// Get data range lower limit
  FXdouble getDataRangeMin() const { return datarange.minimum; }

  /// Get data range upper limit
  FXdouble getDataRangeMax() const { return datarange.maximum; }

  /// Set axis range
  void setAxisRange(FXdouble min,FXdouble max);

  /// Get axis range lower limit
  FXdouble getAxisRangeMin() const { return axisrange.minimum; }

  /// Get axis range upper limit
  FXdouble getAxisRangeMax() const { return axisrange.maximum; }

  /// Change major tick size
  void setMajorTickSize(FXint sz);

  /// Return major tick size
  FXint getMajorTickSize() const { return majorticksize; }

  /// Change minor tick size
  void setMinorTickSize(FXint sz);

  /// Return minor tick size
  FXint getMinorTickSize() const { return minorticksize; }

  /// Change major tick space
  void setMajorTickSpace(FXdouble space);

  /// Get major tick space
  FXdouble getMajorTickSpace() const { return majorspace; }

  /// Change minor tick space
  void setMinorTickSpace(FXdouble space);

  /// Get minor tick space
  FXdouble getMinorTickSpace() const { return minorspace; }

  /// Change number offset
  void setNumberOffset(FXint off);

  /// Get number offset
  FXint getNumberOffset() const { return numberoffset; }

  /// Change label offset
  void setLabelOffset(FXint off);

  /// Get label offset
  FXint getLabelOffset() const { return labeloffset; }

  /// Reset axis to data range
  void resetAxisRange();

  virtual ~FXAxis();
  };

}

#endif
