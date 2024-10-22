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

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
