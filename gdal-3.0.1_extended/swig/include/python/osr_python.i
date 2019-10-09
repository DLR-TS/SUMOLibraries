/*
 * $Id: osr_python.i 8e5eeb35bf76390e3134a4ea7076dab7d478ea0e 2018-11-14 22:55:13 +0100 Even Rouault $
 *
 * python specific code for ogr bindings.
 */

%feature("autodoc");

#ifndef FROM_GDAL_I
%{
#define MODULE_NAME           "osr"
%}

%include "python_exceptions.i"
%include "python_strings.i"
#endif

%include typemaps_python.i
