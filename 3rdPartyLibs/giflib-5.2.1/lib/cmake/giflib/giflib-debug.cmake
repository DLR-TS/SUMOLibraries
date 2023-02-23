#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "giflib::giflib" for configuration "Debug"
set_property(TARGET giflib::giflib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(giflib::giflib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/giflibd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS giflib::giflib )
list(APPEND _IMPORT_CHECK_FILES_FOR_giflib::giflib "${_IMPORT_PREFIX}/lib/giflibd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
