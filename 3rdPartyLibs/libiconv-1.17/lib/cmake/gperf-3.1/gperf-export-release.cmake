#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gperf::gperf" for configuration "Release"
set_property(TARGET gperf::gperf APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(gperf::gperf PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/gperf.exe"
  )

list(APPEND _cmake_import_check_targets gperf::gperf )
list(APPEND _cmake_import_check_files_for_gperf::gperf "${_IMPORT_PREFIX}/bin/gperf.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
