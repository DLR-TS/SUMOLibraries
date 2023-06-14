/********************************************************************************
*                                                                               *
*                        F o n t   S e l e c t i o n   B o x                    *
*                                                                               *
*********************************************************************************
* Copyright (C) 1999,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#include "xincs.h"
#include "fxver.h"
#include "fxdefs.h"
#include "fxmath.h"
#include "FXArray.h"
#include "FXHash.h"
#include "FXMutex.h"
#include "FXElement.h"
#include "FXStream.h"
#include "FXString.h"
#include "FXSize.h"
#include "FXPoint.h"
#include "FXRectangle.h"
#include "FXStringDictionary.h"
#include "FXSettings.h"
#include "FXRegistry.h"
#include "FXObjectList.h"
#include "FXRegistry.h"
#include "FXAccelTable.h"
#include "FXFont.h"
#include "FXEvent.h"
#include "FXWindow.h"
#include "FXApp.h"
#include "FXImage.h"
#include "FXIcon.h"
#include "FXGIFIcon.h"
#include "FXWindow.h"
#include "FXFrame.h"
#include "FXLabel.h"
#include "FXTextField.h"
#include "FXButton.h"
#include "FXCheckButton.h"
#include "FXMenuButton.h"
#include "FXComposite.h"
#include "FXPacker.h"
#include "FXVerticalFrame.h"
#include "FXHorizontalFrame.h"
#include "FXMatrix.h"
#include "FXCanvas.h"
#include "FXShell.h"
#include "FXScrollBar.h"
#include "FXScrollArea.h"
#include "FXScrollWindow.h"
#include "FXList.h"
#include "FXComboBox.h"
#include "FXFontSelector.h"


/*
  Notes:

  - Mega-widget to select a font from the system.
*/

using namespace FX;

/*******************************************************************************/

namespace FX {

// Map
FXDEFMAP(FXFontSelector) FXFontSelectorMap[]={
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_FAMILY,FXFontSelector::onCmdFamily),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_WEIGHT,FXFontSelector::onCmdWeight),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_SIZE,FXFontSelector::onCmdSize),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_SIZE_TEXT,FXFontSelector::onCmdSizeText),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_STYLE,FXFontSelector::onCmdStyle),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_STYLE_TEXT,FXFontSelector::onCmdStyleText),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_CHARSET,FXFontSelector::onCmdCharset),
  FXMAPFUNC(SEL_UPDATE,FXFontSelector::ID_CHARSET,FXFontSelector::onUpdCharset),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_SETWIDTH,FXFontSelector::onCmdSetWidth),
  FXMAPFUNC(SEL_UPDATE,FXFontSelector::ID_SETWIDTH,FXFontSelector::onUpdSetWidth),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_PITCH,FXFontSelector::onCmdPitch),
  FXMAPFUNC(SEL_UPDATE,FXFontSelector::ID_PITCH,FXFontSelector::onUpdPitch),
  FXMAPFUNC(SEL_UPDATE,FXFontSelector::ID_SCALABLE,FXFontSelector::onUpdScalable),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_SCALABLE,FXFontSelector::onCmdScalable),
  FXMAPFUNC(SEL_UPDATE,FXFontSelector::ID_ALLFONTS,FXFontSelector::onUpdAllFonts),
  FXMAPFUNC(SEL_COMMAND,FXFontSelector::ID_ALLFONTS,FXFontSelector::onCmdAllFonts),
  };


// Implementation
FXIMPLEMENT(FXFontSelector,FXPacker,FXFontSelectorMap,ARRAYNUMBER(FXFontSelectorMap))


/*******************************************************************************/


