/********************************************************************************
*                                                                               *
*                                 Test Table Widget                             *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>




/*******************************************************************************/


// Table Window
class TableWindow : public FXMainWindow {
  FXDECLARE(TableWindow)
protected:

  // Member data
  FXToolTip*         tooltip;
  FXMenuBar*         menubar;
  FXMenuPane*        filemenu;
  FXMenuPane*        tablemenu;
  FXMenuPane*        manipmenu;
  FXMenuPane*        selectmenu;
  FXVerticalFrame*   contents;
  FXVerticalFrame*   frame;
  FXTable*           table;
  FXBMPIcon*         penguinicon;

protected:
  TableWindow(){}

public:
  long onCmdTest(FXObject*,FXSelector,void*);
  long onCmdResizeTable(FXObject*,FXSelector,void*);
  long onTableSelected(FXObject*,FXSelector,void*);
  long onTableDeselected(FXObject*,FXSelector,void*);
  long onTableInserted(FXObject*,FXSelector,void*);
  long onTableDeleted(FXObject*,FXSelector,void*);
  long onTableChanged(FXObject*,FXSelector,void*);
  long onTableReplaced(FXObject*,FXSelector,void*);

public:
  enum{
    ID_TEST=FXMainWindow::ID_LAST,
    ID_RESIZETABLE,
    ID_TABLE,
    ID_LAST
    };

public:
  TableWindow(FXApp* a);
  virtual void create();
  virtual ~TableWindow();
  };


/*******************************************************************************/

// Map
FXDEFMAP(TableWindow) TableWindowMap[]={
  FXMAPFUNC(SEL_COMMAND,TableWindow::ID_TEST,TableWindow::onCmdTest),
  FXMAPFUNC(SEL_COMMAND,TableWindow::ID_RESIZETABLE,TableWindow::onCmdResizeTable),
  FXMAPFUNC(SEL_SELECTED,TableWindow::ID_TABLE,TableWindow::onTableSelected),
  FXMAPFUNC(SEL_DESELECTED,TableWindow::ID_TABLE,TableWindow::onTableDeselected),
  FXMAPFUNC(SEL_INSERTED,TableWindow::ID_TABLE,TableWindow::onTableInserted),
  FXMAPFUNC(SEL_DELETED,TableWindow::ID_TABLE,TableWindow::onTableDeleted),
  FXMAPFUNC(SEL_CHANGED,TableWindow::ID_TABLE,TableWindow::onTableChanged),
  FXMAPFUNC(SEL_REPLACED,TableWindow::ID_TABLE,TableWindow::onTableReplaced),
  };


// Object implementation
FXIMPLEMENT(TableWindow,FXMainWindow,TableWindowMap,ARRAYNUMBER(TableWindowMap))


/*******************************************************************************/

extern const unsigned char penguin[]={
  0x42,0x4d,0xe2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x00,0x00,0x00,0x28,0x00,
  0x00,0x00,0x10,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x01,0x00,0x04,0x00,0x00,0x00,
  0x00,0x00,0x90,0x00,0x00,0x00,0x6d,0x0b,0x00,0x00,0x6d,0x0b,0x00,0x00,0x07,0x00,
  0x00,0x00,0x07,0x00,0x00,0x00,0xdc,0xc0,0xb2,0x00,0x80,0x80,0x80,0x00,0x00,0x00,
  0x00,0x00,0xc0,0xc0,0xc0,0x00,0x10,0x10,0x10,0x00,0xff,0xff,0xff,0x00,0x08,0xa0,
  0xe0,0x00,0x00,0x66,0x61,0x22,0x22,0x21,0x66,0x00,0x06,0x66,0x66,0x55,0x22,0x26,
  0x66,0x60,0x66,0x66,0x65,0x55,0x55,0x26,0x66,0x66,0x66,0x66,0x65,0x55,0x55,0x36,
  0x66,0x66,0x06,0x66,0x55,0x55,0x55,0x56,0x22,0x60,0x00,0x63,0x55,0x35,0x55,0x55,
  0x22,0x20,0x00,0x23,0x55,0x35,0x55,0x55,0x22,0x20,0x00,0x02,0x55,0x33,0x55,0x55,
  0x22,0x10,0x00,0x02,0x55,0x53,0x55,0x55,0x22,0x00,0x00,0x01,0x25,0x55,0x55,0x51,
  0x21,0x00,0x00,0x00,0x21,0x55,0x55,0x52,0x10,0x00,0x00,0x00,0x02,0x66,0x55,0x22,
  0x00,0x00,0x00,0x00,0x02,0x66,0x63,0x21,0x00,0x00,0x00,0x00,0x02,0x66,0x63,0x20,
  0x00,0x00,0x00,0x00,0x02,0x42,0x25,0x20,0x00,0x00,0x00,0x00,0x02,0x32,0x32,0x20,
  0x00,0x00,0x00,0x00,0x01,0x22,0x22,0x10,0x00,0x00,0x00,0x00,0x00,0x12,0x21,0x00,
  0x00,0x00
  };


