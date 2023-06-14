/********************************************************************************
*                                                                               *
*             T w o - D i m e n s i o n a l   C h a r t   W i d g e t           *
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
#ifndef FX2DCHART_H
#define FX2DCHART_H

#ifndef FXCHART_H
#include "FXChart.h"
#endif

namespace FXCHART {


/// Base class for two-dimensional chart widgets
class FXCHARTAPI FX2DChart : public FXChart {
  FXDECLARE(FX2DChart)
protected:
  FXAxis   *axes[4];            // Axis definitions
  FillStyle chartstyle;         // Chart fill style
protected:
  FX2DChart();
  virtual void drawSelf(FXDC& dc) const;
private:
  FX2DChart(const FX2DChart&);
  FX2DChart &operator=(const FX2DChart&);
public:
  long onCmdXXX(FXObject*,FXSelector,void*);
public:
  enum{
    ID_XXX=FXChart::ID_LAST,
    ID_LAST
    };
public:
  enum {
    XLO,        /// Lower X axis
    YLO,        /// Lower Y axis
    XHI,        /// Upper X axis
    YHI         /// Upper Y axis
    };
public:


  /// Construct two-dimensional chart widget
  FX2DChart(FXComposite* p,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=CAPTION_ABOVE|CAPTION_SHOWN,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=4,FXint pr=4,FXint pt=4,FXint pb=4);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Return axis
  FXAxis* getAxis(FXint ax) const { return axes[ax]; }

  /// Set chart fill style
  void setChartStyle(const FillStyle& fs);

  /// Get chart fill style
  const FillStyle& getChartStyle() const { return chartstyle; }

  /// Save chart to a stream
  virtual void save(FXStream& store) const;

  /// Load chart from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FX2DChart();
  };

}

#endif