// Separator item
FXFontSelector::FXFontSelector(FXComposite *p,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h):FXPacker(p,opts,x,y,w,h){
  target=tgt;
  message=sel;

  // Contents
  FXVerticalFrame *innards=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);

  // Font selection boxes
  FXMatrix *controls=new FXMatrix(innards,3,LAYOUT_TOP|LAYOUT_FILL_X,0,0,0,0,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,0);

  // Font families, to be filled later
  new FXLabel(controls,tr("&Family:"),nullptr,JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  family=new FXTextField(controls,10,nullptr,0,TEXTFIELD_READONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  FXHorizontalFrame *familyframe=new FXHorizontalFrame(controls,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW,0,0,0,0, 0,0,0,0);
  familylist=new FXList(familyframe,this,ID_FAMILY,LIST_BROWSESELECT|LAYOUT_FILL_Y|LAYOUT_FILL_X|HSCROLLER_NEVER|VSCROLLER_ALWAYS);
  familylist->setNumVisible(8);

  // Initial focus on list
  familylist->setFocus();

  // Font weights
  new FXLabel(controls,tr("&Weight:"),nullptr,JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  weight=new FXTextField(controls,4,nullptr,0,TEXTFIELD_READONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  FXHorizontalFrame *weightframe=new FXHorizontalFrame(controls,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,0,0, 0,0,0,0);
  weightlist=new FXList(weightframe,this,ID_WEIGHT,LIST_BROWSESELECT|LAYOUT_FILL_Y|LAYOUT_FILL_X|HSCROLLER_NEVER|VSCROLLER_ALWAYS);
  weightlist->setNumVisible(8);

  // Font styles
  new FXLabel(controls,tr("&Style:"),nullptr,JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  style=new FXTextField(controls,6,nullptr,0,TEXTFIELD_READONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  FXHorizontalFrame *styleframe=new FXHorizontalFrame(controls,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,0,0, 0,0,0,0);
  stylelist=new FXList(styleframe,this,ID_STYLE,LIST_BROWSESELECT|LAYOUT_FILL_Y|LAYOUT_FILL_X|HSCROLLER_NEVER|VSCROLLER_ALWAYS);
  stylelist->setNumVisible(8);

  // Font sizes, to be filled later
  new FXLabel(controls,tr("Si&ze:"),nullptr,JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  size=new FXTextField(controls,2,this,ID_SIZE_TEXT,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  FXHorizontalFrame *sizeframe=new FXHorizontalFrame(controls,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN,0,0,0,0, 0,0,0,0);
  sizelist=new FXList(sizeframe,this,ID_SIZE,LIST_BROWSESELECT|LAYOUT_FILL_Y|LAYOUT_FILL_X|HSCROLLER_NEVER|VSCROLLER_ALWAYS);
  sizelist->setNumVisible(8);

  // Font filter boxes
  FXMatrix *attributes=new FXMatrix(innards,2,LAYOUT_TOP|LAYOUT_FILL_X,0,0,0,0,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,0);

  // Character set choice
  new FXLabel(attributes,tr("Character Set:"),nullptr,JUSTIFY_LEFT|JUSTIFY_CENTER_Y|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  charset=new FXComboBox(attributes,8,this,ID_CHARSET,COMBOBOX_STATIC|FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  charset->setNumVisible(10);
  charset->appendItem(tr("Any"),(void*)0);
  charset->appendItem(tr("West European"),(void*)FONTENCODING_WESTEUROPE);
  charset->appendItem(tr("East European"),(void*)FONTENCODING_EASTEUROPE);
  charset->appendItem(tr("South European"),(void*)FONTENCODING_SOUTHEUROPE);
  charset->appendItem(tr("North European"),(void*)FONTENCODING_NORTHEUROPE);
  charset->appendItem(tr("Cyrillic"),(void*)FONTENCODING_CYRILLIC);
  charset->appendItem(tr("Arabic"),(void*)FONTENCODING_ARABIC);
  charset->appendItem(tr("Greek"),(void*)FONTENCODING_GREEK);
  charset->appendItem(tr("Hebrew"),(void*)FONTENCODING_HEBREW);
  charset->appendItem(tr("Turkish"),(void*)FONTENCODING_TURKISH);
  charset->appendItem(tr("Nordic"),(void*)FONTENCODING_NORDIC);
  charset->appendItem(tr("Thai"),(void*)FONTENCODING_THAI);
  charset->appendItem(tr("Baltic"),(void*)FONTENCODING_BALTIC);
  charset->appendItem(tr("Celtic"),(void*)FONTENCODING_CELTIC);
  charset->appendItem(tr("Russian"),(void*)FONTENCODING_KOI8);
  charset->appendItem(tr("Central European (cp1250)"),(void*)FONTENCODING_CP1250);
  charset->appendItem(tr("Russian (cp1251)"),(void*)FONTENCODING_CP1251);
  charset->appendItem(tr("Latin1 (cp1252)"),(void*)FONTENCODING_CP1252);
  charset->appendItem(tr("Greek (cp1253)"),(void*)FONTENCODING_CP1253);
  charset->appendItem(tr("Turkish (cp1254)"),(void*)FONTENCODING_CP1254);
  charset->appendItem(tr("Hebrew (cp1255)"),(void*)FONTENCODING_CP1255);
  charset->appendItem(tr("Arabic (cp1256)"),(void*)FONTENCODING_CP1256);
  charset->appendItem(tr("Baltic (cp1257)"),(void*)FONTENCODING_CP1257);
  charset->appendItem(tr("Vietnam (cp1258)"),(void*)FONTENCODING_CP1258);
  charset->appendItem(tr("Thai (cp874)"),(void*)FONTENCODING_CP874);
  charset->appendItem(tr("UNICODE"),(void*)FONTENCODING_UNICODE);
  charset->setCurrentItem(0);

  // Set width
  new FXLabel(attributes,tr("Set Width:"),nullptr,JUSTIFY_LEFT|JUSTIFY_CENTER_Y|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  setwidth=new FXComboBox(attributes,9,this,ID_SETWIDTH,COMBOBOX_STATIC|FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  setwidth->setNumVisible(10);
  setwidth->appendItem(tr("Any"),(void*)0);
  setwidth->appendItem(tr("Ultra condensed"),(void*)FXFont::UltraCondensed);
  setwidth->appendItem(tr("Extra condensed"),(void*)FXFont::ExtraCondensed);
  setwidth->appendItem(tr("Condensed"),(void*)FXFont::Condensed);
  setwidth->appendItem(tr("Semi condensed"),(void*)FXFont::SemiCondensed);
  setwidth->appendItem(tr("Normal"),(void*)FXFont::NonExpanded);
  setwidth->appendItem(tr("Semi expanded"),(void*)FXFont::SemiExpanded);
  setwidth->appendItem(tr("Expanded"),(void*)FXFont::Expanded);
  setwidth->appendItem(tr("Extra expanded"),(void*)FXFont::ExtraExpanded);
  setwidth->appendItem(tr("Ultra expanded"),(void*)FXFont::UltraExpanded);
  setwidth->setCurrentItem(0);

  // Pitch
  new FXLabel(attributes,tr("Pitch:"),nullptr,JUSTIFY_LEFT|JUSTIFY_CENTER_Y|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  pitch=new FXComboBox(attributes,5,this,ID_PITCH,COMBOBOX_STATIC|FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  pitch->setNumVisible(5);
  pitch->appendItem(tr("Any"),(void*)0);
  pitch->appendItem(tr("Variable"),(void*)(FXuval)FXFont::Variable);
  pitch->appendItem(tr("Mono"),(void*)(FXuval)(FXFont::Mono|FXFont::Cell));
  pitch->appendItem(tr("Cell"),(void*)(FXuval)FXFont::Cell);
  pitch->appendItem(tr("Dual"),(void*)(FXuval)FXFont::Dual);
  pitch->setCurrentItem(0);

  // Check for scalable
  new FXFrame(attributes,FRAME_NONE|LAYOUT_FILL_X);
  scalable=new FXCheckButton(attributes,tr("Scalable:"),this,ID_SCALABLE,JUSTIFY_NORMAL|TEXT_BEFORE_ICON|LAYOUT_CENTER_Y|LAYOUT_FILL_X);

  // Check for all (X11) fonts
#if defined(WIN32)
  allfonts=nullptr;
#else
  new FXFrame(attributes,FRAME_NONE|LAYOUT_FILL_X);
  allfonts=new FXCheckButton(attributes,tr("All Fonts:"),this,ID_ALLFONTS,JUSTIFY_NORMAL|TEXT_BEFORE_ICON|LAYOUT_CENTER_Y|LAYOUT_FILL_X);
#endif

  // Preview box
  FXMatrix *bottom=new FXMatrix(innards,2,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,DEFAULT_SPACING,DEFAULT_SPACING-1,DEFAULT_SPACING,DEFAULT_SPACING,0,0);
  new FXLabel(bottom,tr("Preview:"),nullptr,JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
  FXHorizontalFrame *box=new FXHorizontalFrame(bottom,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW|FRAME_SUNKEN|FRAME_THICK,0,0,0,0,0,0,0,0,0,0);
  FXScrollWindow *scrollwindow=new FXScrollWindow(box,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  preview=new FXLabel(scrollwindow,tr("ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789"),nullptr,JUSTIFY_CENTER_X|JUSTIFY_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  preview->setBackColor(getApp()->getBackColor());
  new FXFrame(bottom,FRAME_NONE|LAYOUT_FIX_WIDTH|LAYOUT_FILL_Y,0,0,1,0);
  new FXFrame(bottom,FRAME_NONE|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_FILL_ROW,0,0,1,120);

  // Bottom side
  FXHorizontalFrame *buttons=new FXHorizontalFrame(innards,LAYOUT_BOTTOM|LAYOUT_FILL_X);
  accept=new FXButton(buttons,tr("&Accept"),nullptr,nullptr,0,BUTTON_INITIAL|BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT,0,0,0,0,20,20);
  cancel=new FXButton(buttons,tr("&Cancel"),nullptr,nullptr,0,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT,0,0,0,0,20,20);

  fxstrlcpy(selected.face,"helvetica",sizeof(selected.face));
  selected.size=90;
  selected.weight=FXFont::Bold;
  selected.slant=0;
  selected.encoding=FONTENCODING_USASCII;
  selected.setwidth=0;
  selected.flags=0;
  previewfont=nullptr;
  }


// List the fonts when created
void FXFontSelector::create(){
  FXPacker::create();
  listFontFaces();
  listWeights();
  listSlants();
  listFontSizes();
  }


// Fill the list with face names
void FXFontSelector::listFontFaces(){
  FXFontDesc *fonts;
  FXuint numfonts,f;
  FXint selindex=-1;
  familylist->clearItems();
  family->setText("");
  if(FXFont::listFonts(fonts,numfonts,FXString::null,0,0,selected.setwidth,selected.encoding,selected.flags)){
    FXASSERT(0<numfonts);
    for(f=0; f<numfonts; f++){
      familylist->appendItem(fonts[f].face,nullptr,(void*)(FXuval)fonts[f].flags);
      if(FXString::compare(selected.face,fonts[f].face)==0) selindex=f;
      }
    if(selindex==-1) selindex=0;
    if(0<familylist->getNumItems()){
      familylist->setCurrentItem(selindex);
      familylist->makeItemVisible(selindex);
      family->setText(familylist->getItemText(selindex));
      fxstrlcpy(selected.face,familylist->getItemText(selindex).text(),sizeof(selected.face));
      }
    freeElms(fonts);
    }
  }


// Fill the list with font weights
void FXFontSelector::listWeights(){
  FXFontDesc *fonts;
  FXuint numfonts,f,ww,lastww;
  const FXchar *wgt;
  FXint selindex=-1;
  weightlist->clearItems();
  weight->setText("");
  if(FXFont::listFonts(fonts,numfonts,selected.face,0,0,selected.setwidth,selected.encoding,selected.flags)){
    FXASSERT(0<numfonts);
    lastww=0;
    for(f=0; f<numfonts; f++){
      ww=fonts[f].weight;
      if(ww!=lastww){

        // Get text for the weight
        switch(ww){
          case FXFont::Thin: wgt="thin"; break;
          case FXFont::ExtraLight: wgt="extra light"; break;
          case FXFont::Light: wgt="light"; break;
          case FXFont::Normal: wgt="normal"; break;
          case FXFont::Medium: wgt="medium"; break;
          case FXFont::DemiBold: wgt="demibold"; break;
          case FXFont::Bold: wgt="bold"; break;
          case FXFont::ExtraBold: wgt="extra bold"; break;
          case FXFont::Black: wgt="black"; break;
          default: wgt="normal"; break;
          }

        // Add it
        weightlist->appendItem(tr(wgt),nullptr,(void*)(FXuval)ww);

        // Remember if this was the current selection
        if(selected.weight==ww) selindex=weightlist->getNumItems()-1;
        lastww=ww;
        }
      }
    if(selindex==-1) selindex=0;
    if(0<weightlist->getNumItems()){
      weightlist->setCurrentItem(selindex);
      weightlist->makeItemVisible(selindex);
      weight->setText(weightlist->getItemText(selindex));
      selected.weight=(FXuint)(FXuval)weightlist->getItemData(selindex);
      }
    freeElms(fonts);
    }
  }


// Fill the list with font slants
void FXFontSelector::listSlants(){
  FXFontDesc *fonts;
  FXuint numfonts,f,s,lasts;
  const FXchar *slt;
  FXint selindex=-1;
  stylelist->clearItems();
  style->setText("");
  if(FXFont::listFonts(fonts,numfonts,selected.face,selected.weight,0,selected.setwidth,selected.encoding,selected.flags)){
    FXASSERT(0<numfonts);
    lasts=0;
    for(f=0; f<numfonts; f++){
      s=fonts[f].slant;
      if(s!=lasts){

        // Get text for the weight
        switch(s){
          case FXFont::ReverseOblique: slt="reverse oblique"; break;
          case FXFont::ReverseItalic: slt="reverse italic"; break;
          case FXFont::Straight: slt="regular"; break;
          case FXFont::Italic: slt="italic"; break;
          case FXFont::Oblique: slt="oblique"; break;
          default: slt="normal"; break;
          }

        // Add it
        stylelist->appendItem(tr(slt),nullptr,(void*)(FXuval)s);

        // Remember if this was the current selection
        if(selected.slant == s) selindex=stylelist->getNumItems()-1;
        lasts=s;
        }
      }
    if(selindex==-1) selindex=0;
    if(0<stylelist->getNumItems()){
      stylelist->setCurrentItem(selindex);
      stylelist->makeItemVisible(selindex);
      style->setText(stylelist->getItemText(selindex));
      selected.slant=(FXuint)(FXuval)stylelist->getItemData(selindex);
      }
    freeElms(fonts);
    }
  }


// Fill the list with font sizes
void FXFontSelector::listFontSizes(){
  const FXuint sizeint[]={60,80,90,100,110,120,140,160,200,240,300,360,420,480,640};
  FXFontDesc *fonts;
  FXuint numfonts,f,s,lasts;
  FXint selindex=-1,selindex1=-1;
  sizelist->clearItems();
  size->setText("");
  FXString string;
  if(FXFont::listFonts(fonts,numfonts,selected.face,selected.weight,selected.slant,selected.setwidth,selected.encoding,selected.flags)){
    FXASSERT(0<numfonts);
    lasts=0;
    if(fonts[0].flags&FXFont::Scalable){
      for(f=0; f<ARRAYNUMBER(sizeint); f++){
        s=sizeint[f];
        string.format("%.1f",0.1*s);
        sizelist->appendItem(string,nullptr,(void*)(FXuval)s);
        if(selected.size == s) selindex=sizelist->getNumItems()-1;
        // selindex1 is an index for use if there is not an exact match of size,
        // and will normally be the size from this list just above the actual size
        // as requested.
        if(selected.size>lasts && selected.size<=s) selindex1=sizelist->getNumItems()-1;
        lasts=s;
        }
      // If the requested size was greater than 64.0 I will use 64.0 as the
      // default item in the list.
      if(selindex1<0) selindex1=sizelist->getNumItems()-1;
      }
    else{
      for(f=0; f<numfonts; f++){
        s=fonts[f].size;
        if(s!=lasts){
          string.format("%.1f",0.1*s);
          sizelist->appendItem(string,nullptr,(void*)(FXuval)s);
          if(selected.size == s) selindex=sizelist->getNumItems()-1;
          lasts=s;
          }
        }
      }
    if(selindex==-1){
      // For scalable fonts select the best match if there is not a perfect one
      if(selindex1!=-1) selindex=selindex1;
      else selindex=0;
      }
    if(0<sizelist->getNumItems()){
      sizelist->setCurrentItem(selindex);
      sizelist->makeItemVisible(selindex);
      if(fonts[0].flags&FXFont::Scalable){
        string.format("%.1f",0.1*selected.size);
        size->setText(string);
        }
      else{
        size->setText(sizelist->getItemText(selindex));
        selected.size=(FXuint)(FXuval)sizelist->getItemData(selindex);
        }
      }
    freeElms(fonts);
    }
  }


// Preview
void FXFontSelector::previewFont(){

  // Save old font
  FXFont *old=previewfont;

  // Get new font
  previewfont=new FXFont(getApp(),selected);

  // Realize new font
  previewfont->create();

  // Set new font
  preview->setFont(previewfont);

  // Delete old font
  delete old;
  }


// Selected font family
long FXFontSelector::onCmdFamily(FXObject*,FXSelector,void* ptr){
  fxstrlcpy(selected.face,familylist->getItemText((FXint)(FXival)ptr).text(),sizeof(selected.face));
  family->setText(selected.face);
  listWeights();
  listSlants();
  listFontSizes();
  previewFont();
  return 1;
  }


// Changed weight setting
long FXFontSelector::onCmdWeight(FXObject*,FXSelector,void* ptr){
  selected.weight=(FXuint)(FXuval)weightlist->getItemData((FXint)(FXival)ptr);
  weight->setText(weightlist->getItemText((FXint)(FXival)ptr));
  listSlants();
  listFontSizes();
  previewFont();
  return 1;
  }


// User clicked up directory button
long FXFontSelector::onCmdSize(FXObject*,FXSelector,void* ptr){
  selected.size=(FXuint)(FXuval)sizelist->getItemData((FXint)(FXival)ptr);
  size->setText(sizelist->getItemText((FXint)(FXival)ptr));
  previewFont();
  return 1;
  }


// User clicked up directory button
long FXFontSelector::onCmdSizeText(FXObject*,FXSelector,void*){
  selected.size=(FXuint)(10.0*size->getText().toDouble());
  if(selected.size<60) selected.size=60;
  if(selected.size>2400) selected.size=2400;
  previewFont();
  return 1;
  }


// User clicked up directory button
long FXFontSelector::onCmdStyle(FXObject*,FXSelector,void* ptr){
  selected.slant=(FXuint)(FXuval)stylelist->getItemData((FXint)(FXival)ptr);
  style->setText(stylelist->getItemText((FXint)(FXival)ptr));
  listFontSizes();
  previewFont();
  return 1;
  }


// Style type in
long FXFontSelector::onCmdStyleText(FXObject*,FXSelector,void*){
  return 1;
  }


// Character set
long FXFontSelector::onCmdCharset(FXObject*,FXSelector,void*){
  FXint index=charset->getCurrentItem();
  FXuint enc=(FXuint)(FXuval)charset->getItemData(index);
  selected.encoding=(FXFontEncoding)enc;
  listFontFaces();
  listWeights();
  listSlants();
  listFontSizes();
  previewFont();
  return 1;
  }


// Update character set
long FXFontSelector::onUpdCharset(FXObject*,FXSelector,void*){
  charset->setCurrentItem(charset->findItemByData((void*)(FXuval)selected.encoding));
  return 1;
  }


// Changed set width
long FXFontSelector::onCmdSetWidth(FXObject*,FXSelector,void*){
  FXint index=setwidth->getCurrentItem();
  selected.setwidth=(FXuint)(FXuval)setwidth->getItemData(index);
  listFontFaces();
  listWeights();
  listSlants();
  listFontSizes();
  previewFont();
  return 1;
  }


// Update set width
long FXFontSelector::onUpdSetWidth(FXObject*,FXSelector,void*){
  setwidth->setCurrentItem(setwidth->findItemByData((void*)(FXuval)selected.setwidth));
  return 1;
  }


// Changed pitch
long FXFontSelector::onCmdPitch(FXObject*,FXSelector,void*){
  FXint index=pitch->getCurrentItem();
  selected.flags&=~(FXFont::Mono|FXFont::Variable|FXFont::Dual|FXFont::Cell);
  selected.flags|=(FXuint)(FXuval)pitch->getItemData(index);
  listFontFaces();
  listWeights();
  listSlants();
  listFontSizes();
  previewFont();
  return 1;
  }


// Update pitch
long FXFontSelector::onUpdPitch(FXObject*,FXSelector,void*){
  FXint item=0;
  switch(selected.flags&(FXFont::Mono|FXFont::Variable|FXFont::Dual|FXFont::Cell)){
    case FXFont::Variable: item=1; break;
    case FXFont::Mono|FXFont::Cell: item=2; break;
    case FXFont::Cell: item=3; break;
    case FXFont::Dual: item=4; break;
    }
  pitch->setCurrentItem(item);
  return 1;
  }


// Scalable toggle
long FXFontSelector::onCmdScalable(FXObject*,FXSelector,void* ptr){
  if(ptr) selected.flags|=FXFont::Scalable; else selected.flags&=~FXFont::Scalable;
  listFontFaces();
  listWeights();
  listSlants();
  listFontSizes();
  previewFont();
  return 1;
  }


// Update scalable toggle
long FXFontSelector::onUpdScalable(FXObject*,FXSelector,void*){
  scalable->setCheck((selected.flags&FXFont::Scalable)!=0);
  return 1;
  }


// All fonts toggle
long FXFontSelector::onCmdAllFonts(FXObject*,FXSelector,void* ptr){
  if(ptr) selected.flags|=FXFont::X11; else selected.flags&=~FXFont::X11;
  listFontFaces();
  listWeights();
  listSlants();
  listFontSizes();
  previewFont();
  return 1;
  }


// Update all fonts toggle
long FXFontSelector::onUpdAllFonts(FXObject*,FXSelector,void*){
  allfonts->setCheck((selected.flags&FXFont::X11)!=0);
  return 1;
  }


// Set font selection as a string
void FXFontSelector::setFont(const FXString& string){
  selected.setFont(string);

  // Validate these numbers
  if(selected.encoding>FONTENCODING_UNICODE){
    selected.encoding=FONTENCODING_UNICODE;
    }
  if(selected.slant>FXFont::ReverseOblique){
    selected.slant=FXFont::ReverseOblique;
    }
  if(selected.weight>FXFont::Black){
    selected.weight=FXFont::Black;
    }
  if(selected.setwidth>FXFont::UltraExpanded){
    selected.setwidth=FXFont::UltraExpanded;
    }
  if(selected.size>10000){
    selected.size=10000;
    }

  // Under Windows, this should be OFF
  selected.flags&=~FXFont::X11;

  // Relist fonts
  listFontFaces();
  listWeights();
  listSlants();
  listFontSizes();

  // Update preview
  previewFont();
  }


// Get font selection as a string
FXString FXFontSelector::getFont() const {
  return selected.getFont();
  }


// Change font selection
void FXFontSelector::setFontDesc(const FXFontDesc& fontdesc){
  selected=fontdesc;

  // Validate these numbers
  if(selected.encoding>FONTENCODING_UNICODE){
    selected.encoding=FONTENCODING_UNICODE;
    }
  if(selected.slant>FXFont::ReverseOblique){
    selected.slant=FXFont::ReverseOblique;
    }
  if(selected.weight>FXFont::Black){
    selected.weight=FXFont::Black;
    }
  if(selected.setwidth>FXFont::UltraExpanded){
    selected.setwidth=FXFont::UltraExpanded;
    }
  if(selected.size>10000){
    selected.size=10000;
    }

  // Under Windows, this should be OFF
  selected.flags&=~FXFont::X11;

  // Relist fonts
  listFontFaces();
  listWeights();
  listSlants();
  listFontSizes();

  // Update preview
  previewFont();
  }


// Change font selection
const FXFontDesc& FXFontSelector::getFontDesc() const {
  return selected;
  }


// Set sample text for font sample
void FXFontSelector::setSampleText(const FXString& sampletext){
  preview->setText(sampletext);
  }


// Save data
void FXFontSelector::save(FXStream& store) const {
  FXPacker::save(store);
  store << family;
  store << familylist;
  store << weight;
  store << weightlist;
  store << style;
  store << stylelist;
  store << size;
  store << sizelist;
  store << charset;
  store << setwidth;
  store << pitch;
  store << scalable;
  store << allfonts;
  store << accept;
  store << cancel;
  store << preview;
  store << previewfont;
  }


// Load data
void FXFontSelector::load(FXStream& store){
  FXPacker::load(store);
  store >> family;
  store >> familylist;
  store >> weight;
  store >> weightlist;
  store >> style;
  store >> stylelist;
  store >> size;
  store >> sizelist;
  store >> charset;
  store >> setwidth;
  store >> pitch;
  store >> scalable;
  store >> allfonts;
  store >> accept;
  store >> cancel;
  store >> preview;
  store >> previewfont;
  }


// Cleanup
FXFontSelector::~FXFontSelector(){
  delete previewfont;
  family=(FXTextField*)-1L;
  familylist=(FXList*)-1L;
  weight=(FXTextField*)-1L;
  weightlist=(FXList*)-1L;
  style=(FXTextField*)-1L;
  stylelist=(FXList*)-1L;
  size=(FXTextField*)-1L;
  sizelist=(FXList*)-1L;
  charset=(FXComboBox*)-1L;
  setwidth=(FXComboBox*)-1L;
  pitch=(FXComboBox*)-1L;
  scalable=(FXCheckButton*)-1L;
  allfonts=(FXCheckButton*)-1L;
  preview=(FXLabel*)-1L;
  previewfont=(FXFont*)-1L;
  accept=(FXButton*)-1L;
  cancel=(FXButton*)-1L;
  }

}

