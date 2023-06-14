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
#include "FXGLLine.h"

#define HANDLE_SIZE 4.0

/*******************************************************************************/

// Object implementation
FXIMPLEMENT(FXGLLine,FXGLObject,nullptr,0)


// Create line
FXGLLine::FXGLLine():fm(-0.5f,0.0f,0.0f),to(0.5f,0.0f,0.0f){
  }


// Copy constructor
FXGLLine::FXGLLine(const FXGLLine& orig):FXGLObject(orig),fm(orig.fm),to(orig.to){
  }


// Create inittialized line
FXGLLine::FXGLLine(FXfloat fx,FXfloat fy,FXfloat fz,FXfloat tx,FXfloat ty,FXfloat tz):fm(fx,fy,fz),to(tx,ty,tz){
  }


// Get bounding box
void FXGLLine::bounds(FXRangef& box){
  box.lower.x=Math::fmin(fm.pos.x,to.pos.x); box.upper.x=Math::fmax(fm.pos.x,to.pos.x);
  box.lower.y=Math::fmin(fm.pos.y,to.pos.y); box.upper.y=Math::fmax(fm.pos.y,to.pos.y);
  box.lower.z=Math::fmin(fm.pos.z,to.pos.z); box.upper.z=Math::fmax(fm.pos.z,to.pos.z);
  }


// Draw
void FXGLLine::draw(FXGLViewer*){
#ifdef HAVE_GL_H
  glColor3f(1.0,0.0,0.0);
  glPointSize(HANDLE_SIZE);
  glBegin(GL_LINES);
  glVertex3fv(fm.pos);
  glVertex3fv(to.pos);
  glEnd();
#endif
  }


// Draw for hit
void FXGLLine::hit(FXGLViewer* ){
#ifdef HAVE_GL_H
  glBegin(GL_LINES);
  glVertex3fv(fm.pos);
  glVertex3fv(to.pos);
  glEnd();
#endif
  }


// Copy
FXGLObject* FXGLLine::copy(){
  return new FXGLLine(*this);
  }


// Save object to stream
void FXGLLine::save(FXStream& store) const {
  FXGLObject::save(store);
  fm.save(store);
  to.save(store);
  }


// Load object from stream
void FXGLLine::load(FXStream& store){
  FXGLObject::load(store);
  fm.load(store);
  to.load(store);
  }
