# Install script for directory: C:/SUMOLibraries/32bits/proj-6.1.1/data

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:\Libraries\32bits\proj-6.1.1")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/proj" TYPE FILE FILES
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/null"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/world"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/other.extra"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/nad27"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/GL27"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/nad83"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/nad.lst"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/CH"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/ITRF2000"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/ITRF2008"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/ITRF2014"
    "C:/SUMOLibraries/32bits/proj-6.1.1/data/proj.db"
    )
endif()

