/********************************************************************************
*                                                                               *
*                                  Chart  Test                                  *
*                                                                               *
*********************************************************************************
* Copyright (C) 2003,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"
#include "chart.h"
#include "icons.h"

/*
  Notes:

*/

/*******************************************************************************/


// Mini application object
class ChartWindow : public FXMainWindow {
  FXDECLARE(ChartWindow)
protected:
  FX2DPlot     *plot;
  FXMenuBar    *menubar;
  FXStatusBar  *statusbar;
  FXMenuPane   *filemenu;
  FXBMPImage   *image;
  FXFont       *captionFont;    // Caption font
  FXFont       *horzLabelFont;  // Normal horizontal font
  FXFont       *vertLabelFont;  // Vertical font
  FXFont       *horzNumberFont; // Normal horizontal font
  FXFont       *vertNumberFont; // Vertical font
protected:
  ChartWindow(){}
private:
  ChartWindow(const ChartWindow&);
  ChartWindow &operator=(const ChartWindow&);
public:
  long onCmdChart(FXObject*,FXSelector,void*);
public:
  enum {
    ID_CHART=FXMainWindow::ID_LAST
    };
public:
  ChartWindow(FXApp *a);
  virtual void create();
  virtual ~ChartWindow();
  };



/*******************************************************************************/

// Map
FXDEFMAP(ChartWindow) ChartWindowMap[]={
  FXMAPFUNC(SEL_COMMAND,  ChartWindow::ID_CHART,     ChartWindow::onCmdChart),
  };


// Object implementation
FXIMPLEMENT(ChartWindow,FXMainWindow,ChartWindowMap,ARRAYNUMBER(ChartWindowMap))



