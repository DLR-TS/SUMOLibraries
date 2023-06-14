/********************************************************************************
*                                                                               *
*                      P l o t - C u r v e   D e s c r i p t i o n              *
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
#include "fx.h"
#include "chartdefs.h"
#include "chartutils.h"
#include "FXChart.h"
#include "FXCurve.h"
#include "FX2DChart.h"
#include "FX2DPlot.h"

/*
  Notes:
*/


using namespace FXCHART;

/*******************************************************************************/

namespace FXCHART {

// Object implementation
FXIMPLEMENT(FXCurve,FXObject,nullptr,0)


class FXData {
  };


// Deserialization
FXCurve::FXCurve():plot(nullptr),xdata(nullptr),ydata(nullptr),xaxis(0),yaxis(0){
  }


// Init
FXCurve::FXCurve(FX2DPlot* plt,const FXString& nm):plot(plt),xdata(nullptr),ydata(nullptr),label(nm),xaxis(0),yaxis(0){
  }


// Sets the curve name
void FXCurve::setLabel(const FXString& nm){
  if(label!=nm){
    label=nm;
    plot->update();
    }
  }


// Change x axis description
void FXCurve::setXAxis(FXuchar ax){
  if(xaxis!=ax){
    xaxis=ax;
    plot->update();
    }
  }


// Change y axis description
void FXCurve::setYAxis(FXuchar ax){
  if(yaxis!=ax){
    yaxis=ax;
    plot->update();
    }
  }


// Change x data samples
void FXCurve::setXData(FXData* dd){
  if(xdata!=dd){
    xdata=dd;
    plot->update();
    }
  }

// Change y data samples
void FXCurve::setYData(FXData* dd){
  if(ydata!=dd){
    ydata=dd;
    plot->update();
    }
  }


// Save data
void FXCurve::save(FXStream& store) const {
  FXObject::save(store);
  store << label;
  store << xaxis;
  store << yaxis;
  }


// Load data
void FXCurve::load(FXStream& store){
  FXObject::load(store);
  store >> label;
  store >> xaxis;
  store >> yaxis;
  }


// Destroy
FXCurve::~FXCurve(){
  delete xdata;
  delete ydata;
  plot=(FX2DPlot*)-1L;
  xdata=(FXData*)-1L;
  ydata=(FXData*)-1L;
  }

}
