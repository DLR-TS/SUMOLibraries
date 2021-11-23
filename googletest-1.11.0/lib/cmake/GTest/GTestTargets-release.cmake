#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GTest::gtest" for configuration "Release"
set_property(TARGET GTest::gtest APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GTest::gtest PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/gtest.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/gtest.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS GTest::gtest )
list(APPEND _IMPORT_CHECK_FILES_FOR_GTest::gtest "${_IMPORT_PREFIX}/lib/gtest.lib" "${_IMPORT_PREFIX}/bin/gtest.dll" )

# Import target "GTest::gtest_main" for configuration "Release"
set_property(TARGET GTest::gtest_main APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GTest::gtest_main PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/gtest_main.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/gtest_main.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS GTest::gtest_main )
list(APPEND _IMPORT_CHECK_FILES_FOR_GTest::gtest_main "${_IMPORT_PREFIX}/lib/gtest_main.lib" "${_IMPORT_PREFIX}/bin/gtest_main.dll" )

# Import target "GTest::gmock" for configuration "Release"
set_property(TARGET GTest::gmock APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GTest::gmock PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/gmock.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/gmock.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS GTest::gmock )
list(APPEND _IMPORT_CHECK_FILES_FOR_GTest::gmock "${_IMPORT_PREFIX}/lib/gmock.lib" "${_IMPORT_PREFIX}/bin/gmock.dll" )

# Import target "GTest::gmock_main" for configuration "Release"
set_property(TARGET GTest::gmock_main APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GTest::gmock_main PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/gmock_main.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/gmock_main.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS GTest::gmock_main )
list(APPEND _IMPORT_CHECK_FILES_FOR_GTest::gmock_main "${_IMPORT_PREFIX}/lib/gmock_main.lib" "${_IMPORT_PREFIX}/bin/gmock_main.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
