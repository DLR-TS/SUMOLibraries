/********************************************************************************
*                                                                               *
*                          U t i l i t y - F u n c t i o n s                    *
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


/*
  Notes:
*/


using namespace FXCHART;


/*******************************************************************************/

namespace FXCHART {


// Measure text width
FXint textWidth(FXFont *font,const FXString& string){
  FXint angle=font->getAngle()/(90*64);
  FXint beg=0,tw=0,end,w;
  do{
    end=beg;
    while(end<string.length() && string[end]!='\n') end++;
    if(angle&1){
      tw+=font->getFontHeight();
      }
    else{
      if((w=font->getTextWidth(&string[beg],end-beg))>tw) tw=w;
      }
    beg=end+1;
    }
  while(end<string.length());
  return tw;
  }


// Measure text height
FXint textHeight(FXFont *font,const FXString& string){
  FXint angle=font->getAngle()/(90*64);
  FXint beg=0,th=0,end,h;
  do{
    end=beg;
    while(end<string.length() && string[end]!='\n') end++;
    if(angle&1){
      if((h=font->getTextWidth(&string[beg],end-beg))>th) th=h;
      }
    else{
      th+=font->getFontHeight();
      }
    beg=end+1;
    }
  while(end<string.length());
  return th;
  }


// Draw text
void drawText(FXDC& dc,const FXString& string,FXint x,FXint y,FXuint style){
  FXint angle=dc.getFont()->getAngle()/(90*64);
  FXint tw=textWidth(dc.getFont(),string);
  FXint th=textHeight(dc.getFont(),string);
  FXint beg,end,xx,yy,tx,ty;


  // Text box vertical attachment
  if(style&TEXT_ATTACH_TOP) yy=y;
  else if(style&TEXT_ATTACH_BOTTOM) yy=y-th;
  else yy=y-th/2;

  // Text box horizontal attachment
  if(style&TEXT_ATTACH_LEFT) xx=x;
  else if(style&TEXT_ATTACH_RIGHT) xx=x-tw;
  else xx=x-tw/2;

  //FXTRACE((1," x=%d y=%d xx=%d yy=%d ",x,y,xx,yy));

  // Draw text
  switch(angle){
    case 1:         // Bottom to top
      //FXTRACE((1," Case 1: %s w=%d h=%d ",string.text(),tw,th));
      beg=0;
      tx=xx+dc.getFont()->getFontAscent();
      do{
        end=beg;
        while(end<string.length() && string[end]!='\n') end++;
        if(style&TEXT_JUSTIFY_LEFT) ty=yy-th;
        else if(style&TEXT_JUSTIFY_RIGHT) ty=yy+dc.getFont()->getTextWidth(&string[beg],end-beg);
        else ty=yy+th/2+dc.getFont()->getTextWidth(&string[beg],end-beg)/2;
        //FXTRACE((1," tx=%d ty=%d a=%d\n",tx,ty,dc.getFont()->getFontAscent()));
        dc.drawText(tx,ty,&string[beg],end-beg);
        tx+=dc.getFont()->getFontHeight();
        beg=end+1;
        }
      while(end<string.length());
      break;
    case 2:        // Right to left
      //FXTRACE((1," Case 2: %s w=%d h=%d ",string.text(),tw,th));
      beg=0;
      ty=yy+th-dc.getFont()->getFontAscent();
      do{
        end=beg;
        while(end<string.length() && string[end]!='\n') end++;
        if(style&TEXT_JUSTIFY_LEFT) tx=xx+tw;
        else if(style&TEXT_JUSTIFY_RIGHT) tx=xx+dc.getFont()->getTextWidth(&string[beg],end-beg);
        else tx=xx+tw/2+dc.getFont()->getTextWidth(&string[beg],end-beg)/2;
        //FXTRACE((1," tx=%d ty=%d a=%d\n",tx,ty,dc.getFont()->getFontAscent()));
        dc.drawText(tx,ty,&string[beg],end-beg);
        ty-=dc.getFont()->getFontHeight();
        beg=end+1;
        }
      while(end<string.length());
      break;
    case 3:        // Top to bottom
      //FXTRACE((1," Case 3: %s w=%d h=%d ",string.text(),tw,th));
      beg=0;
      tx=xx+tw-dc.getFont()->getFontAscent();
      do{
        end=beg;
        while(end<string.length() && string[end]!='\n') end++;
        if(style&TEXT_JUSTIFY_LEFT) ty=yy;
        else if(style&TEXT_JUSTIFY_RIGHT) ty=yy+th-dc.getFont()->getTextWidth(&string[beg],end-beg);
        else ty=yy+th/2-dc.getFont()->getTextWidth(&string[beg],end-beg)/2;
        //FXTRACE((1," tx=%d ty=%d a=%d\n",tx,ty,dc.getFont()->getFontAscent()));
        dc.drawText(tx,ty,&string[beg],end-beg);
        tx-=dc.getFont()->getFontHeight();
        beg=end+1;
        }
      while(end<string.length());
      break;
    default:            // Left to right
      //FXTRACE((1," Case 4: %s w=%d h=%d ",string.text(),tw,th));
      beg=0;
      ty=yy+dc.getFont()->getFontAscent();
      do{
        end=beg;
        while(end<string.length() && string[end]!='\n') end++;
        if(style&TEXT_JUSTIFY_LEFT) tx=xx;
        else if(style&TEXT_JUSTIFY_RIGHT) tx=xx+tw-dc.getFont()->getTextWidth(&string[beg],end-beg);
        else tx=xx+tw/2-dc.getFont()->getTextWidth(&string[beg],end-beg)/2;
        //FXTRACE((1," tx=%d ty=%d a=%d\n",tx,ty,dc.getFont()->getFontAscent()));
        dc.drawText(tx,ty,&string[beg],end-beg);
        ty+=dc.getFont()->getFontHeight();
        beg=end+1;
        }
      while(end<string.length());
      break;
    }
  }



// Marker size is in pixels; x,y are canvas coordinates of the center
void drawMarker(FXDC& dc,const Marker& ms,FXint x,FXint y){
  FXint s=ms.size;
  FXint h=s>>1;
  FXPoint p[5];
  dc.setForeground(ms.color);
  switch(ms.style){
    case MARKER_SQUARE:
      dc.drawRectangle(x-h,y-h,s,s);
      break;
    case MARKER_SQUARE|MARKER_SOLID:
      dc.fillRectangle(x-h,y-h,s,s);
      break;
    case MARKER_CIRCLE:
      dc.drawArc(x-h,y-h,s,s,0,23040);
      break;
    case MARKER_CIRCLE|MARKER_SOLID:
      dc.fillArc(x-h,y-h,s,s,0,23040);
      break;
    case MARKER_DIAMOND:
      p[0].x=p[2].x=p[4].x=x;
      p[1].x=x-h;
      p[3].x=p[1].x+s;
      p[0].y=p[4].y=y-h;
      p[1].y=p[3].y=y;
      p[2].y=p[0].y+s;
      dc.drawLines(p,5);
      break;
    case MARKER_DIAMOND|MARKER_SOLID:
      p[0].x=p[2].x=x;
      p[1].x=x-h;
      p[3].x=p[1].x+s;
      p[0].y=y-h;
      p[1].y=p[3].y=y;
      p[2].y=p[0].y+s;
      dc.fillPolygon(p,4);
      break;
    case MARKER_TRIANGLE_UP:
      p[0].x=p[3].x=x;
      p[1].x=x-h;
      p[2].x=x+h;
      p[0].y=p[3].y=y-h;
      p[1].y=p[2].y=y+h;
      dc.drawLines(p,4);
      break;
    case MARKER_TRIANGLE_UP|MARKER_SOLID:
      p[0].x=x;
      p[1].x=x-h;
      p[2].x=x+h;
      p[0].y=y-h;
      p[1].y=p[2].y=y+h;
      dc.fillPolygon(p,3);
      break;
    case MARKER_TRIANGLE_DN:
      p[0].x=p[3].x=x-h;
      p[1].x=x;
      p[2].x=x+h;
      p[0].y=p[2].y=p[3].y=y-h;
      p[1].y=y+h;
      dc.drawLines(p,4);
      break;
    case MARKER_TRIANGLE_DN|MARKER_SOLID:
      p[0].x=x-h;
      p[1].x=x;
      p[2].x=x+h;
      p[0].y=p[2].y=y-h;
      p[1].y=y+h;
      dc.fillPolygon(p,3);
      break;
    case MARKER_TRIANGLE_LT:
      p[0].x=p[3].x=x-h;
      p[1].x=p[2].x=x+h;
      p[0].y=p[3].y=y;
      p[1].y=y+h;
      p[2].y=y-h;
      dc.drawLines(p,4);
      break;
    case MARKER_TRIANGLE_LT|MARKER_SOLID:
      p[0].x=x-h;
      p[1].x=p[2].x=x+h;
      p[0].y=y;
      p[1].y=y+h;
      p[2].y=y-h;
      dc.fillPolygon(p,3);
      break;
    case MARKER_TRIANGLE_RT:
      p[0].x=p[3].x=x+h;
      p[1].x=p[2].x=x-h;
      p[0].y=p[3].y=y;
      p[1].y=y-h;
      p[2].y=y+h;
      dc.drawLines(p,4);
      break;
    case MARKER_TRIANGLE_RT|MARKER_SOLID:
      p[0].x=x+h;
      p[1].x=p[2].x=x-h;
      p[0].y=y;
      p[1].y=y-h;
      p[2].y=y+h;
      dc.fillPolygon(p,3);
      break;
    }
  }


// Draw rectangle
void drawRectangle(FXDC& dc,const FillStyle& fs,FXint x,FXint y,FXint w,FXint h){
  if(0<w && 0<h){
    switch(fs.style){
      case FILLSTYLE_SOLID:
        dc.setStipple(STIPPLE_NONE);
        dc.setFillStyle(FILL_SOLID);
        dc.setForeground(fs.forecolor);
        dc.fillRectangle(x,y,w,h);
        break;
      case FILLSTYLE_HATCH:
        if(fs.backcolor){
          dc.setFillStyle(FILL_OPAQUESTIPPLED);
          dc.setBackground(fs.backcolor);
          }
        else{
          dc.setFillStyle(FILL_STIPPLED);
          }
        dc.setStipple((FXStipplePattern)fs.hatch);
        dc.setForeground(fs.forecolor);
        dc.fillRectangle(x,y,w,h);
        break;
      case FILLSTYLE_TEXTURE:
        dc.setStipple(STIPPLE_NONE);
        dc.setFillStyle(FILL_TILED);
        dc.setTile(fs.image);
        dc.fillRectangle(x,y,w,h);
        break;
      case FILLSTYLE_IMAGE:
        dc.setStipple(STIPPLE_NONE);
        dc.setFillStyle(FILL_TILED);
        dc.setTile(fs.image);
        dc.fillRectangle(x,y,w,h);
        break;
      case FILLSTYLE_HORIZONTAL:
        dc.setStipple(STIPPLE_NONE);
        dc.setFillStyle(FILL_SOLID);
        dc.fillHorizontalGradient(x,y,w,h,fs.lower,fs.upper);
        break;
      case FILLSTYLE_VERTICAL:
        dc.setStipple(STIPPLE_NONE);
        dc.setFillStyle(FILL_SOLID);
        dc.fillVerticalGradient(x,y,w,h,fs.lower,fs.upper);
        break;
      }
    }
  }


// Determine tick spacing
void getTickSpacing(FXdouble& major,FXdouble& minor,FXdouble min,FXdouble max){
  FXdouble range=max-min;
  FXdouble delta=Math::pow(10.0,Math::floor(Math::log10(range)));
  FXdouble frac=range/delta;
  if(frac<1.5){
    major=0.1*delta;
    minor=0.02*delta;
    }
  else if(frac<3.0){
    major=0.2*delta;
    minor=0.05*delta;
    }
  else if(frac<7.0){
    major=0.5*delta;
    minor=0.1*delta;
    }
  else{
    major=delta;
    minor=0.1*delta;
    }
  }

}
