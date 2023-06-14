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
#ifndef FXGLPOINT_H
#define FXGLPOINT_H


/// OpenGL Point Object
class FXAPI FXGLPoint : public FXGLObject {
  FXDECLARE(FXGLPoint)
public:
  FXVec3f pos;
public:

  /// Default constructor
  FXGLPoint();

  /// Copy constructor
  FXGLPoint(const FXGLPoint& orig);

  /// Construct with specified coordinates
  FXGLPoint(FXfloat x,FXfloat y,FXfloat z);

  /// Copy this object
  virtual FXGLObject* copy();

  /// Called by the viewer to get bounds for this object
  virtual void bounds(FXRangef& box);

  /// Draw this object in a viewer
  virtual void draw(FXGLViewer* viewer);

  /// Draw this object for hit-testing purposes
  virtual void hit(FXGLViewer* viewer);

  /// Save to a stream
  virtual void save(FXStream& store) const;

  /// Load from a stream
  virtual void load(FXStream& store);
  };

#endif

