#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Iconv::Charset" for configuration "Release"
set_property(TARGET Iconv::Charset APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Iconv::Charset PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libcharset.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libcharset.dll"
  )

list(APPEND _cmake_import_check_targets Iconv::Charset )
list(APPEND _cmake_import_check_files_for_Iconv::Charset "${_IMPORT_PREFIX}/lib/libcharset.lib" "${_IMPORT_PREFIX}/bin/libcharset.dll" )

# Import target "Iconv::Iconv" for configuration "Release"
set_property(TARGET Iconv::Iconv APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Iconv::Iconv PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libiconv.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libiconv.dll"
  )

list(APPEND _cmake_import_check_targets Iconv::Iconv )
list(APPEND _cmake_import_check_files_for_Iconv::Iconv "${_IMPORT_PREFIX}/lib/libiconv.lib" "${_IMPORT_PREFIX}/bin/libiconv.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
