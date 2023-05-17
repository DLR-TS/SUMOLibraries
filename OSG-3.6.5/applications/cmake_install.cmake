# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/applications

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Librerias/SUMOLibraries/OSG-3.6.5")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/applications/osgviewer/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/applications/osgarchive/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/applications/osgconv/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/applications/osgfilecache/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/applications/osgversion/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/applications/present3D/cmake_install.cmake")

endif()

