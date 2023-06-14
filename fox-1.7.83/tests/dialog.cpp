/********************************************************************************
*                                                                               *
*                                 Test Dialog Box                               *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*******************************************************************************/

class FXTestDialog : public FXDialogBox {
  FXDECLARE(FXTestDialog)
protected:
  FXHorizontalFrame* contents;
  FXHorizontalFrame* buttons;
  FXMenuPane*        menu;
  FXMenuPane*        submenu;
  FXPopup*           pane;
private:
  FXTestDialog(){}
public:
  FXTestDialog(FXWindow* own);
  virtual ~FXTestDialog();
  };


/*******************************************************************************/


// Mini application object
class DialogTester : public FXMainWindow {
  FXDECLARE(DialogTester)
protected:

  // Member data
  FXMenuBar         *menubar;
  FXMenuPane        *filemenu;
  FXHorizontalFrame *contents;
  FXTestDialog      *dialog;

protected:
  DialogTester(){}

public:

  // Message handlers
  long onCmdShowDialog(FXObject*,FXSelector,void*);
  long onCmdShowDialogModal(FXObject*,FXSelector,void*);

public:

  // Messages
  enum {
    ID_SHOWDIALOG=FXMainWindow::ID_LAST,
    ID_SHOWDIALOGMODAL
    };

public:
  DialogTester(FXApp *app);
  virtual void create();
  virtual ~DialogTester();
  };


/*******************************************************************************/


// FXTestDialog implementation
FXIMPLEMENT(FXTestDialog,FXDialogBox,nullptr,0)


// Construct a dialog box
FXTestDialog::FXTestDialog(FXWindow* own):FXDialogBox(own,"Test of Dialog Box",DECOR_TITLE|DECOR_BORDER){

  // Bottom buttons
  buttons=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|FRAME_NONE|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0,40,40,20,20);

  // Separator
  new FXHorizontalSeparator(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|SEPARATOR_GROOVE);

  // Contents
  contents=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y|PACK_UNIFORM_WIDTH);

  submenu=new FXMenuPane(this);
  new FXMenuCommand(submenu,"One");
  new FXMenuCommand(submenu,"Two");
  new FXMenuCommand(submenu,"Three");

  // Menu
  menu=new FXMenuPane(this);
  new FXMenuCommand(menu,"&Accept",nullptr,this,ID_ACCEPT);
  new FXMenuCommand(menu,"&Cancel",nullptr,this,ID_CANCEL);
  new FXMenuCascade(menu,"Submenu",nullptr,submenu);
  new FXMenuCommand(menu,"&Quit\tCtl-Q",nullptr,getApp(),FXApp::ID_QUIT);

  // Popup menu
  pane=new FXPopup(this);
  new FXOption(pane,"One",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Two",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Three",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Four",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Five",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Six",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Seven",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Eight",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Nine",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);
  new FXOption(pane,"Ten",nullptr,nullptr,0,JUSTIFY_HZ_APART|ICON_AFTER_TEXT);

  FXComboBox* combobox=new FXComboBox(contents,10,nullptr,0,COMBOBOX_STATIC|FRAME_SUNKEN|FRAME_THICK|LAYOUT_SIDE_TOP);
  combobox->setNumVisible(4);
  combobox->appendItem("One");
  combobox->appendItem("Two");
  combobox->appendItem("Three");
  combobox->appendItem("Four");

  // Option menu
  new FXOptionMenu(contents,pane,FRAME_RAISED|FRAME_THICK|JUSTIFY_HZ_APART|ICON_AFTER_TEXT|LAYOUT_CENTER_X|LAYOUT_CENTER_Y);

  // Button to pop menu
  new FXMenuButton(contents,"&Menu",nullptr,menu,MENUBUTTON_DOWN|JUSTIFY_LEFT|LAYOUT_TOP|FRAME_RAISED|FRAME_THICK|ICON_AFTER_TEXT|LAYOUT_CENTER_X|LAYOUT_CENTER_Y);

  // Accept
 new FXButton(buttons,"&Accept",nullptr,this,ID_ACCEPT,BUTTON_DEFAULT|BUTTON_INITIAL|FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT|LAYOUT_CENTER_Y);

  // Cancel
  new FXButton(buttons,"&Cancel",nullptr,this,ID_CANCEL,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  }


// Must delete the menus
FXTestDialog::~FXTestDialog(){
  delete menu;
  delete submenu;
  delete pane;
  }

/*******************************************************************************/

// Map
FXDEFMAP(DialogTester) DialogTesterMap[]={
  FXMAPFUNC(SEL_COMMAND,  DialogTester::ID_SHOWDIALOG,      DialogTester::onCmdShowDialog),
  FXMAPFUNC(SEL_COMMAND,  DialogTester::ID_SHOWDIALOGMODAL, DialogTester::onCmdShowDialogModal),
  };


// FXDialogBoxApp implementation
FXIMPLEMENT(DialogTester,FXMainWindow,DialogTesterMap,ARRAYNUMBER(DialogTesterMap))



/*******************************************************************************/


// Make some windows
DialogTester::DialogTester(FXApp* a):FXMainWindow(a,"Group Box Test",nullptr,nullptr,DECOR_ALL,0,0,400,200){

  // Tooltip
  new FXToolTip(getApp());

  // Menubar
  menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);

  // File Menu
  filemenu=new FXMenuPane(this);
  new FXMenuCommand(filemenu,"&Quit\tCtl-Q",nullptr,getApp(),FXApp::ID_QUIT,0);
  new FXMenuTitle(menubar,"&File",nullptr,filemenu);

  // Separator
  new FXHorizontalSeparator(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|SEPARATOR_GROOVE);

  // Contents
  contents=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|FRAME_NONE|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH);

  // Button to pop normal dialog
  new FXButton(contents,"&Non-Modal Dialog...\tDisplay normal dialog",nullptr,this,ID_SHOWDIALOG,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_X|LAYOUT_CENTER_Y);

  // Button to pop modal dialog
  new FXButton(contents,"&Modal Dialog...\tDisplay modal dialog",nullptr,this,ID_SHOWDIALOGMODAL,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_X|LAYOUT_CENTER_Y);

  // Build a dialog box
  dialog=new FXTestDialog(this);

  // Separator
  new FXHorizontalSeparator(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|SEPARATOR_GROOVE);

  }


// Clean up
DialogTester::~DialogTester(){
  delete filemenu;
  }


// Open
long DialogTester::onCmdShowDialog(FXObject*,FXSelector,void*){
  dialog->show(PLACEMENT_OWNER);
  return 1;
  }


// Option
long DialogTester::onCmdShowDialogModal(FXObject*,FXSelector,void*){
  FXTestDialog modaldialog(this);
  modaldialog.execute(PLACEMENT_OWNER);
  return 1;
  }


// Start
void DialogTester::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
  }


/*******************************************************************************/


// Start the whole thing
int main(int argc,char *argv[]){

  // Make application
  FXApp  application("Dialog","FoxTest");

  // Open display
  application.init(argc,argv);

  new DialogTester(&application);

  // Create app
  application.create();

  // Run
  return application.run();
  }


