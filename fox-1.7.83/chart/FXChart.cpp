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
#include "fx.h"
#include "chartdefs.h"
#include "chartutils.h"
#include "FXAxis.h"
#include "FXChart.h"

/*
  Notes:
*/

#define CAPTION_MASK    (CAPTION_ABOVE|CAPTION_BELOW|CAPTION_LEFT|CAPTION_RIGHT|CAPTION_SHOWN)

using namespace FXCHART;


/*******************************************************************************/

namespace FXCHART {

// Map
FXDEFMAP(FXChart) FXChartMap[]={
  FXMAPFUNC(SEL_PAINT,0,FXChart::onPaint),
  FXMAPFUNC(SEL_CLIPBOARD_LOST,0,FXChart::onClipboardLost),
  FXMAPFUNC(SEL_CLIPBOARD_REQUEST,0,FXChart::onClipboardRequest),
  FXMAPFUNC(SEL_QUERY_TIP,0,FXChart::onQueryTip),
  FXMAPFUNC(SEL_QUERY_HELP,0,FXChart::onQueryHelp),
  };


// Object implementation
FXIMPLEMENT(FXChart,FXComposite,FXChartMap,ARRAYNUMBER(FXChartMap))


// Drag types
FXDragType FXChart::bmpType=0;
FXDragType FXChart::gifType=0;
FXDragType FXChart::jpgType=0;
FXDragType FXChart::pngType=0;
FXDragType FXChart::csvType=0;
FXDragType FXChart::tifType=0;


// Default back fill style
const FillStyle defaultBackStyle={
  nullptr,FXRGB(255,255,255),FXRGBA(0,0,0,0),FXRGB(255,255,255),FXRGB(0,0,255),STIPPLE_NONE,FILLSTYLE_SOLID
  };

/*******************************************************************************/

// Init
FXChart::FXChart(){
  flags|=FLAG_SHOWN|FLAG_ENABLED|FLAG_DROPTARGET;
  captionfont=(FXFont*)-1L;
  captioncolor=0;
  captionoffset=0;
  chart=(FXImage*)-1L;
  margintop=0;
  marginbottom=0;
  marginleft=0;
  marginright=0;
  }


// Make a chart
FXChart::FXChart(FXComposite* p,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb):FXComposite(p,opts,x,y,w,h){
  flags|=FLAG_SHOWN|FLAG_ENABLED|FLAG_DROPTARGET;
  chart=new FXImage(getApp(),nullptr,IMAGE_DITHER|IMAGE_SHMI|IMAGE_SHMP,w,h);
  captionfont=getApp()->getNormalFont();
  captioncolor=FXRGB(0,0,0);
  captionoffset=5;
  backstyle=defaultBackStyle;
  target=tgt;
  message=sel;
  margintop=pt;
  marginbottom=pb;
  marginleft=pl;
  marginright=pr;
  }


// Create window; register drag types
void FXChart::create(){
  FXComposite::create();
  chart->create();
  captionfont->create();
  if(backstyle.image) backstyle.image->create();
  if(!colorType) colorType=getApp()->registerDragType(colorTypeName);
  if(!textType) textType=getApp()->registerDragType(textTypeName);
  if(!bmpType) bmpType=getApp()->registerDragType(FXBMPImage::mimeType);
  if(!gifType) gifType=getApp()->registerDragType(FXGIFImage::mimeType);
  if(!jpgType) jpgType=getApp()->registerDragType(FXJPGImage::mimeType);
  if(!pngType) pngType=getApp()->registerDragType(FXPNGImage::mimeType);
  if(!tifType) tifType=getApp()->registerDragType(FXTIFImage::mimeType);
  if(!csvType) csvType=getApp()->registerDragType("Csv");
  }


// Detach window; zero out drag types
void FXChart::detach(){
  FXComposite::detach();
  chart->detach();
  captionfont->detach();
  if(backstyle.image) backstyle.image->detach();
  textType=0;
  colorType=0;
  bmpType=0;
  gifType=0;
  jpgType=0;
  pngType=0;
  csvType=0;
  tifType=0;
  }


// Draw contents of chart
void FXChart::drawSelf(FXDC& dc) const {
  drawRectangle(dc,backstyle,dc.getClipX(),dc.getClipY(),dc.getClipWidth(),dc.getClipHeight());
  }


// Update chart
void FXChart::updateChart(){
  FXDCWindow dc(chart);
  drawSelf(dc);
  }


// Layout the chart
void FXChart::layout(){
  FXuint dirty=(flags&FLAG_DIRTY);
  FXComposite::layout();
  if((chart->getWidth()!=width) || (chart->getHeight()!=height) || dirty){
    if((chart->getWidth()!=width) || (chart->getHeight()!=height)){
      chart->resize(width,height);
      }
    updateChart();
    }
  }


// We were asked about tip text
long FXChart::onQueryTip(FXObject* sender,FXSelector sel,void* ptr){
  if(FXWindow::onQueryTip(sender,sel,ptr)) return 1;
  if((flags&FLAG_TIP) && !tip.empty()){
    sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&tip);
    return 1;
    }
  return 0;
  }


