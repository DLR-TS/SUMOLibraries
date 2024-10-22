#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "giflib::giflib" for configuration "Release"
set_property(TARGET giflib::giflib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(giflib::giflib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/giflib.lib"
  )

list(APPEND _cmake_import_check_targets giflib::giflib )
list(APPEND _cmake_import_check_files_for_giflib::giflib "${_IMPORT_PREFIX}/lib/giflib.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
