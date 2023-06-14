/********************************************************************************
*                                                                               *
*                        C h a r t   B a s e   W i d g e t                      *
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
#ifndef FXCHART_H
#define FXCHART_H


namespace FXCHART {


class FXAxis;

/// Chart caption options
enum {
  CAPTION_NONE   = 0,           /// No caption
  CAPTION_ABOVE  = 0x00001000,  /// Caption above chart
  CAPTION_BELOW  = 0x00002000,  /// Caption below chart
  CAPTION_LEFT   = 0x00004000,  /// Caption left of chart
  CAPTION_RIGHT  = 0x00008000,  /// Caption right of chart
  CAPTION_SHOWN  = 0x00010000   /// Show caption
  };


/// Base class for the various chart widgets
class FXCHARTAPI FXChart : public FXComposite {
  FXDECLARE(FXChart)
protected:
  FXString  caption;            // Caption over plot
  FXFont   *captionfont;        // Caption font
  FXColor   captioncolor;       // Caption color
  FXint     captionoffset;      // Caption offset
  FXImage  *chart;              // Chart image
  FillStyle backstyle;          // Plot background fill style
  FXint     margintop;          // Margins top
  FXint     marginbottom;       // Margin bottom
  FXint     marginleft;         // Margin left
  FXint     marginright;        // Margin right
  FXString  tip;                // Tooltip value
  FXString  help;               // Help value
protected:
  FXChart();
  virtual void updateChart();
  virtual void drawSelf(FXDC& dc) const;
private:
  FXChart(const FXChart&);
  FXChart &operator=(const FXChart&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
  long onClipboardLost(FXObject*,FXSelector,void*);
  long onClipboardRequest(FXObject*,FXSelector,void*);
public:
  static FXDragType bmpType;
  static FXDragType gifType;
  static FXDragType jpgType;
  static FXDragType pngType;
  static FXDragType tifType;
  static FXDragType csvType;
public:

  /// Construct chart widget
  FXChart(FXComposite* p,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=CAPTION_ABOVE|CAPTION_SHOWN,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=4,FXint pr=4,FXint pt=4,FXint pb=4);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Perform layout
  virtual void layout();

  /// Change caption
  void setCaption(const FXString& cap);

  /// Return caption
  const FXString& getCaption() const { return caption; }

  /// Set fill style
  void setBackStyle(const FillStyle& bs);

  /// Get fill style
  const FillStyle& getBackStyle() const { return backstyle; }

  /// Change caption font
  void setCaptionFont(FXFont* font);

  /// Get caption font
  FXFont* getCaptionFont() const { return captionfont; }

  /// Change caption color
  void setCaptionColor(FXColor clr);

  /// Get caption color
  FXColor getCaptionColor() const { return captioncolor; }

  /// Change caption style
  void setCaptionStyle(FXuint sty);

  /// Get caption style
  FXuint getCaptionStyle() const;

  /// Change top margin
  void setMarginTop(FXint pt);

  /// Return top margin
  FXint getMarginTop() const { return margintop; }

  /// Change bottom margin
  void setMarginBottom(FXint pb);

  /// Return bottom margin
  FXint getMarginBottom() const { return marginbottom; }

  /// Change left margin
  void setMarginLeft(FXint pl);

  /// Return left margin
  FXint getMarginLeft() const { return marginleft; }

  /// Change right margin
  void setMarginRight(FXint pr);

  /// Return right margin
  FXint getMarginRight() const { return marginright; }

  /// Change caption offset
  void setCaptionOffset(FXint off);

  /// Get caption offset
  FXint getCaptionOffset() const { return captionoffset; }

  /// Set status line help text for this chart
  void setHelpText(const FXString& text);

  /// Get status line help text for this chart
  FXString getHelpText() const { return help; }

  /// Set tool tip message for this chart
  void setTipText(const FXString& text);

  /// Get tool tip message for this chart
  FXString getTipText() const { return tip; }

  /// Save chart to a stream
  virtual void save(FXStream& store) const;

  /// Load chart from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXChart();
  };

}

#endif