// Make some windows
TableWindow::TableWindow(FXApp* a):FXMainWindow(a,"Table Widget Test",nullptr,nullptr,DECOR_ALL,0,0,0,0){
  static const FXchar *months[]={"January","February","March","April","May","June","July","August","September","October","November","December"};
  FXint r,c;

  // Tooltip
  tooltip=new FXToolTip(getApp());

  penguinicon=new FXBMPIcon(getApp(),penguin,0,IMAGE_ALPHAGUESS);
  //penguinicon=new FXBMPIcon(getApp(),penguin,0,IMAGE_OPAQUE);

  // Menubar
  menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);

  new FXStatusBar(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|STATUSBAR_WITH_DRAGCORNER);

  // Separator
  new FXHorizontalSeparator(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|SEPARATOR_GROOVE);

  // Contents
  contents=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y);

  frame=new FXVerticalFrame(contents,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);

  // Table
  table=new FXTable(frame,this,ID_TABLE,TABLE_COL_SIZABLE|TABLE_ROW_SIZABLE|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 2,2,2,2);
  table->setVisibleRows(20);
  table->setVisibleColumns(8);
//  table->setRowHeaderMode(0);
  table->setTableSize(50,14);
  table->setBackColor(FXRGB(255,255,255));
  table->setCellColor(0,0,FXColors::WhiteSmoke);
  table->setCellColor(0,1,FXColors::WhiteSmoke);
  table->setCellColor(1,0,FXColors::MistyRose2);
  table->setCellColor(1,1,FXColors::MistyRose2);
  table->setHelpText("Editable table.");

  table->setRowRenumbering(FXHeader::decimalNumbering);
  table->setColumnRenumbering(FXHeader::alphaNumbering);

  // Initialize scrollable part of table
  for(r=0; r<50; r++){
    for(c=0; c<14; c++){
      table->setItemText(r,c,"r:"+FXString::value(r)+" c:"+FXString::value(c));
      }
    }

  // Initialize column headers
  for(c=0; c<12; c++){
    table->setColumnText(c,months[c]);
    }

  // Initialize row headers
  for(r=0; r<50; r++){
    table->setRowText(r,"Row"+FXString::value(r));
    }

/*
  table->setLeadingRows(1);
  table->setLeadingColumns(1);
  table->setTrailingRows(1);
  table->setTrailingColumns(1);
  // Init corners
  table->setItemText(0,0,FXString::null);
  table->setItemText(0,13,FXString::null);
  table->setItemText(49,0,FXString::null);
  table->setItemText(49,13,FXString::null);

  // Initialize first/last fixed rows
  for(c=1; c<=12; c++){
    table->setItemText(0,c,months[c-1]);
    table->setItemText(49,c,months[c-1]);
    table->setItemButton(0,c,true);
    table->setItemJustify(0,c,FXTableItem::LEFT|FXTableItem::CENTER_Y);
    }
  table->setItemButton(0,0,true);
  table->setItemButton(0,13,true);

  // Initialize first/last fixed columns
  for(r=1; r<=48; r++){
    table->setItemText(r,0,FXStringVal(r));
    table->setItemText(r,13,FXStringVal(r));
    table->setItemButton(r,0,true);
    }
  table->setItemButton(49,0,true);
*/
  table->setItemText(10,10,"This is multi-\nline text");
