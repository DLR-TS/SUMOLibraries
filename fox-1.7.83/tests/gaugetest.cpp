/********************************************************************************
*                                                                               *
*                                 Gauge Widget Test                             *
*                                                                               *
*********************************************************************************
* Copyright (C) 2010,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"

/*******************************************************************************/


// Mini application object
class GaugeWindow : public FXMainWindow {
  FXDECLARE(GaugeWindow)
protected:
  FXMenuBar    *menubar;        // Menu bar
  FXMenuPane   *filemenu;       // File menu
  FXDataTarget  valuetarget;    // So we can connect all gauges to a single value
  FXdouble      value;          // This is the value
protected:
  GaugeWindow(){}
public:
  long onCmdXXX(FXObject*,FXSelector,void*);
public:
  enum {
    ID_XXX=FXMainWindow::ID_LAST,
    ID_LAST
    };
public:
  GaugeWindow(FXApp *a);
  virtual void create();
  virtual ~GaugeWindow();
  };


/*******************************************************************************/


// Map
FXDEFMAP(GaugeWindow) GaugeWindowMap[]={
  FXMAPFUNC(SEL_COMMAND,GaugeWindow::ID_XXX,GaugeWindow::onCmdXXX),
  };


// Object implementation
FXIMPLEMENT(GaugeWindow,FXMainWindow,GaugeWindowMap,ARRAYNUMBER(GaugeWindowMap))



// Make some windows
GaugeWindow::GaugeWindow(FXApp* a):FXMainWindow(a,"Gauge Widget Test",nullptr,nullptr,DECOR_ALL,20,20,800,550){

  // Clear value
  value=0.0;

  // Connect target to value
  valuetarget.connect(value);

  // Menubar
  menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);

  // File menu
  filemenu=new FXMenuPane(this);
  new FXMenuCommand(filemenu,"&Quit\tCtl-Q",nullptr,getApp(),FXApp::ID_QUIT);
  new FXMenuTitle(menubar,"&File",nullptr,filemenu);

  // Matrix for sliders
  FXMatrix* slidermatrix=new FXMatrix(this,2,MATRIX_BY_COLUMNS|LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
  new FXLabel(slidermatrix,tr("Gauge value:"),nullptr,JUSTIFY_LEFT|JUSTIFY_CENTER_Y|LAYOUT_CENTER_Y);
  FXRealSlider *slider=new FXRealSlider(slidermatrix,&valuetarget,FXDataTarget::ID_VALUE,SLIDER_HORIZONTAL|SLIDER_INSIDE_BAR|LAYOUT_FILL_X|LAYOUT_CENTER_Y|LAYOUT_FILL_COLUMN,0,0,0,0);
  slider->setRange(0.0,1.0);

  // Separator
  new FXHorizontalSeparator(this,LAYOUT_SIDE_BOTTOM|SEPARATOR_GROOVE|LAYOUT_FILL_X);

  // Arange nicely
  FXMatrix* matrix=new FXMatrix(this,2,MATRIX_BY_COLUMNS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  new FXLabel(matrix,"Basic\nClockwise\nArc",nullptr,LAYOUT_CENTER_Y|LAYOUT_CENTER_X|JUSTIFY_RIGHT|LAYOUT_FILL_ROW);

  // Make test gauge
  FXGauge *g1=new FXGauge(matrix,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW|LAYOUT_FILL,150,-120,0,0,0,0,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD);
//  FXGauge *g1=new FXGauge(matrix,GAUGE_PIVOT_INSIDE|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW|LAYOUT_FILL,150,-120,0,0,0,0,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD);
//  FXGauge *g1=new FXGauge(matrix,GAUGE_ELLIPTICAL|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW|LAYOUT_FILL,150,-120,0,0,0,0,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD);
//  FXGauge *g1=new FXGauge(matrix,GAUGE_ELLIPTICAL|GAUGE_PIVOT_INSIDE|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW|LAYOUT_FILL,150,-120,0,0,0,0,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD);
//  FXGauge *g1=new FXGauge(matrix,GAUGE_ELLIPTICAL|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW|LAYOUT_FILL,190,-110,0,0,0,0,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD);
//  FXGauge *g1=new FXGauge(matrix,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW|LAYOUT_FILL,170,-70,0,0,0,0,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD,DEFAULT_PAD);
  g1->setTarget(&valuetarget);
  g1->setSelector(FXDataTarget::ID_VALUE);

  // Make tooltip
  new FXToolTip(getApp());
  }


// XXX
long GaugeWindow::onCmdXXX(FXObject*,FXSelector,void*){
  return 1;
  }


// Create window
void GaugeWindow::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
  }


// Clean up
GaugeWindow::~GaugeWindow(){
  delete filemenu;
  }

/*******************************************************************************/


// Start the whole thing
int main(int argc,char *argv[]){

  // Make application
  FXApp application("Gauge","FoxTest");

  // Open display
  application.init(argc,argv);

  // Main window
  GaugeWindow* window=new GaugeWindow(&application);

  // Create app
  application.create();

  // Run
  return application.run();
  }
