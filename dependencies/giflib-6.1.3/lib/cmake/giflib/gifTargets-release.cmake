#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gif::gif" for configuration "Release"
set_property(TARGET gif::gif APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(gif::gif PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/gif.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/gif.dll"
  )

list(APPEND _cmake_import_check_targets gif::gif )
list(APPEND _cmake_import_check_files_for_gif::gif "${_IMPORT_PREFIX}/lib/gif.lib" "${_IMPORT_PREFIX}/bin/gif.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