// We were asked about status text
long FXChart::onQueryHelp(FXObject* sender,FXSelector sel,void* ptr){
  if(FXWindow::onQueryHelp(sender,sel,ptr)) return 1;
  if((flags&FLAG_HELP) && !help.empty()){
    sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&help);
    return 1;
    }
  return 0;
  }


// Lost clipboard, so destroy data
long FXChart::onClipboardLost(FXObject*,FXSelector,void*){
  return 1;
  }


// Request for clipboard data
long FXChart::onClipboardRequest(FXObject *sender,FXSelector sel,void *ptr){
  FXEvent *event=(FXEvent*)ptr;
  FXuchar *pointer;
  FXuval   length;

  // Try handling it in base class first
  if(FXComposite::onClipboardRequest(sender,sel,ptr)) return 1;

  // One of the supported image types?
  if(event->target==bmpType || event->target==gifType || event->target==jpgType || event->target==pngType){
    FXMemoryStream ms;

    // Read back pixels
    chart->restore();

    // Open memory stream
    ms.open(FXStreamSave,nullptr,0);

    // Render image to memory stream
    if(event->target==bmpType)
      fxsaveBMP(ms,chart->getData(),chart->getWidth(),chart->getHeight());
    else if(event->target==gifType)
      fxsaveGIF(ms,chart->getData(),chart->getWidth(),chart->getHeight());
    else if(event->target==jpgType)
      fxsaveJPG(ms,chart->getData(),chart->getWidth(),chart->getHeight(),75);
    else if(event->target==pngType)
      fxsavePNG(ms,chart->getData(),chart->getWidth(),chart->getHeight());
    else if(event->target==tifType)
      fxsaveTIF(ms,chart->getData(),chart->getWidth(),chart->getHeight(),0);

    // Grab buffered image
    ms.takeBuffer(pointer,length);

    // Close memory stream
    ms.close();

    // Release pixels
    chart->release();

    // Set DND data
    setDNDData(FROM_CLIPBOARD,event->target,pointer,length);
    return 1;
    }

  return 0;
  }


// Handle repaint
long FXChart::onPaint(FXObject*,FXSelector,void* ptr){
  FXDCWindow dc(this,(FXEvent*)ptr);
  dc.drawImage(chart,0,0);
  return 1;
  }



// Change caption
void FXChart::setCaption(const FXString& cap){
  if(caption!=cap){
    caption=cap;
    recalc();
    update();
    }
  }


// Set fill style
void FXChart::setBackStyle(const FillStyle& bs){
  backstyle=bs;
  recalc();
  update();
  }


// Change label text style
void FXChart::setCaptionFont(FXFont* font){
  if(!font){ fxerror("%s::setCaptionFont: NULL font specified.\n",getClassName()); }
  if(captionfont!=font){
    captionfont=font;
    recalc();
    update();
    }
  }


// Change caption color
void FXChart::setCaptionColor(FXColor clr){
  if(captioncolor!=clr){
    captioncolor=clr;
    recalc();
    update();
    }
  }


// Change caption style
void FXChart::setCaptionStyle(FXuint sty){
  FXuint opts=((sty^options)&CAPTION_MASK)^options;
  if(options!=opts){
    options=opts;
    recalc();
    update();
    }
  }


// Get caption style
FXuint FXChart::getCaptionStyle() const {
  return (options&CAPTION_MASK);
  }


// Change top margin
void FXChart::setMarginTop(FXint mt){
  if(margintop!=mt){
    margintop=mt;
    recalc();
    update();
    }
  }


// Change bottom margin
void FXChart::setMarginBottom(FXint mb){
  if(marginbottom!=mb){
    marginbottom=mb;
    recalc();
    update();
    }
  }


// Change left margin
void FXChart::setMarginLeft(FXint ml){
  if(marginleft!=ml){
    marginleft=ml;
    recalc();
    update();
    }
  }


// Change right margin
void FXChart::setMarginRight(FXint mr){
  if(marginright!=mr){
    marginright=mr;
    recalc();
    update();
    }
  }



// Change caption offset
void FXChart::setCaptionOffset(FXint off){
  if(captionoffset!=off){
    captionoffset=off;
    recalc();
    update();
    }
  }


// Change help text
void FXChart::setHelpText(const FXString& text){
  help=text;
  }


// Change tip text
void FXChart::setTipText(const FXString& text){
  tip=text;
  }


// Save data
void FXChart::save(FXStream& store) const {
  FXComposite::save(store);
  store << caption;
  store << captionfont;
  store << captioncolor;
  store << captionoffset;
  store << chart;
  store << margintop;
  store << marginbottom;
  store << marginleft;
  store << marginright;
  store << tip;
  store << help;
  }


// Load data
void FXChart::load(FXStream& store){
  FXComposite::load(store);
  store >> caption;
  store >> captionfont;
  store >> captioncolor;
  store >> captionoffset;
  store >> chart;
  store >> margintop;
  store >> marginbottom;
  store >> marginleft;
  store >> marginright;
  store >> tip;
  store >> help;
  }


// Destroy
FXChart::~FXChart(){
  delete chart;
  captionfont=(FXFont*)-1L;
  chart=(FXImage*)-1L;
  }

}
