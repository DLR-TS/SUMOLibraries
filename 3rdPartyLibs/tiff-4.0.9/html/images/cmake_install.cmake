# Install script for directory: C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:\Libraries\3rdPartyLibs\tiff-4.0.9")
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
   "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/back.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/bali.jpg;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/cat.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/cover.jpg;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/cramps.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/dave.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/info.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/jello.jpg;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/jim.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/note.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/oxford.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/quad.jpg;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/ring.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/smallliz.jpg;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/strike.gif;C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images/warning.gif")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/share/doc/tiff/html/images" TYPE FILE FILES
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/back.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/bali.jpg"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/cat.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/cover.jpg"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/cramps.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/dave.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/info.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/jello.jpg"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/jim.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/note.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/oxford.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/quad.jpg"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/ring.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/smallliz.jpg"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/strike.gif"
    "C:/SUMOLibraries/3rdPartyLibs/tiff-4.0.9/html/images/warning.gif"
    )
endif()

