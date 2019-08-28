#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "proj" for configuration "Release"
set_property(TARGET proj APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(proj PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/proj_6_1.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/proj_6_1.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS proj )
list(APPEND _IMPORT_CHECK_FILES_FOR_proj "${_IMPORT_PREFIX}/lib/proj_6_1.lib" "${_IMPORT_PREFIX}/bin/proj_6_1.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
