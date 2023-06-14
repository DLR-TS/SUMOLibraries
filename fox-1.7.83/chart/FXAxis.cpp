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
#include "fx.h"
#include "chartdefs.h"
#include "chartutils.h"
#include "FXAxis.h"
#include "FXChart.h"

/*
  Notes:
*/

#define FUDGE_FACTOR  0.001     // For tickmark computation


using namespace FXCHART;


/*******************************************************************************/

namespace FXCHART {


// Construct axis
FXAxis::FXAxis(FXChart* ch,const FXString& nm,FXuint lblsty,FXuint numsty,FXuint sty):chart(ch),label(nm),labelstyle(lblsty),numberstyle(numsty),style(sty){
  labelfont=chart->getApp()->getNormalFont();
  numberfont=chart->getApp()->getNormalFont();
  datarange.minimum=0.0;
  datarange.maximum=1.0;
  axisrange.minimum=0.0;
  axisrange.maximum=1.0;
  majorspace=0.1;
  minorspace=0.01;
  labelcolor=FXRGB(0,0,0);
  numbercolor=FXRGB(0,0,0);
  axiscolor=FXRGB(0,0,0);
  majortickcolor=FXRGB(0,0,0);
  minortickcolor=FXRGB(0,0,0);
  majorgridcolor=FXRGB(128,128,128);
  minorgridcolor=FXRGB(192,192,192);
  majorticksize=7;
  minorticksize=4;
  numberoffset=3;
  labeloffset=6;
  precision=3;
  }


// Create server-side resources
void FXAxis::create(){
  labelfont->create();
  numberfont->create();
  }


// Detach server-side resources
void FXAxis::detach(){
  labelfont->detach();
  numberfont->detach();
  }

#if defined(WIN32) || defined(__minix)
#define lrint(x) ((x)+0.5)
#endif

// Draw axis
void FXAxis::drawAxis(FXDC& dc,FXint fx,FXint fy,FXint tx,FXint ty,FXint dx,FXint dy) const {
  if(style&(AXIS_LABEL|AXIS_NUMBERS|AXIS_GRID_MAJOR|AXIS_GRID_MINOR|AXIS_TICKS_MAJOR|AXIS_TICKS_MINOR|AXIS_LINE)){
    FXint ex,ey,tin,tout,x,y,majh,majl,minh,minl,i;
    FXdouble mulx,muly;
    FXString num;

    // Unit direction
    ex=0; if(dx) ex=dx/FXABS(dx);
    ey=0; if(dy) ey=dy/FXABS(dy);

    // Multipliers
    mulx=((FXdouble)(tx-fx))/(axisrange.maximum-axisrange.minimum);
    muly=((FXdouble)(ty-fy))/(axisrange.maximum-axisrange.minimum);

    // Minor grid lines
    if(style&AXIS_GRID_MINOR){
      dc.setForeground(minorgridcolor);
      minl=(FXint)Math::ceil((axisrange.minimum+FUDGE_FACTOR*minorspace)/minorspace);
      minh=(FXint)Math::floor((axisrange.maximum-FUDGE_FACTOR*minorspace)/minorspace);
      for(i=minl; i<=minh; i++){
        x=fx+lrint(mulx*(i*minorspace-axisrange.minimum));
        y=fy+lrint(muly*(i*minorspace-axisrange.minimum));
        dc.drawLine(x,y,x+dx,y+dy);
        }
      }

    // Major grid lines
    if(style&AXIS_GRID_MAJOR){
      dc.setForeground(majorgridcolor);
      majl=(FXint)Math::ceil((axisrange.minimum+FUDGE_FACTOR*majorspace)/majorspace);
      majh=(FXint)Math::floor((axisrange.maximum-FUDGE_FACTOR*majorspace)/majorspace);
      for(i=majl; i<=majh; i++){
        x=fx+lrint(mulx*(i*majorspace-axisrange.minimum));
        y=fy+lrint(muly*(i*majorspace-axisrange.minimum));
        dc.drawLine(x,y,x+dx,y+dy);
        }
      }

    // Axis line
    if(style&AXIS_LINE){
      dc.setForeground(axiscolor);
      dc.drawLine(fx,fy,tx,ty);
      }

    tin=tout=0;

    // Minor ticks
    if(style&AXIS_TICKS_MINOR){
      if(style&AXIS_TICKS_INSIDE) tin=minorticksize;
      if(style&AXIS_TICKS_OUTSIDE) tout=minorticksize;
      dc.setForeground(minortickcolor);
      minl=(FXint)Math::ceil(axisrange.minimum/minorspace);
      minh=(FXint)Math::floor(axisrange.maximum/minorspace);
      for(i=minl; i<=minh; i++){
        x=fx+lrint(mulx*(i*minorspace-axisrange.minimum));
        y=fy+lrint(muly*(i*minorspace-axisrange.minimum));
        dc.drawLine(x-tout*ex,y-tout*ey,x+tin*ex,y+tin*ey);
        }
      }

    // Major ticks
    if(style&AXIS_TICKS_MAJOR){
      if(style&AXIS_TICKS_INSIDE) tin=majorticksize;
      if(style&AXIS_TICKS_OUTSIDE) tout=majorticksize;
      dc.setForeground(majortickcolor);
      majl=(FXint)Math::ceil(axisrange.minimum/majorspace);
      majh=(FXint)Math::floor(axisrange.maximum/majorspace);
      for(i=majl; i<=majh; i++){
        x=fx+lrint(mulx*(i*majorspace-axisrange.minimum));
        y=fy+lrint(muly*(i*majorspace-axisrange.minimum));
        dc.drawLine(x-tout*ex,y-tout*ey,x+tin*ex,y+tin*ey);
        }
      }

    // Numbers
    if(style&AXIS_NUMBERS){
      dc.setFont(numberfont);
      dc.setForeground(numbercolor);
      if(style&AXIS_NUMBERS_INSIDE){
        tin+=numberoffset;
        majl=(FXint)Math::ceil((axisrange.minimum+FUDGE_FACTOR*majorspace)/majorspace);
        majh=(FXint)Math::floor((axisrange.maximum-FUDGE_FACTOR*majorspace)/majorspace);
        for(i=majl; i<=majh; i++){
          x=fx+lrint(mulx*(i*majorspace-axisrange.minimum));
          y=fy+lrint(muly*(i*majorspace-axisrange.minimum));
          num.format("%g",i*majorspace);
          drawText(dc,num,x+tin*ex,y+tin*ey,numberstyle);
          }
        }
      else{
        tout+=numberoffset;
        majl=(FXint)Math::ceil(axisrange.minimum/majorspace);
        majh=(FXint)Math::floor(axisrange.maximum/majorspace);
        for(i=majl; i<=majh; i++){
          x=fx+lrint(mulx*(i*majorspace-axisrange.minimum));
          y=fy+lrint(muly*(i*majorspace-axisrange.minimum));
          num.format("%g",i*majorspace);
          drawText(dc,num,x-tout*ex,y-tout*ey,numberstyle);
          }
        if(numberstyle&(TEXT_ATTACH_TOP|TEXT_ATTACH_BOTTOM)){
          tout+=textHeight(numberfont,"8.8");
          }
        else if(numberstyle&(TEXT_ATTACH_LEFT|TEXT_ATTACH_RIGHT)){
          tout+=textWidth(numberfont,"8.8");
          }
        }
      }

    // Axis label
    if(style&AXIS_LABEL){
      dc.setFont(labelfont);
      dc.setForeground(labelcolor);
      tout+=labeloffset;

      // Horizontal attachment
      if(labelstyle&TEXT_ATTACH_LEFT) x=fx;
      else if(labelstyle&TEXT_ATTACH_RIGHT) x=tx;
      else x=(fx+tx)/2;

      // Vertical attachment
      if(labelstyle&TEXT_ATTACH_TOP) y=fy;
      else if(labelstyle&TEXT_ATTACH_BOTTOM) y=ty;
      else y=(fy+ty)/2;

//dc.drawLine(x-tout*ex-4,y-tout*ey,x-tout*ex+4,y-tout*ey);
//dc.drawLine(x-tout*ex,y-tout*ey-4,x-tout*ex,y-tout*ey+4);

      drawText(dc,label,x-tout*ex,y-tout*ey,labelstyle);
      }
    }
  }


// Set axis style
void FXAxis::setStyle(FXuint sty){
  if(style!=sty){
    style=sty;
    chart->recalc();
    }
  }


// Change axis label
void FXAxis::setLabel(const FXString& text){
  if(label!=text){
    label=text;
    chart->recalc();
    }
  }


// Change label text style
void FXAxis::setLabelStyle(FXuint sty){
  if(labelstyle!=sty){
    labelstyle=sty;
    chart->recalc();
    }
  }


// Change label text color
void FXAxis::setLabelColor(FXColor clr){
  if(labelcolor!=clr){
    labelcolor=clr;
    chart->recalc();
    }
  }


// Change label text style
void FXAxis::setLabelFont(FXFont* font){
  if(!font){ fxerror("FXAxis::setLabelFont: NULL font specified.\n"); }
  if(labelfont!=font){
    labelfont=font;
    chart->recalc();
    }
  }


// Change number text style
void FXAxis::setNumberStyle(FXuint sty){
  if(numberstyle!=sty){
    numberstyle=sty;
    chart->recalc();
    }
  }


// Change number color
void FXAxis::setNumberColor(FXColor clr){
  if(numbercolor!=clr){
    numbercolor=clr;
    chart->recalc();
    }
  }


// Change label text style
void FXAxis::setNumberFont(FXFont* font){
  if(!font){ fxerror("FXAxis::setNumberFont: NULL font specified.\n"); }
  if(numberfont!=font){
    numberfont=font;
    chart->recalc();
    }
  }


// Set data range
void FXAxis::setDataRange(FXdouble min,FXdouble max){
  if(min>max){ fxerror("FXAxis::setDataRange: bad range argument.\n"); }
  if(datarange.minimum!=min || datarange.maximum!=max){
    datarange.minimum=min;
    datarange.maximum=max;
    chart->recalc();
    }
  }


// Set axis range
void FXAxis::setAxisRange(FXdouble min,FXdouble max){
  if(min>max){ fxerror("FXAxis::setAxisRange: bad range argument.\n"); }
  if(axisrange.minimum!=min || axisrange.maximum!=max){
    if(min==max){
      if(0.0<min){ min=0.0; }
      else if(max<0.0){ max=0.0; }
      else{ min=0.0; max=1.0; }
      }
    if(style&AXIS_TICKS_AUTO){
      getTickSpacing(majorspace,minorspace,min,max);
      }
    if(style&AXIS_ROUND){
      min=majorspace*Math::floor(min/majorspace);
      max=majorspace*Math::ceil(max/majorspace);
      }
    FXASSERT(min<max);
    axisrange.minimum=min;
    axisrange.maximum=max;
    chart->recalc();
    }
  }


// Change major tick size
void FXAxis::setMajorTickSize(FXint sz){
  if(majorticksize!=sz){
    majorticksize=sz;
    chart->recalc();
    }
  }


// Change minor tick size
void FXAxis::setMinorTickSize(FXint sz){
  if(minorticksize!=sz){
    minorticksize=sz;
    chart->recalc();
    }
  }


// Change major tick space
void FXAxis::setMajorTickSpace(FXdouble space){
  if(space<=0.0){ fxerror("FXAxis::setMajorTickSpace: tick space should be > 0.\n"); }
  if(majorspace!=space){
    majorspace=space;
    chart->recalc();
    }
  }


// Change minor tick space
void FXAxis::setMinorTickSpace(FXdouble space){
  if(space<=0.0){ fxerror("FXAxis::setMinorTickSpace: tick space should be > 0.\n"); }
  if(minorspace!=space){
    minorspace=space;
    chart->recalc();
    }
  }


// Change number offset
void FXAxis::setNumberOffset(FXint off){
  if(numberoffset!=off){
    numberoffset=off;
    chart->recalc();
    }
  }


// Change label offset
void FXAxis::setLabelOffset(FXint off){
  if(labeloffset!=off){
    labeloffset=off;
    chart->recalc();
    }
  }


// Reset axis to data range
void FXAxis::resetAxisRange(){
  setAxisRange(datarange.minimum,datarange.maximum);
  }


// Destroy axis
FXAxis::~FXAxis(){
  chart=(FXChart*)-1L;
  labelfont=(FXFont*)-1L;
  numberfont=(FXFont*)-1L;
  }


}
