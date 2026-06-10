#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GEOS::geos" for configuration "Release"
set_property(TARGET GEOS::geos APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GEOS::geos PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/geos.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/geos.dll"
  )

list(APPEND _cmake_import_check_targets GEOS::geos )
list(APPEND _cmake_import_check_files_for_GEOS::geos "${_IMPORT_PREFIX}/lib/geos.lib" "${_IMPORT_PREFIX}/bin/geos.dll" )

# Import target "GEOS::geos_c" for configuration "Release"
set_property(TARGET GEOS::geos_c APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GEOS::geos_c PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/geos_c.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "GEOS::geos"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/geos_c.dll"
  )

list(APPEND _cmake_import_check_targets GEOS::geos_c )
list(APPEND _cmake_import_check_files_for_GEOS::geos_c "${_IMPORT_PREFIX}/lib/geos_c.lib" "${_IMPORT_PREFIX}/bin/geos_c.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
