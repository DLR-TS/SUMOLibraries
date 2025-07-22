#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gperf::gperf" for configuration "Debug"
set_property(TARGET gperf::gperf APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gperf::gperf PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/gperf.exe"
  )

list(APPEND _cmake_import_check_targets gperf::gperf )
list(APPEND _cmake_import_check_files_for_gperf::gperf "${_IMPORT_PREFIX}/bin/gperf.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
