## Process this file with automake to produce Makefile.in

AM_CXXFLAGS = @X_CFLAGS@
INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include
RESWRAP  = $(top_builddir)/utils/reswrap$(EXEEXT)
SUFFIXES = .gif .bmp
LDADD = $(top_builddir)/lib/libFOX-1.7.la -lm @GLU_LIBS@

noinst_PROGRAMS	= \
glviewer

glviewer_SOURCES = \
FXGLCone.h \
FXGLCube.h \
FXGLCylinder.h \
FXGLLine.h \
FXGLPoint.h \
FXGLShape.h \
FXGLSphere.h \
FXGLTriangleMesh.h \
FXGLCone.cpp \
FXGLCube.cpp \
FXGLCylinder.cpp \
FXGLLine.cpp \
FXGLPoint.cpp \
FXGLShape.cpp \
FXGLSphere.cpp \
FXGLTriangleMesh.cpp \
glviewer.cpp