//  table->setRowHeight(10,35);
  table->setItemJustify(10,10,FXTableItem::CENTER_X|FXTableItem::CENTER_Y);

  table->setItem(3,3,nullptr);
  table->setItem(5,6,table->getItem(5,5));      // FIXME this is not the right way to make spanning cells
  table->setItem(5,7,table->getItem(5,5));
  table->setItemText(5,5,"Spanning Item");
  table->setItemJustify(5,5,FXTableItem::CENTER_X|FXTableItem::CENTER_Y);

  table->setItemBorders(9,9,FXTableItem::TBORDER|FXTableItem::LBORDER|FXTableItem::BBORDER);
  table->setItemBorders(9,10,FXTableItem::TBORDER|FXTableItem::RBORDER|FXTableItem::BBORDER);

  table->setItemJustify(10,5,FXTableItem::LEFT|FXTableItem::CENTER_Y);

  table->setItemBorders(40,13,FXTableItem::LBORDER|FXTableItem::TBORDER|FXTableItem::RBORDER|FXTableItem::BBORDER);
  table->setItemBorders(49,13,FXTableItem::LBORDER|FXTableItem::TBORDER|FXTableItem::RBORDER|FXTableItem::BBORDER);
  table->setItemBorders(5,0,FXTableItem::LBORDER|FXTableItem::TBORDER|FXTableItem::RBORDER|FXTableItem::BBORDER);

  table->setItemIcon(6,6,penguinicon);
  table->setItemIconPosition(6,6,FXTableItem::ABOVE);
  table->setItemJustify(6,6,FXTableItem::CENTER_X|FXTableItem::CENTER_Y);

  table->setItemStipple(3,4,STIPPLE_CROSSDIAG);

  table->setItem(10,2,new FXComboTableItem("One\nTwo\nThree\nFour"));

  table->setItemText(10,3,"Disabled");
  table->disableItem(10,3);

  // File Menu
  filemenu=new FXMenuPane(this);
  new FXMenuCommand(filemenu,"&Quit\tCtl-Q",nullptr,getApp(),FXApp::ID_QUIT);
  new FXMenuTitle(menubar,"&File",nullptr,filemenu);

  // Tab side
  tablemenu=new FXMenuPane(this);
  new FXMenuCheck(tablemenu,"Horizontal grid",table,FXTable::ID_HORZ_GRID);
  new FXMenuCheck(tablemenu,"Vertical grid",table,FXTable::ID_VERT_GRID);
  new FXMenuCheck(tablemenu,"Editable",table,FXTable::ID_TOGGLE_EDITABLE);
  new FXMenuTitle(menubar,"&Options",nullptr,tablemenu);

  manipmenu=new FXMenuPane(this);
  new FXMenuCommand(manipmenu,"Edit Cell",nullptr,table,FXTable::ID_START_INPUT);
  new FXMenuCommand(manipmenu,"Delete Column",nullptr,table,FXTable::ID_DELETE_COLUMN);
  new FXMenuCommand(manipmenu,"Delete Row",nullptr,table,FXTable::ID_DELETE_ROW);
  new FXMenuCommand(manipmenu,"Insert Column",nullptr,table,FXTable::ID_INSERT_COLUMN);
  new FXMenuCommand(manipmenu,"Insert Row",nullptr,table,FXTable::ID_INSERT_ROW);
  new FXMenuCommand(manipmenu,"Resize table...",nullptr,this,TableWindow::ID_RESIZETABLE);
  new FXMenuCommand(manipmenu,"Test",nullptr,this,TableWindow::ID_TEST);
  new FXMenuTitle(menubar,"&Manipulations",nullptr,manipmenu);

  selectmenu=new FXMenuPane(this);
  new FXMenuCommand(selectmenu,"Select All",nullptr,table,FXTable::ID_SELECT_ALL);
  new FXMenuCommand(selectmenu,"Select Cell",nullptr,table,FXTable::ID_SELECT_CELL);
  new FXMenuCommand(selectmenu,"Select Row",nullptr,table,FXTable::ID_SELECT_ROW);
  new FXMenuCommand(selectmenu,"Select Column",nullptr,table,FXTable::ID_SELECT_COLUMN);
  new FXMenuCommand(selectmenu,"Deselect All",nullptr,table,FXTable::ID_DESELECT_ALL);
  new FXMenuCommand(selectmenu,"Cut to clipboard",nullptr,table,FXTable::ID_CUT_SEL);
  new FXMenuCommand(selectmenu,"Copy to clipboard",nullptr,table,FXTable::ID_COPY_SEL);
  new FXMenuCommand(selectmenu,"Paste from clipboard",nullptr,table,FXTable::ID_PASTE_SEL);
  new FXMenuCommand(selectmenu,"Delete",nullptr,table,FXTable::ID_DELETE_SEL);
  new FXMenuTitle(menubar,"&Selection",nullptr,selectmenu);
  }


TableWindow::~TableWindow(){
  delete filemenu;
  delete tablemenu;
  delete manipmenu;
  delete selectmenu;
  delete penguinicon;
  }


