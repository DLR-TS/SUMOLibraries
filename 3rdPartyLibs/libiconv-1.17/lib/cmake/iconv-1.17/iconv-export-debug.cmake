#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Iconv::Charset" for configuration "Debug"
set_property(TARGET Iconv::Charset APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Iconv::Charset PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libcharset_Debug.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libcharset_Debug.dll"
  )

list(APPEND _cmake_import_check_targets Iconv::Charset )
list(APPEND _cmake_import_check_files_for_Iconv::Charset "${_IMPORT_PREFIX}/lib/libcharset_Debug.lib" "${_IMPORT_PREFIX}/bin/libcharset_Debug.dll" )

# Import target "Iconv::Iconv" for configuration "Debug"
set_property(TARGET Iconv::Iconv APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Iconv::Iconv PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libiconv_Debug.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libiconv_Debug.dll"
  )

list(APPEND _cmake_import_check_targets Iconv::Iconv )
list(APPEND _cmake_import_check_files_for_Iconv::Iconv "${_IMPORT_PREFIX}/lib/libiconv_Debug.lib" "${_IMPORT_PREFIX}/bin/libiconv_Debug.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
