#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xerces_xerces-c" for configuration "Debug"
set_property(TARGET xerces_xerces-c APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(xerces_xerces-c PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/xerces-c_3D.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/xerces-c_3_3D.dll"
  )

list(APPEND _cmake_import_check_targets xerces_xerces-c )
list(APPEND _cmake_import_check_files_for_xerces_xerces-c "${_IMPORT_PREFIX}/lib/xerces-c_3D.lib" "${_IMPORT_PREFIX}/bin/xerces-c_3_3D.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
