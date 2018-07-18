# Install script for directory: C:/Libraries/OSG-3.4.0_64/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/OpenSceneGraph")
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
  include("C:/Libraries/OSG-3.4.0_64/src/OpenThreads/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osg/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgDB/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgUtil/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgGA/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgText/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgViewer/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgAnimation/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgFX/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgManipulator/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgParticle/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgPresentation/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgShadow/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgSim/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgTerrain/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgWidget/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgUI/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgVolume/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgWrappers/serializers/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgWrappers/deprecated-dotosg/cmake_install.cmake")
  include("C:/Libraries/OSG-3.4.0_64/src/osgPlugins/cmake_install.cmake")

endif()

