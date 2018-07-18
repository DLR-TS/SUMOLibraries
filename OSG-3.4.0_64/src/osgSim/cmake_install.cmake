# Install script for directory: C:/Libraries/OSG-3.4.0_64/src/osgSim

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgSimd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgSim.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgSims.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgSimrd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgSimd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgSim.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgSims.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgSimrd.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgSim" TYPE FILE FILES
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/BlinkSequence"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/ColorRange"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/DOFTransform"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/ElevationSlice"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/HeightAboveTerrain"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/Export"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/GeographicLocation"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/Impostor"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/ImpostorSprite"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/InsertImpostorsVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/LightPoint"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/LightPointNode"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/LightPointSystem"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/LineOfSight"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/MultiSwitch"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/OverlayNode"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/ObjectRecordData"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/ScalarBar"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/ScalarsToColors"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/Sector"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/ShapeAttribute"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/SphereSegment"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/Version"
    "C:/Libraries/OSG-3.4.0_64/include/osgSim/VisibilityGroup"
    )
endif()

