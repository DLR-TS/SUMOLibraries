#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "jupedsim::jupedsim" for configuration "Debug"
set_property(TARGET jupedsim::jupedsim APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(jupedsim::jupedsim PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/jupedsim.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/jupedsim.dll"
  )

list(APPEND _cmake_import_check_targets jupedsim::jupedsim )
list(APPEND _cmake_import_check_files_for_jupedsim::jupedsim "${_IMPORT_PREFIX}/lib/jupedsim.lib" "${_IMPORT_PREFIX}/bin/jupedsim.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
