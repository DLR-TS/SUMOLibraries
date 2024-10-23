#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GDAL::GDAL" for configuration "Debug"
set_property(TARGET GDAL::GDAL APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GDAL::GDAL PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/gdald.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "CURL::libcurl_shared;PROJ::proj;GEOS::geos_c;expat::expat"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/gdald.dll"
  )

list(APPEND _cmake_import_check_targets GDAL::GDAL )
list(APPEND _cmake_import_check_files_for_GDAL::GDAL "${_IMPORT_PREFIX}/lib/gdald.lib" "${_IMPORT_PREFIX}/bin/gdald.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
