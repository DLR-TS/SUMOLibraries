# CMake generated Testfile for 
# Source directory: C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests
# Build directory: C:/Librerias/SUMOLibraries/proj-9.0.0/build/src/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(geodesic-test "C:/Librerias/SUMOLibraries/proj-9.0.0/build/bin/Debug/geodtest.exe")
  set_tests_properties(geodesic-test PROPERTIES  _BACKTRACE_TRIPLES "C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests/bin_geodtest.cmake;10;add_test;C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests/bin_geodtest.cmake;0;;C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests/CMakeLists.txt;3;include;C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(geodesic-test "C:/Librerias/SUMOLibraries/proj-9.0.0/build/bin/Release/geodtest.exe")
  set_tests_properties(geodesic-test PROPERTIES  _BACKTRACE_TRIPLES "C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests/bin_geodtest.cmake;10;add_test;C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests/bin_geodtest.cmake;0;;C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests/CMakeLists.txt;3;include;C:/Librerias/SUMOLibraries/proj-9.0.0/src/tests/CMakeLists.txt;0;")
else()
  add_test(geodesic-test NOT_AVAILABLE)
endif()
