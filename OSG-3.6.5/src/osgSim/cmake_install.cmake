# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgSim

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgSimd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgSim.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgSimd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgSim.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgSimrd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgSimd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgSim" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/BlinkSequence"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/ColorRange"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/DOFTransform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/ElevationSlice"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/HeightAboveTerrain"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/GeographicLocation"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/Impostor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/ImpostorSprite"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/InsertImpostorsVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/LightPoint"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/LightPointNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/LightPointSystem"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/LineOfSight"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/MultiSwitch"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/OverlayNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/ObjectRecordData"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/ScalarBar"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/ScalarsToColors"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/Sector"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/ShapeAttribute"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/SphereSegment"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/Version"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgSim/VisibilityGroup"
    )
endif()

