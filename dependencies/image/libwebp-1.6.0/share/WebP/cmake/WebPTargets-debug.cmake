#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "WebP::sharpyuv" for configuration "Debug"
set_property(TARGET WebP::sharpyuv APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(WebP::sharpyuv PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libsharpyuvd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libsharpyuvd.dll"
  )

list(APPEND _cmake_import_check_targets WebP::sharpyuv )
list(APPEND _cmake_import_check_files_for_WebP::sharpyuv "${_IMPORT_PREFIX}/lib/libsharpyuvd.lib" "${_IMPORT_PREFIX}/bin/libsharpyuvd.dll" )

# Import target "WebP::webpdecoder" for configuration "Debug"
set_property(TARGET WebP::webpdecoder APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(WebP::webpdecoder PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libwebpdecoderd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libwebpdecoderd.dll"
  )

list(APPEND _cmake_import_check_targets WebP::webpdecoder )
list(APPEND _cmake_import_check_files_for_WebP::webpdecoder "${_IMPORT_PREFIX}/lib/libwebpdecoderd.lib" "${_IMPORT_PREFIX}/bin/libwebpdecoderd.dll" )

# Import target "WebP::webp" for configuration "Debug"
set_property(TARGET WebP::webp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(WebP::webp PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libwebpd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libwebpd.dll"
  )

list(APPEND _cmake_import_check_targets WebP::webp )
list(APPEND _cmake_import_check_files_for_WebP::webp "${_IMPORT_PREFIX}/lib/libwebpd.lib" "${_IMPORT_PREFIX}/bin/libwebpd.dll" )

# Import target "WebP::webpdemux" for configuration "Debug"
set_property(TARGET WebP::webpdemux APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(WebP::webpdemux PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libwebpdemuxd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libwebpdemuxd.dll"
  )

list(APPEND _cmake_import_check_targets WebP::webpdemux )
list(APPEND _cmake_import_check_files_for_WebP::webpdemux "${_IMPORT_PREFIX}/lib/libwebpdemuxd.lib" "${_IMPORT_PREFIX}/bin/libwebpdemuxd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