// Make some windows
ChartWindow::ChartWindow(FXApp* a):FXMainWindow(a,"Chart Test",nullptr,nullptr,DECOR_ALL,20,20,700,460){

  // Menubar
  menubar=new FXMenuBar(this,FRAME_RAISED|LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
  statusbar=new FXStatusBar(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|FRAME_RAISED|STATUSBAR_WITH_DRAGCORNER);

  // File menu
  filemenu=new FXMenuPane(this);
  new FXMenuCommand(filemenu,"&Quit\tCtl-Q",nullptr,getApp(),FXApp::ID_QUIT);
  new FXMenuTitle(menubar,"&File",nullptr,filemenu);

  // Container
  FXHorizontalFrame *container=new FXHorizontalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED);

  // Well for chart
  FXHorizontalFrame *chartwell=new FXHorizontalFrame(container,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0, 0,0,0,0, 0,0);

  // Chart
  plot=new FX2DPlot(chartwell,this,ID_CHART,CAPTION_ABOVE|CAPTION_SHOWN|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,20,20,20,20);

  plot->setCaption("Multi-line\ncaptions are great!");

  // Make tile pattern
  image=new FXBMPImage(getApp(),marble);

  // Make label fonts
  horzLabelFont=new FXFont(getApp(),"helvetica",14,FXFont::Normal,FXFont::Straight,FONTENCODING_DEFAULT,FXFont::NonExpanded,FXFont::Scalable|FXFont::Rotatable);
  vertLabelFont=new FXFont(getApp(),"helvetica",14,FXFont::Normal,FXFont::Straight,FONTENCODING_DEFAULT,FXFont::NonExpanded,FXFont::Scalable|FXFont::Rotatable);
  vertLabelFont->setAngle(90*64);
//vertLabelFont->setAngle(180*64);

  // Make number fonts
  horzNumberFont=new FXFont(getApp(),"helvetica",9,FXFont::Normal,FXFont::Straight,FONTENCODING_DEFAULT,FXFont::NonExpanded,FXFont::Scalable|FXFont::Rotatable);
  vertNumberFont=new FXFont(getApp(),"helvetica",9,FXFont::Normal,FXFont::Straight,FONTENCODING_DEFAULT,FXFont::NonExpanded,FXFont::Scalable|FXFont::Rotatable);
  vertNumberFont->setAngle(90*64);
//vertNumberFont->setAngle(270*64);

  // Make caption font
  captionFont=new FXFont(getApp(),"helvetica",18,FXFont::Normal,FXFont::Straight,FONTENCODING_DEFAULT,FXFont::NonExpanded,FXFont::Scalable|FXFont::Rotatable);
  //captionFont->setAngle(90*64);

  // Set up fill style
  FillStyle bs={image,FXRGB(128,255,255),FXRGB(128,128,255),FXRGB(200,200,200),FXRGB(128,128,128),STIPPLE_NONE,FILLSTYLE_HORIZONTAL};
  FillStyle cs={image,FXRGB(128,255,255),FXRGB(128,128,255),FXRGB(230,230,230),FXRGB(128,128,255),STIPPLE_NONE,FILLSTYLE_VERTICAL};

  // Change fill styles
  plot->setBackStyle(bs);
  plot->setChartStyle(cs);

  // Change axis labels
  plot->getAxis(FX2DChart::XLO)->setLabel("X lo gee");
  plot->getAxis(FX2DChart::XHI)->setLabel("X hi gee");
  plot->getAxis(FX2DChart::YLO)->setLabel("Y lo gee");
  plot->getAxis(FX2DChart::YHI)->setLabel("Y hi gee");

  // Set caption font
  plot->setCaptionFont(captionFont);

  // Set label fonts
  plot->getAxis(FX2DChart::XLO)->setLabelFont(horzLabelFont);
  plot->getAxis(FX2DChart::XHI)->setLabelFont(horzLabelFont);
  plot->getAxis(FX2DChart::YLO)->setLabelFont(vertLabelFont);
  plot->getAxis(FX2DChart::YHI)->setLabelFont(vertLabelFont);

  // Set number fonts
  plot->getAxis(FX2DChart::XLO)->setNumberFont(horzNumberFont);
  plot->getAxis(FX2DChart::XHI)->setNumberFont(horzNumberFont);
  plot->getAxis(FX2DChart::YLO)->setNumberFont(vertNumberFont);
  plot->getAxis(FX2DChart::YHI)->setNumberFont(vertNumberFont);

  // Change text styles
  plot->getAxis(FX2DChart::XLO)->setLabelStyle(TEXT_JUSTIFY_CENTER|TEXT_ATTACH_TOP);
  plot->getAxis(FX2DChart::XHI)->setLabelStyle(TEXT_JUSTIFY_CENTER|TEXT_ATTACH_BOTTOM);
  plot->getAxis(FX2DChart::YLO)->setLabelStyle(TEXT_JUSTIFY_CENTER|TEXT_ATTACH_RIGHT);
  plot->getAxis(FX2DChart::YHI)->setLabelStyle(TEXT_JUSTIFY_CENTER|TEXT_ATTACH_LEFT);

  plot->getAxis(FX2DChart::XLO)->setAxisRange(-0.012345,0.12345);
  }


// Create image
void ChartWindow::create(){
  FXMainWindow::create();
  image->create();
  captionFont->create();
  horzLabelFont->create();
  vertLabelFont->create();
  horzNumberFont->create();
  vertNumberFont->create();
  }


// Command from chart
long ChartWindow::onCmdChart(FXObject*,FXSelector,void*){
  return 1;
  }


// Clean up
ChartWindow::~ChartWindow(){
  delete filemenu;
  delete image;
  delete captionFont;
  delete horzLabelFont;
  delete vertLabelFont;
  delete horzNumberFont;
  delete vertNumberFont;
  }


/*******************************************************************************/


// Start the whole thing
int main(int argc,char *argv[]){

  // Make application
  FXApp application("ChartWindow","FoxTest");

  // Open display
  application.init(argc,argv);

  // Main window
  ChartWindow* window=new ChartWindow(&application);

  // Create app
  application.create();

  // Show it
  window->show(PLACEMENT_SCREEN);

  // Run
  return application.run();
  }