// Test
long TableWindow::onCmdTest(FXObject*,FXSelector,void*){
  FXint cr=table->getCurrentRow();
  FXint cc=table->getCurrentColumn();
  table->setSpanningRange(cr,cc,Math::imax(cr-1,0),Math::imin(cr+1,table->getNumRows()),Math::imax(cc-1,0),Math::imin(cc+1,table->getNumColumns()-1));
  return 1;
  }


// Resize table
long TableWindow::onCmdResizeTable(FXObject*,FXSelector,void*){
  FXDialogBox dlg(this,"Resize Table");
  FXint nr,nc,r,c;
  FXHorizontalFrame *hframe=new FXHorizontalFrame(&dlg,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(hframe,"Rows:",nullptr,LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y);
  FXTextField* rows=new FXTextField(hframe,5,nullptr,0,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK|LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y);
  new FXLabel(hframe,"Columns:",nullptr,LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y);
  FXTextField* cols=new FXTextField(hframe,5,nullptr,0,JUSTIFY_RIGHT|FRAME_SUNKEN|FRAME_THICK|LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y);
  new FXButton(hframe,"Cancel",nullptr,&dlg,FXDialogBox::ID_CANCEL,FRAME_RAISED|FRAME_THICK|LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y);
  new FXButton(hframe,"  OK  ",nullptr,&dlg,FXDialogBox::ID_ACCEPT,FRAME_RAISED|FRAME_THICK|LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y);
  FXint oldnr,oldnc;
  oldnr=table->getNumRows();
  oldnc=table->getNumColumns();
  rows->setText(FXString::value(oldnr));
  cols->setText(FXString::value(oldnc));
  if(dlg.execute()){
    nr=rows->getText().toInt();
    nc=cols->getText().toInt();
    if(nr<0) nr=0;
    if(nc<0) nc=0;
    table->setTableSize(nr,nc);
    for(r=0; r<nr; r++){
      for(c=0; c<nc; c++){
        table->setItemText(r,c,"r:"+FXString::value(r+1)+" c:"+FXString::value(c+1));
        }
      }
    }
  return 1;
  }

// Selected
long TableWindow::onTableSelected(FXObject*,FXSelector,void* ptr){
  FXTablePos *tp=(FXTablePos*)ptr;
  FXTRACE((10,"SEL_SELECTED row=%d, col=%d\n",tp->row,tp->col));
  return 1;
  }

// Deselected
long TableWindow::onTableDeselected(FXObject*,FXSelector,void* ptr){
  FXTablePos *tp=(FXTablePos*)ptr;
  FXTRACE((10,"SEL_DESELECTED row=%d, col=%d\n",tp->row,tp->col));
  return 1;
  }

// Inserted
long TableWindow::onTableInserted(FXObject*,FXSelector,void* ptr){
  FXTableRange *range=(FXTableRange*)ptr;
  FXTRACE((10,"SEL_INSERTED fm.row=%d, fm.col=%d to.row=%d, to.col=%d\n",range->fm.row,range->fm.col,range->to.row,range->to.col));
  return 1;
  }

// Deleted
long TableWindow::onTableDeleted(FXObject*,FXSelector,void* ptr){
  FXTableRange *range=(FXTableRange*)ptr;
  FXTRACE((10,"SEL_DELETED fm.row=%d, fm.col=%d to.row=%d, to.col=%d\n",range->fm.row,range->fm.col,range->to.row,range->to.col));
  return 1;
  }

// Changed
long TableWindow::onTableChanged(FXObject*,FXSelector,void* ptr){
  FXTablePos *tp=(FXTablePos*)ptr;
  FXTRACE((10,"SEL_CHANGED row=%d, col=%d\n",tp->row,tp->col));
  return 1;
  }


// Replaced
long TableWindow::onTableReplaced(FXObject*,FXSelector,void* ptr){
  FXTableRange *range=(FXTableRange*)ptr;
  FXTRACE((10,"SEL_REPLACED fm.row=%d, fm.col=%d to.row=%d, to.col=%d\n",range->fm.row,range->fm.col,range->to.row,range->to.col));
  return 1;
  }


// Start
void TableWindow::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
  }


/*******************************************************************************/


// Start the whole thing
int main(int argc,char *argv[]){

  // Make application
  FXApp application("TableApp","FoxTest");

  // Open display
  application.init(argc,argv);

  // Make window
  new TableWindow(&application);

  // Create app
  application.create();

  // Run
  return application.run();
  }


