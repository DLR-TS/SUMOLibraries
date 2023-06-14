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
#include "fx.h"
#include "chartdefs.h"
#include "chartutils.h"
#include "FXAxis.h"
#include "FXChart.h"
#include "FX2DChart.h"


/*
  Notes:
*/

using namespace FXCHART;

/*******************************************************************************/

namespace FXCHART {


// Default chart fill style
const FillStyle defaultChartStyle={nullptr,FXRGB(255,255,255),FXRGBA(0,0,0,0),FXRGB(240,240,240),FXRGB(255,255,255),STIPPLE_NONE,FILLSTYLE_SOLID};


// Map
FXDEFMAP(FX2DChart) FX2DChartMap[]={
  FXMAPFUNC(SEL_COMMAND,FX2DChart::ID_XXX,FX2DChart::onCmdXXX),
  };


// Object implementation
FXIMPLEMENT(FX2DChart,FXChart,FX2DChartMap,ARRAYNUMBER(FX2DChartMap))


// Deserialization
FX2DChart::FX2DChart(){
  chartstyle=defaultChartStyle;
  axes[XLO]=nullptr;
  axes[YLO]=nullptr;
  axes[XHI]=nullptr;
  axes[YHI]=nullptr;
  }


// Construct two-dimensional chart widget
FX2DChart::FX2DChart(FXComposite* p,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb):FXChart(p,tgt,sel,opts,x,y,w,h,pl,pr,pt,pb){
  chartstyle=defaultChartStyle;
  axes[XLO]=new FXAxis(this,"X",TEXT_ATTACH_TOP,   TEXT_ATTACH_TOP,   AXIS_LINE|AXIS_LABEL|AXIS_NUMBERS|AXIS_TICKS_MAJOR|AXIS_TICKS_MINOR|AXIS_TICKS_INSIDE|AXIS_TICKS_AUTO|AXIS_GRID_MAJOR);
  axes[YLO]=new FXAxis(this,"Y",TEXT_ATTACH_RIGHT, TEXT_ATTACH_RIGHT, AXIS_LINE|AXIS_LABEL|AXIS_NUMBERS|AXIS_TICKS_MAJOR|AXIS_TICKS_MINOR|AXIS_TICKS_INSIDE|AXIS_TICKS_AUTO|AXIS_GRID_MAJOR);
  axes[XHI]=new FXAxis(this,"X",TEXT_ATTACH_BOTTOM,TEXT_ATTACH_BOTTOM,AXIS_LINE);
  axes[YHI]=new FXAxis(this,"Y",TEXT_ATTACH_LEFT,  TEXT_ATTACH_LEFT,  AXIS_LINE);
  }


// Create window; register drag types
void FX2DChart::create(){
  FXChart::create();
  if(chartstyle.image) chartstyle.image->create();
  axes[XLO]->create();
  axes[YLO]->create();
  axes[XHI]->create();
  axes[YHI]->create();
  }


// Detach window; zero out drag types
void FX2DChart::detach(){
  FXChart::detach();
  if(chartstyle.image) chartstyle.image->detach();
  axes[XLO]->detach();
  axes[YLO]->detach();
  axes[XHI]->detach();
  axes[YHI]->detach();
  }


// Draw contents of chart
void FX2DChart::drawSelf(FXDC& dc) const {
  FXint plotleft=dc.getClipX()+marginleft;
  FXint plottop=dc.getClipY()+margintop;
  FXint plotright=dc.getClipX()+dc.getClipWidth()-marginright-1;
  FXint plotbottom=dc.getClipY()+dc.getClipHeight()-marginbottom-1;

  // Draw chart background
  drawRectangle(dc,backstyle,dc.getClipX(),dc.getClipY(),dc.getClipWidth(),dc.getClipHeight());

  // Space left after margins?
  if(plotleft<plotright && plottop<plotbottom){
    FXint caps=TEXT_ATTACH_CENTER|TEXT_JUSTIFY_CENTER;
    FXint capx=0;
    FXint capy=0;
    FXint capw=0;
    FXint caph=0;
    FXint ss;

    // Account for caption
    if(options&CAPTION_SHOWN){
      caps=TEXT_JUSTIFY_CENTER;
      capx=(plotleft+plotright)/2;
      capy=(plottop+plotbottom)/2;
      capw=textWidth(captionfont,caption);
      caph=textHeight(captionfont,caption);
      if(options&CAPTION_LEFT){           // Caption left of chart
        caps|=TEXT_ATTACH_LEFT;
        capx=plotleft;
        plotleft+=capw+captionoffset;
        }
      else if(options&CAPTION_RIGHT){     // Caption right of chart
        caps|=TEXT_ATTACH_RIGHT;
        capx=plotright;
        plotright-=capw+captionoffset;
        }
      if(options&CAPTION_ABOVE){          // Caption above chart
        caps|=TEXT_ATTACH_TOP;
        capy=plottop;
        plottop+=caph+captionoffset;
        }
      else if(options&CAPTION_BELOW){     // Caption below chart
        caps|=TEXT_ATTACH_BOTTOM;
        capy=plotbottom;
        plotbottom-=caph+captionoffset;
        }
      }

    // Space left after caption?
    if(plotleft<plotright && plottop<plotbottom){

      // Account for the lower x-axis attributes to figure the plot size
      if(axes[XLO]->getStyle()&(AXIS_LABEL|AXIS_NUMBERS|AXIS_TICKS_MAJOR)){
        if(axes[XLO]->getStyle()&AXIS_TICKS_OUTSIDE){
          ss=0;
          if(axes[XLO]->getStyle()&AXIS_TICKS_MINOR) ss=axes[XLO]->getMinorTickSize();
          if(axes[XLO]->getStyle()&AXIS_TICKS_MAJOR) ss=axes[XLO]->getMajorTickSize();
          plotbottom-=ss;
          }
        if(axes[XLO]->getStyle()&AXIS_NUMBERS){
          if(!(axes[XLO]->getStyle()&AXIS_NUMBERS_INSIDE)){
            if(axes[XLO]->getNumberStyle()&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
              plotbottom-=textHeight(axes[XLO]->getNumberFont(),"8.8")+axes[XLO]->getLabelOffset();
              }
            else if(axes[XLO]->getNumberStyle()&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
              plotbottom-=textWidth(axes[XLO]->getNumberFont(),"8.8")+axes[XLO]->getLabelOffset();
              }
            }
          }
        if(axes[XLO]->getStyle()&AXIS_LABEL){
          if(axes[XLO]->getLabelStyle()&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
            plotbottom-=textHeight(axes[XLO]->getLabelFont(),axes[XLO]->getLabel())+axes[XLO]->getLabelOffset();
            }
          else if(axes[XLO]->getLabelStyle()&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
            plotbottom-=textWidth(axes[XLO]->getLabelFont(),axes[XLO]->getLabel())+axes[XLO]->getLabelOffset();
            }
          }
        }

      // Account for the upper x-axis attributes to figure the plot size
      if(axes[XHI]->getStyle()&(AXIS_LABEL|AXIS_NUMBERS|AXIS_TICKS_MAJOR)){
        if(axes[XHI]->getStyle()&AXIS_TICKS_OUTSIDE){
          ss=0;
          if(axes[XHI]->getStyle()&AXIS_TICKS_MINOR) ss=axes[XHI]->getMinorTickSize();
          if(axes[XHI]->getStyle()&AXIS_TICKS_MAJOR) ss=axes[XHI]->getMajorTickSize();
          plottop+=ss;
          }
        if(axes[XHI]->getStyle()&AXIS_NUMBERS){
          if(!(axes[XHI]->getStyle()&AXIS_NUMBERS_INSIDE)){
            if(axes[XHI]->getNumberStyle()&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
              plottop+=textHeight(axes[XHI]->getNumberFont(),"8.8")+axes[XHI]->getLabelOffset();
              }
            else if(axes[XHI]->getNumberStyle()&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
              plottop+=textWidth(axes[XHI]->getNumberFont(),"8.8")+axes[XHI]->getLabelOffset();
              }
            }
          }
        if(axes[XHI]->getStyle()&AXIS_LABEL){
          if(axes[XHI]->getLabelStyle()&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
            plottop+=textHeight(axes[XHI]->getLabelFont(),axes[XHI]->getLabel())+axes[XHI]->getLabelOffset();
            }
          else if(axes[XHI]->getLabelStyle()&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
            plottop+=textWidth(axes[XHI]->getLabelFont(),axes[XHI]->getLabel())+axes[XHI]->getLabelOffset();
            }
          }
        }

      // Account for the lower y-axis attributes to figure the plot size
      if(axes[YLO]->getStyle()&(AXIS_LABEL|AXIS_NUMBERS|AXIS_TICKS_MAJOR)){
        if(axes[YLO]->getStyle()&AXIS_TICKS_OUTSIDE){
          ss=0;
          if(axes[YLO]->getStyle()&AXIS_TICKS_MINOR) ss=axes[YLO]->getMinorTickSize();
          if(axes[YLO]->getStyle()&AXIS_TICKS_MAJOR) ss=axes[YLO]->getMajorTickSize();
          plotleft+=ss;
          }
        if(axes[YLO]->getStyle()&AXIS_NUMBERS){
          if(!(axes[YLO]->getStyle()&AXIS_NUMBERS_INSIDE)){
            if(axes[YLO]->getNumberStyle()&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
              plotleft+=textHeight(axes[YLO]->getNumberFont(),"8.8")+axes[YLO]->getNumberOffset();
              }
            else if(axes[YLO]->getNumberStyle()&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
              plotleft+=textWidth(axes[YLO]->getNumberFont(),"8.8")+axes[YLO]->getNumberOffset();
              }
            }
          }
        if(axes[YLO]->getStyle()&AXIS_LABEL){
          if(axes[YLO]->getLabelStyle()&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
            plotleft+=textHeight(axes[YLO]->getLabelFont(),axes[YLO]->getLabel())+axes[YLO]->getLabelOffset();
            }
          else if(axes[YLO]->getLabelStyle()&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
            plotleft+=textWidth(axes[YLO]->getLabelFont(),axes[YLO]->getLabel())+axes[YLO]->getLabelOffset();
            }
          }
        }

      // Account for the upper y-axis attributes to figure the plot size
      if(axes[YHI]->getStyle()&(AXIS_LABEL|AXIS_NUMBERS|AXIS_TICKS_MAJOR)){
        if(axes[YHI]->getStyle()&AXIS_TICKS_OUTSIDE){
          ss=0;
          if(axes[YHI]->getStyle()&AXIS_TICKS_MINOR) ss=axes[YHI]->getMinorTickSize();
          if(axes[YHI]->getStyle()&AXIS_TICKS_MAJOR) ss=axes[YHI]->getMajorTickSize();
          plotright-=ss;
          }
        if(axes[YHI]->getStyle()&AXIS_NUMBERS){
          if(!(axes[YHI]->getStyle()&AXIS_NUMBERS_INSIDE)){
            if(axes[YHI]->getNumberStyle()&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
              plotright-=textHeight(axes[YHI]->getNumberFont(),"8.8")+axes[YHI]->getNumberOffset();
              }
            else if(axes[YHI]->getNumberStyle()&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
              plotright-=textWidth(axes[YHI]->getNumberFont(),"8.8")+axes[YHI]->getNumberOffset();
              }
            }
          }
        if(axes[YHI]->getStyle()&AXIS_LABEL){
          if(axes[YHI]->getLabelStyle()&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
            plotright-=textHeight(axes[YHI]->getLabelFont(),axes[YHI]->getLabel())+axes[YHI]->getLabelOffset();
            }
          else if(axes[YHI]->getLabelStyle()&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
            plotright-=textWidth(axes[YHI]->getLabelFont(),axes[YHI]->getLabel())+axes[YHI]->getLabelOffset();
            }
          }
        }

      // Draw plot if there's still space for it
      if(plotleft<plotright && plottop<plotbottom){

        // Draw the caption
        if(options&CAPTION_SHOWN){
          if(!(options&(CAPTION_LEFT|CAPTION_RIGHT))){      // Caption centered above/below chart
            capx=(plotleft+plotright)/2;
            }
          if(!(options&(CAPTION_ABOVE|CAPTION_BELOW))){     // Caption centered left/right of chart
            capy=(plottop+plotbottom)/2;
            }
          dc.setFont(captionfont);
          dc.setForeground(captioncolor);
          drawText(dc,caption,capx,capy,caps);
          }

        // Now draw chart background
        drawRectangle(dc,chartstyle,plotleft,plottop,plotright-plotleft,plotbottom-plottop);

        // Draw the axes
        axes[XLO]->drawAxis(dc,plotleft,plotbottom,plotright,plotbottom,0,plottop-plotbottom);
        axes[YLO]->drawAxis(dc,plotleft,plotbottom,plotleft,plottop,plotright-plotleft,0);
        axes[XHI]->drawAxis(dc,plotleft,plottop,plotright,plottop,0,plotbottom-plottop);
        axes[YHI]->drawAxis(dc,plotright,plotbottom,plotright,plottop,plotleft-plotright,0);
        }
      }
    }
  }


long FX2DChart::onCmdXXX(FXObject*,FXSelector,void*){
  return 1;
  }


// Set chart fill style
void FX2DChart::setChartStyle(const FillStyle& fs){
  chartstyle=fs;
  recalc();
  }


// Save data
void FX2DChart::save(FXStream& store) const {
  FXChart::save(store);
  ////
  }


// Load data
void FX2DChart::load(FXStream& store){
  FXChart::load(store);
  ////
  }


// Destroy
FX2DChart::~FX2DChart(){
  delete axes[XLO];
  delete axes[YLO];
  delete axes[XHI];
  delete axes[YHI];
  }

}
