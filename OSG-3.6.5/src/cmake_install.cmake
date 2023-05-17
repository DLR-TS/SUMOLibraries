# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src

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
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/OpenThreads/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osg/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgDB/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgUtil/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgGA/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgText/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgViewer/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgAnimation/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgFX/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgManipulator/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgParticle/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgUI/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgVolume/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgShadow/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgSim/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgTerrain/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgWidget/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgPresentation/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgWrappers/serializers/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgPlugins/cmake_install.cmake")
  include("C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgWrappers/deprecated-dotosg/cmake_install.cmake")

endif()

