/*
 * $Id: gnm_python.i 592b14dafd74fbe83ef717c0d4ad704087b15212 2017-05-16 19:28:50Z Even Rouault $
 *
 * python specific code for ogr bindings.
 */

%feature("autodoc");

#ifndef FROM_GDAL_I
%init %{

  if ( OGRGetDriverCount() == 0 ) {
    OGRRegisterAll();
  }

%}
#endif

%rename (RegisterAll) OGRRegisterAll();

#ifndef FROM_GDAL_I
%{
#define MODULE_NAME           "gnm"
%}
%include "python_exceptions.i"
%include "python_strings.i"

%import typemaps_python.i

%include "callback.i"


%extend GDALMajorObjectShadow {
%pythoncode %{
  def GetMetadata( self, domain = '' ):
    if domain[:4] == 'xml:':
      return self.GetMetadata_List( domain )
    return self.GetMetadata_Dict( domain )
%}
}
#endif

