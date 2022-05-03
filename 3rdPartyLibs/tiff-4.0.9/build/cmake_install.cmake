# Install script for directory: C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/lib/pkgconfig/libtiff-4.pc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/lib/pkgconfig" TYPE FILE FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/libtiff-4.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/port/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/libtiff/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/tools/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/test/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/contrib/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/build/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/man/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/html/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Librerias/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
