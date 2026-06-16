#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "WebP::sharpyuv" for configuration "Release"
set_property(TARGET WebP::sharpyuv APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WebP::sharpyuv PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libsharpyuv.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libsharpyuv.dll"
  )

list(APPEND _cmake_import_check_targets WebP::sharpyuv )
list(APPEND _cmake_import_check_files_for_WebP::sharpyuv "${_IMPORT_PREFIX}/lib/libsharpyuv.lib" "${_IMPORT_PREFIX}/bin/libsharpyuv.dll" )

# Import target "WebP::webpdecoder" for configuration "Release"
set_property(TARGET WebP::webpdecoder APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WebP::webpdecoder PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libwebpdecoder.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libwebpdecoder.dll"
  )

list(APPEND _cmake_import_check_targets WebP::webpdecoder )
list(APPEND _cmake_import_check_files_for_WebP::webpdecoder "${_IMPORT_PREFIX}/lib/libwebpdecoder.lib" "${_IMPORT_PREFIX}/bin/libwebpdecoder.dll" )

# Import target "WebP::webp" for configuration "Release"
set_property(TARGET WebP::webp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WebP::webp PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libwebp.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libwebp.dll"
  )

list(APPEND _cmake_import_check_targets WebP::webp )
list(APPEND _cmake_import_check_files_for_WebP::webp "${_IMPORT_PREFIX}/lib/libwebp.lib" "${_IMPORT_PREFIX}/bin/libwebp.dll" )

# Import target "WebP::webpdemux" for configuration "Release"
set_property(TARGET WebP::webpdemux APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WebP::webpdemux PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libwebpdemux.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libwebpdemux.dll"
  )

list(APPEND _cmake_import_check_targets WebP::webpdemux )
list(APPEND _cmake_import_check_files_for_WebP::webpdemux "${_IMPORT_PREFIX}/lib/libwebpdemux.lib" "${_IMPORT_PREFIX}/bin/libwebpdemux.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
