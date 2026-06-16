#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "utf8_range::utf8_validity" for configuration "Debug"
set_property(TARGET utf8_range::utf8_validity APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(utf8_range::utf8_validity PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libutf8_validity.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libutf8_validity.dll"
  )

list(APPEND _cmake_import_check_targets utf8_range::utf8_validity )
list(APPEND _cmake_import_check_files_for_utf8_range::utf8_validity "${_IMPORT_PREFIX}/lib/libutf8_validity.lib" "${_IMPORT_PREFIX}/bin/libutf8_validity.dll" )

# Import target "utf8_range::utf8_range" for configuration "Debug"
set_property(TARGET utf8_range::utf8_range APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(utf8_range::utf8_range PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libutf8_range.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libutf8_range.dll"
  )

list(APPEND _cmake_import_check_targets utf8_range::utf8_range )
list(APPEND _cmake_import_check_files_for_utf8_range::utf8_range "${_IMPORT_PREFIX}/lib/libutf8_range.lib" "${_IMPORT_PREFIX}/bin/libutf8_range.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
