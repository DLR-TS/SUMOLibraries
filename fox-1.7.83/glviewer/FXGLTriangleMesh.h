/********************************************************************************
*                                                                               *
*             O p e n G L   T r i a n g l e   M e s h   O b j e c t             *
*                                                                               *
*********************************************************************************
* Copyright (C) 1999,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXGLTRIANGLEMESH_H
#define FXGLTRIANGLEMESH_H


/// OpenGL Triangle Mesh Object
class FXGLTriangleMesh : public FXGLShape {
  FXDECLARE(FXGLTriangleMesh)
private:
  FXfloat   *vertexBuffer;
  FXfloat   *colorBuffer;
  FXfloat   *normalBuffer;
  FXfloat   *textureBuffer;
  FXint      vertexNumber;
protected:
  FXGLTriangleMesh();
  virtual void drawshape(FXGLViewer* viewer);
  virtual void recomputerange();
  virtual void generatenormals();
public:

  /// Construct triangle mesh with nv vertices, and optional normals, colors, and texture coordinates
  FXGLTriangleMesh(FXfloat x,FXfloat y,FXfloat z,FXint nv,FXfloat *v,FXfloat *n=nullptr,FXfloat *c=nullptr,FXfloat *t=nullptr);

  /// Construct triangle mesh with nv vertices, and optional normals, colors, and texture coordinates, and surface material
  FXGLTriangleMesh(FXfloat x,FXfloat y,FXfloat z,FXint nv,FXfloat *v,FXfloat *n,FXfloat *c,FXfloat *t,const FXMaterial& mtl);

  /// Copy constructor
  FXGLTriangleMesh(const FXGLTriangleMesh& orig);

  /// Copy this object
  virtual FXGLObject* copy();

  /// Change number of vertices
  void setVertexNumber(FXint nvertices){ vertexNumber=nvertices; }

  /// Get number of vertices
  FXint getVertexNumber() const { return vertexNumber; }

  /// Set vertex buffer
  void setVertexBuffer(FXfloat *vertices);

  /// Get vertex buffer
  FXfloat* getVertexBuffer() const { return vertexBuffer; }

  /// Set color buffer
  void setColorBuffer(FXfloat *colors){ colorBuffer=colors; }

  /// Get color buffer
  FXfloat* getColorBuffer() const { return colorBuffer; }

  /// Set normals buffer
  void setNormalBuffer(FXfloat *normals){ normalBuffer=normals; }

  /// Get normals buffer
  FXfloat* getNormalBuffer() const { return normalBuffer; }

  /// Set texture coordinate buffer
  void setTextureCoordBuffer(FXfloat *textures){ textureBuffer=textures; }

  /// Get texture coordinate buffer
  FXfloat* getTextureCoordBuffer() const { return textureBuffer; }

  /// Save to a stream
  virtual void save(FXStream& store) const;

  /// Load from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXGLTriangleMesh();
  };

#endif
