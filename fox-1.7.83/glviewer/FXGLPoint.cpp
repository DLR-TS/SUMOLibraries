/********************************************************************************
*                                                                               *
*                           O p e n G L   O b j e c t                           *
*                                                                               *
*********************************************************************************
* Copyright (C) 1998,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#include "fx3d.h"
#include "FXGLPoint.h"

#define HANDLE_SIZE 4.0

/*******************************************************************************/

// Object implementation
FXIMPLEMENT(FXGLPoint,FXGLObject,nullptr,0)


// Create point
FXGLPoint::FXGLPoint():pos(0.0f,0.0f,0.0f){
  }

// Copy constructor
FXGLPoint::FXGLPoint(const FXGLPoint& orig):FXGLObject(orig),pos(orig.pos){
  }

// Create initialized point
FXGLPoint::FXGLPoint(FXfloat x,FXfloat y,FXfloat z):pos(x,y,z){
  }


// Get bounding box
void FXGLPoint::bounds(FXRangef& box){
  box.upper.x=box.lower.x=pos.x;
  box.upper.y=box.lower.y=pos.y;
  box.upper.z=box.lower.z=pos.z;
  }


// Draw
void FXGLPoint::draw(FXGLViewer* ){
#ifdef HAVE_GL_H
  glColor3f(0.0,0.0,1.0);
  glPointSize(HANDLE_SIZE);
  glBegin(GL_POINTS);
  glVertex3fv(pos);
  glEnd();
#endif
  }


// Draw for hit
void FXGLPoint::hit(FXGLViewer* ){
#ifdef HAVE_GL_H
  glBegin(GL_POINTS);
  glVertex3fv(pos);
  glEnd();
#endif
  }


// Copy
FXGLObject* FXGLPoint::copy(){
  return new FXGLPoint(*this);
  }

// Save object to stream
void FXGLPoint::save(FXStream& store) const {
  FXGLObject::save(store);
  store << pos;
  }


// Load object from stream
void FXGLPoint::load(FXStream& store){
  FXGLObject::load(store);
  store >> pos;
  }
