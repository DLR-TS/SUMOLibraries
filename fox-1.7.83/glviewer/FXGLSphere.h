/********************************************************************************
*                                                                               *
*                   O p e n G L   S p h e r e   O b j e c t                     *
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
#ifndef FXGLSPHERE_H
#define FXGLSPHERE_H


/// OpenGL Sphere Object
class FXGLSphere : public FXGLShape {
  FXDECLARE(FXGLSphere)
public:
  FXfloat radius;   // Radius of sphere
  FXint   slices;   // Longitudinal subdivision
  FXint   stacks;   // Latitudinal subdivision
protected:
  FXGLSphere();
  virtual void drawshape(FXGLViewer* viewer);
public:
  enum {
    ID_LAST=FXGLShape::ID_LAST
    };
public:

  /// Construct with specified origin and radius
  FXGLSphere(FXfloat x,FXfloat y,FXfloat z,FXfloat r=1.0f);

  /// Construct with specified origin, radius and material
  FXGLSphere(FXfloat x,FXfloat y,FXfloat z,FXfloat r,const FXMaterial& mtl);

  /// Copy constructor
  FXGLSphere(const FXGLSphere& orig);

  /// Copy this object
  virtual FXGLObject* copy();

  /// Change radius
  virtual void setRadius(FXfloat r);
  FXfloat getRadius() const { return radius; }

  /// Change slices
  void setSlices(FXint s){ slices=s; }
  FXint getSlices() const { return slices; }

  /// Change stacks
  void setStacks(FXint s){ stacks=s; }
  FXint getStacks() const { return stacks; }

  /// Save to a stream
  virtual void save(FXStream& store) const;

  /// Load from a stream
  virtual void load(FXStream& store);

  /// Destroy
  virtual ~FXGLSphere();
  };

#endif
