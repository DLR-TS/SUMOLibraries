#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TIFF::tiff" for configuration "Release"
set_property(TARGET TIFF::tiff APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TIFF::tiff PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/tiff.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/tiff.dll"
  )

list(APPEND _cmake_import_check_targets TIFF::tiff )
list(APPEND _cmake_import_check_files_for_TIFF::tiff "${_IMPORT_PREFIX}/lib/tiff.lib" "${_IMPORT_PREFIX}/bin/tiff.dll" )

# Import target "TIFF::tiffxx" for configuration "Release"
set_property(TARGET TIFF::tiffxx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TIFF::tiffxx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/tiffxx.lib"
  )

list(APPEND _cmake_import_check_targets TIFF::tiffxx )
list(APPEND _cmake_import_check_files_for_TIFF::tiffxx "${_IMPORT_PREFIX}/lib/tiffxx.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
