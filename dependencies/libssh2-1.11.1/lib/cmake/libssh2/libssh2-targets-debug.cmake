#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libssh2::libssh2_shared" for configuration "Debug"
set_property(TARGET libssh2::libssh2_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libssh2::libssh2_shared PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libssh2d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libssh2d.dll"
  )

list(APPEND _cmake_import_check_targets libssh2::libssh2_shared )
list(APPEND _cmake_import_check_files_for_libssh2::libssh2_shared "${_IMPORT_PREFIX}/lib/libssh2d.lib" "${_IMPORT_PREFIX}/bin/libssh2d.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
