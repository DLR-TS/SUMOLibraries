#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "MINIZIP::minizip" for configuration "Release"
set_property(TARGET MINIZIP::minizip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MINIZIP::minizip PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/minizip-classic.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/minizip-classic.dll"
  )

list(APPEND _cmake_import_check_targets MINIZIP::minizip )
list(APPEND _cmake_import_check_files_for_MINIZIP::minizip "${_IMPORT_PREFIX}/lib/minizip-classic.lib" "${_IMPORT_PREFIX}/bin/minizip-classic.dll" )

# Import target "MINIZIP::minizip_executable" for configuration "Release"
set_property(TARGET MINIZIP::minizip_executable APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MINIZIP::minizip_executable PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/minizip.exe"
  )

list(APPEND _cmake_import_check_targets MINIZIP::minizip_executable )
list(APPEND _cmake_import_check_files_for_MINIZIP::minizip_executable "${_IMPORT_PREFIX}/bin/minizip.exe" )

# Import target "MINIZIP::miniunzip_executable" for configuration "Release"
set_property(TARGET MINIZIP::miniunzip_executable APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MINIZIP::miniunzip_executable PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/miniunzip.exe"
  )

list(APPEND _cmake_import_check_targets MINIZIP::miniunzip_executable )
list(APPEND _cmake_import_check_files_for_MINIZIP::miniunzip_executable "${_IMPORT_PREFIX}/bin/miniunzip.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
