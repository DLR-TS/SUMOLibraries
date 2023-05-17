#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "png" for configuration "Debug"
set_property(TARGET png APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(png PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libpng16d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libpng16d.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS png )
list(APPEND _IMPORT_CHECK_FILES_FOR_png "${_IMPORT_PREFIX}/lib/libpng16d.lib" "${_IMPORT_PREFIX}/bin/libpng16d.dll" )

# Import target "png_static" for configuration "Debug"
set_property(TARGET png_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(png_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libpng16_staticd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS png_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_png_static "${_IMPORT_PREFIX}/lib/libpng16_staticd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
