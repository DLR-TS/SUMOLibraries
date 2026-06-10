#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ZLIB::ZLIB" for configuration "Debug"
set_property(TARGET ZLIB::ZLIB APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ZLIB::ZLIB PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/zd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/zd.dll"
  )

list(APPEND _cmake_import_check_targets ZLIB::ZLIB )
list(APPEND _cmake_import_check_files_for_ZLIB::ZLIB "${_IMPORT_PREFIX}/lib/zd.lib" "${_IMPORT_PREFIX}/bin/zd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
