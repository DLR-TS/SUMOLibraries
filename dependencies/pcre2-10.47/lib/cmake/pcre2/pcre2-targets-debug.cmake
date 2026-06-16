#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "pcre2::pcre2-8-shared" for configuration "Debug"
set_property(TARGET pcre2::pcre2-8-shared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(pcre2::pcre2-8-shared PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/pcre2-8d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/pcre2-8d.dll"
  )

list(APPEND _cmake_import_check_targets pcre2::pcre2-8-shared )
list(APPEND _cmake_import_check_files_for_pcre2::pcre2-8-shared "${_IMPORT_PREFIX}/lib/pcre2-8d.lib" "${_IMPORT_PREFIX}/bin/pcre2-8d.dll" )

# Import target "pcre2::pcre2-posix-shared" for configuration "Debug"
set_property(TARGET pcre2::pcre2-posix-shared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(pcre2::pcre2-posix-shared PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/pcre2-posixd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/pcre2-posixd.dll"
  )

list(APPEND _cmake_import_check_targets pcre2::pcre2-posix-shared )
list(APPEND _cmake_import_check_files_for_pcre2::pcre2-posix-shared "${_IMPORT_PREFIX}/lib/pcre2-posixd.lib" "${_IMPORT_PREFIX}/bin/pcre2-posixd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
