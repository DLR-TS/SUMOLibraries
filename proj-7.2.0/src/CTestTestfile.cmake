# CMake generated Testfile for 
# Source directory: C:/Librerias/SUMOLibraries/proj-7.2.0/src
# Build directory: C:/Librerias/SUMOLibraries/proj-7.2.0/src
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(geodesic-test "C:/Librerias/SUMOLibraries/proj-7.2.0/bin/Debug/geodtest.exe")
  set_tests_properties(geodesic-test PROPERTIES  _BACKTRACE_TRIPLES "C:/Librerias/SUMOLibraries/proj-7.2.0/src/bin_geodtest.cmake;11;add_test;C:/Librerias/SUMOLibraries/proj-7.2.0/src/bin_geodtest.cmake;0;;C:/Librerias/SUMOLibraries/proj-7.2.0/src/CMakeLists.txt;57;include;C:/Librerias/SUMOLibraries/proj-7.2.0/src/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(geodesic-test "C:/Librerias/SUMOLibraries/proj-7.2.0/bin/Release/geodtest.exe")
  set_tests_properties(geodesic-test PROPERTIES  _BACKTRACE_TRIPLES "C:/Librerias/SUMOLibraries/proj-7.2.0/src/bin_geodtest.cmake;11;add_test;C:/Librerias/SUMOLibraries/proj-7.2.0/src/bin_geodtest.cmake;0;;C:/Librerias/SUMOLibraries/proj-7.2.0/src/CMakeLists.txt;57;include;C:/Librerias/SUMOLibraries/proj-7.2.0/src/CMakeLists.txt;0;")
else()
  add_test(geodesic-test NOT_AVAILABLE)
endif()
