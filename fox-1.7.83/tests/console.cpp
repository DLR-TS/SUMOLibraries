/********************************************************************************
*                                                                               *
*                                 Test Console Widget                           *
*                                                                               *
*********************************************************************************
* Copyright (C) 2006,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>




/*******************************************************************************/


// Console Window
class ConsoleWindow : public FXMainWindow {
  FXDECLARE(ConsoleWindow)
protected:
  FXToolTip       *tooltip;
  FXMenuBar       *menubar;
  FXMenuPane      *filemenu;
  FXMenuPane      *helpmenu;
  FXVerticalFrame *contents;
  FXVerticalFrame *frame;
  FXConsole       *console;
protected:
  ConsoleWindow(){}
public:
  long onCmdAbout(FXObject*,FXSelector,void*);
public:
  enum{
    ID_ABOUT=FXMainWindow::ID_LAST,
    ID_CONSOLE,
    ID_LAST
    };
public:
  ConsoleWindow(FXApp* a);
  virtual void create();
  virtual ~ConsoleWindow();
  };


/*******************************************************************************/

// Map
FXDEFMAP(ConsoleWindow) ConsoleWindowMap[]={
  FXMAPFUNC(SEL_COMMAND,ConsoleWindow::ID_ABOUT,ConsoleWindow::onCmdAbout),
  };


// Object implementation
FXIMPLEMENT(ConsoleWindow,FXMainWindow,ConsoleWindowMap,ARRAYNUMBER(ConsoleWindowMap))


/*******************************************************************************/

// Make some windows
ConsoleWindow::ConsoleWindow(FXApp* a):FXMainWindow(a,"Console",nullptr,nullptr,DECOR_ALL,0,0,800,800){

  // Menubar
  menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);

  // Contents
  contents=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  // Console frame
  frame=new FXVerticalFrame(contents,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);

  // Console widget
  console=new FXConsole(frame,this,ID_CONSOLE,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 2,2,2,2);

  // File Menu
  filemenu=new FXMenuPane(this);
  new FXMenuCommand(filemenu,"&Quit\tCtl-Q",nullptr,getApp(),FXApp::ID_QUIT);
  new FXMenuTitle(menubar,"&File",nullptr,filemenu);

  helpmenu=new FXMenuPane(this);
  new FXMenuCommand(helpmenu,"About...",nullptr,this,ID_ABOUT);
  new FXMenuTitle(menubar,"&Help",nullptr,helpmenu,LAYOUT_RIGHT);

  // Tooltip
  tooltip=new FXToolTip(getApp());
  }


// Start
void ConsoleWindow::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
  }


// About
long ConsoleWindow::onCmdAbout(FXObject*,FXSelector,void*){
  FXMessageBox::information(this,MBOX_OK,"About Console","Console Widget.\nCopyright (C) 2006 Jeroen van der Zijp");
  return 1;
  }


// Delete it
ConsoleWindow::~ConsoleWindow(){
  delete filemenu;
  delete helpmenu;
  }


/*******************************************************************************/


// Start the whole thing
int main(int argc,char *argv[]){

  // Make application
  FXApp application("Console");

  // Open display
  application.init(argc,argv);

  // Make window
  new ConsoleWindow(&application);

  // Create app
  application.create();

  // Run
  return application.run();
  }


