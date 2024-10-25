#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "expat::expat" for configuration "Debug"
set_property(TARGET expat::expat APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(expat::expat PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libexpatd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libexpatd.dll"
  )

list(APPEND _cmake_import_check_targets expat::expat )
list(APPEND _cmake_import_check_files_for_expat::expat "${_IMPORT_PREFIX}/lib/libexpatd.lib" "${_IMPORT_PREFIX}/bin/libexpatd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
