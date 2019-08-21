# CMake generated Testfile for 
# Source directory: C:/Libraries/32bits/proj-6.1.1/src
# Build directory: C:/Libraries/32bits/proj-6.1.1/src
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(geodesic-test "C:/Libraries/32bits/proj-6.1.1/bin/Release/geodtest.exe")
  set_tests_properties(geodesic-test PROPERTIES  _BACKTRACE_TRIPLES "C:/Libraries/32bits/proj-6.1.1/src/bin_geodtest.cmake;11;add_test;C:/Libraries/32bits/proj-6.1.1/src/bin_geodtest.cmake;0;;C:/Libraries/32bits/proj-6.1.1/src/CMakeLists.txt;54;include;C:/Libraries/32bits/proj-6.1.1/src/CMakeLists.txt;0;")
else()
  add_test(geodesic-test NOT_AVAILABLE)
endif()
