/******************************************************************************
 * $Id: gnm_csharp.i e3dcd9dd0abb07d5da0e410272e0b4c5f4530411 2016-06-20 10:44:12Z Dmitry Baryshnikov $
******************************************************************************/

%include cpl_exceptions.i

%rename (RegisterAll) OGRRegisterAll();

%include typemaps_csharp.i

DEFINE_EXTERNAL_CLASS(OSRSpatialReferenceShadow, OSGeo.OSR.SpatialReference)
DEFINE_EXTERNAL_CLASS(OSRCoordinateTransformationShadow, OSGeo.OSR.CoordinateTransformation)
DEFINE_EXTERNAL_CLASS(GDALMajorObjectShadow, OSGeo.GDAL.MajorObject)

