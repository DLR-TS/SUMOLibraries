/********************************************************************************
*                                                                               *
*                     U n d o a b l e   C o m m a n d s                         *
*                                                                               *
*********************************************************************************
* Copyright (C) 1998,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This program is free software: you can redistribute it and/or modify          *
* it under the terms of the GNU General Public License as published by          *
* the Free Software Foundation, either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* This program is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU General Public License for more details.                                  *
*                                                                               *
* You should have received a copy of the GNU General Public License             *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.         *
********************************************************************************/
#include "fx.h"
#include "Commands.h"
#include "Syntax.h"
#include "Modeline.h"
#include "TextWindow.h"
#include "Adie.h"


/*
  Notes:

  - When manipulating text widget, we generate callbacks even when changing
    the text via undo or redo.
  - During the execution of an undo or redo, FXUndoList is marked as busy;
    thus, the busy state may be checked to determine if one is in the middle
    of an undo or redo.
  - Keep undo-text around basically forever; but there are limits: when undo-
    list gets trimmed, old undo-commands may be deleted.
    We have two ways to trim the undo-buffer: by total undo information size or
    by the number of undo records.
    Either way, if the undo buffer ends up being trimmed, only the older undo
    records that are in excess of the chosen trim-limits are deleted; keeping
    the most recent undo records.
*/


/*******************************************************************************/

FXIMPLEMENT_ABSTRACT(FXTextCommand,FXCommand,nullptr,0)

// Command
FXTextCommand::FXTextCommand(FXText* txt,FXint p,FXint nd,FXint ni):text(txt),pos(p),ndel(nd),nins(ni){
  }


// Return size of record plus any data kept here
FXuint FXTextCommand::size() const {
  return sizeof(FXTextCommand)+buffer.length();
  }


// Delete it
FXTextCommand::~FXTextCommand(){
  }


FXIMPLEMENT_ABSTRACT(FXTextInsert,FXTextCommand,nullptr,0)

// Insert command
FXTextInsert::FXTextInsert(FXText* txt,FXint p,FXint ni,const FXchar* ins):FXTextCommand(txt,p,0,ni){
  buffer.assign(ins,nins);
  }


// Undo an insert removes the inserted text
void FXTextInsert::undo(){
  text->removeText(pos,nins,true);
  text->setCursorPos(pos);
  text->makePositionVisible(pos);
  }


// Redo an insert inserts the same old text again
void FXTextInsert::redo(){
  text->insertText(pos,buffer,true);
  text->setCursorPos(pos+nins);
  text->makePositionVisible(pos+nins);
  }


FXIMPLEMENT_ABSTRACT(FXTextDelete,FXTextCommand,nullptr,0)

// Delete command
FXTextDelete::FXTextDelete(FXText* txt,FXint p,FXint nd,const FXchar* del):FXTextCommand(txt,p,nd,0){
  buffer.assign(del,ndel);
  }


// Undo a delete reinserts the old text
void FXTextDelete::undo(){
  text->insertText(pos,buffer,true);
  text->setCursorPos(pos+ndel);
  text->makePositionVisible(pos+ndel);
  }


// Redo a delete removes it again
void FXTextDelete::redo(){
  text->removeText(pos,ndel,true);
  text->setCursorPos(pos);
  text->makePositionVisible(pos);
  }


FXIMPLEMENT_ABSTRACT(FXTextReplace,FXTextCommand,nullptr,0)

// Replace command
FXTextReplace::FXTextReplace(FXText* txt,FXint p,FXint nd,FXint ni,const FXchar* del,const FXchar* ins):FXTextCommand(txt,p,nd,ni){
  buffer.assign(del,ndel);
  buffer.append(ins,nins);
  }


// Undo a replace reinserts the old text
void FXTextReplace::undo(){
  text->replaceText(pos,nins,&buffer[0],ndel,true);
  text->setCursorPos(pos+ndel);
  text->makePositionVisible(pos+ndel);
  }


// Redo a replace reinserts the new text
void FXTextReplace::redo(){
  text->replaceText(pos,ndel,&buffer[ndel],nins,true);
  text->setCursorPos(pos+nins);
  text->makePositionVisible(pos+nins);
  }

