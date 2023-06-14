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
FXIMPLEMENT(FX2DPlot,FX2DChart,nullptr,0)


// Init
FX2DPlot::FX2DPlot(){
  }


// Construct two-dimensional plot widget
FX2DPlot::FX2DPlot(FXComposite* p,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb):FX2DChart(p,tgt,sel,opts,x,y,w,h,pl,pr,pt,pb){
  }


// Replace curve at index
void FX2DPlot::setCurve(FXint index,FXCurve* c){
  if(curves[index]!=c){
    delete curves[index];
    curves[index]=c;
    recalc();
    }
  }


// Append curve
void FX2DPlot::appendCurve(FXCurve* c){
  curves.append(c);
  recalc();
  }


// Insert curve at given position
void FX2DPlot::insertCurve(FXint index,FXCurve* c){
  curves.insert(index,c);
  recalc();
  }


// Remove curve at given position
void FX2DPlot::eraseCurve(FXint index){
  delete curves[index];
  curves.erase(index);
  recalc();
  }


// Remove all curves
void FX2DPlot::clearCurves(){
  for(FXint i=0; i<curves.no(); i++){
    delete curves[i];
    }
  curves.clear();
  recalc();
  }


// Save data
void FX2DPlot::save(FXStream& store) const {
  FX2DChart::save(store);
  ////
  }


// Load data
void FX2DPlot::load(FXStream& store){
  FX2DChart::load(store);
  ////
  }


// Destroy
FX2DPlot::~FX2DPlot(){
  clearCurves();
  }

}
