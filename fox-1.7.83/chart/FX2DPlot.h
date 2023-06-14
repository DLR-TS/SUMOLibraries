/********************************************************************************
*                                                                               *
*              T w o - D i m e n s i o n a l   P l o t   W i d g e t            *
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
#ifndef FX2DPLOT_H
#define FX2DPLOT_H

#ifndef FX2DCHART_H
#include "FX2DChart.h"
#endif

namespace FXCHART {


class FXCurve;


/// List of curves
typedef FXObjectListOf<FXCurve> FXCurveList;


/// Two-dimensional X-Y Plot widget
class FXCHARTAPI FX2DPlot : public FX2DChart {
  FXDECLARE(FX2DPlot)
protected:
  FXCurveList curves;   // List of curves
protected:
  FX2DPlot();
private:
  FX2DPlot(const FX2DPlot&);
  FX2DPlot &operator=(const FX2DPlot&);
public:

  /// Construct two-dimensional plot widget
  FX2DPlot(FXComposite* p,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=FRAME_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=4,FXint pr=4,FXint pt=4,FXint pb=4);

  /// Return number of curves
  FXint numCurves() const { return curves.no(); }

  /// Replace curve at index
  virtual void setCurve(FXint index,FXCurve* c);

  /// Curve at position
  FXCurve* getCurve(FXint index) const { return curves[index]; }

  /// Append curve
  virtual void appendCurve(FXCurve* c);

  /// Insert curve at given position
  virtual void insertCurve(FXint index,FXCurve* c);

  /// Remove curve at given position
  virtual void eraseCurve(FXint index);

  /// Remove all curves
  virtual void clearCurves();

  /// Save plot to a stream
  virtual void save(FXStream& store) const;

  /// Load plot from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FX2DPlot();
  };

}

#endif
