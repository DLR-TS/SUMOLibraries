#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gif::gif" for configuration "Debug"
set_property(TARGET gif::gif APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gif::gif PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/gifd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/gifd.dll"
  )

list(APPEND _cmake_import_check_targets gif::gif )
list(APPEND _cmake_import_check_files_for_gif::gif "${_IMPORT_PREFIX}/lib/gifd.lib" "${_IMPORT_PREFIX}/bin/gifd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
