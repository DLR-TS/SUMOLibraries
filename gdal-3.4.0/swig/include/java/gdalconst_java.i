/******************************************************************************
 * $Id: gdalconst_java.i 19f708c8a21ace821b243cbb6833e12dffcb82b3 2018-01-08 19:18:49Z Even Rouault $
 *
 * Name:     gdalconst_java.i
 * Project:  GDAL SWIG Interface
 * Purpose:  Typemaps for Java bindings
 * Author:   Benjamin Collins, The MITRE Corporation
 *
*/

%pragma(java) jniclasscode=%{

  static {
      gdalJNI.isAvailable();   // force gdalJNI static initializer to run and load library
  }

  public static boolean isAvailable() {
    return gdalJNI.isAvailable();
  }
%}

%pragma(java) jniclassimports=%{
import org.gdal.gdal.gdalJNI;
%}

%pragma(java) modulecode=%{

    /* Uninstanciable class */
    private gdalconst()
    {
    }
%}

%include typemaps_java.i
