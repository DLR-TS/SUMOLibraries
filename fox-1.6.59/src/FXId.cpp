/********************************************************************************
*                                                                               *
*                                  X - O b j e c t                              *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2006 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or                 *
* modify it under the terms of the GNU Lesser General Public                    *
* License as published by the Free Software Foundation; either                  *
* version 2.1 of the License, or (at your option) any later version.            *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
* Lesser General Public License for more details.                               *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public              *
* License along with this library; if not, write to the Free Software           *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.    *
*********************************************************************************
* $Id: FXId.cpp,v 1.24 2006/01/22 17:58:32 fox Exp $                            *
********************************************************************************/
#include "xincs.h"
#include "fxver.h"
#include "fxdefs.h"
#include "FXHash.h"
#include "FXThread.h"
#include "FXStream.h"
#include "FXString.h"
#include "FXSize.h"
#include "FXPoint.h"
#include "FXRectangle.h"
#include "FXRegistry.h"
#include "FXApp.h"
#include "FXId.h"
#include "FXException.h"

using namespace FX;

enum { MAGIC = 0x464f5831 };


/*******************************************************************************/

namespace FX {

// Object implementation
FXIMPLEMENT_ABSTRACT(FXId,FXObject,NULL,0)


// Save data
void FXId::save(FXStream& store) const {
  FXuint version=MAGIC;
  FXObject::save(store);
  store << version;
  store << app;
  }


// Load data
void FXId::load(FXStream& store){
  FXuint version;
  FXObject::load(store);
  store >> version;
  if(version!=MAGIC){
    store.setError(FXStreamFormat);
    throw FXResourceException("expected to match MAGIC tag");
    }
  store >> app;
  }

}
