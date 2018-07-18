#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xerces_xerces-c" for configuration "Release"
set_property(TARGET xerces_xerces-c APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xerces_xerces-c PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/xerces-c_3.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/xerces-c_3_2.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS xerces_xerces-c )
list(APPEND _IMPORT_CHECK_FILES_FOR_xerces_xerces-c "${_IMPORT_PREFIX}/lib/xerces-c_3.lib" "${_IMPORT_PREFIX}/bin/xerces-c_3_2.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
