# Install script for directory: C:/Libraries/OSG-3.4.0_64/src/osgManipulator

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgManipulatord.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgManipulator.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgManipulators.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgManipulatorrd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgManipulatord.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgManipulator.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgManipulators.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgManipulatorrd.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgManipulator" TYPE FILE FILES
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/AntiSquish"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Command"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/CommandManager"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Constraint"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Dragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Export"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Projector"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/RotateCylinderDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/RotateSphereDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Scale1DDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Scale2DDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/ScaleAxisDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Selection"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/TabBoxDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/TabBoxTrackballDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/TabPlaneDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/TabPlaneTrackballDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/TrackballDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Translate1DDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Translate2DDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/TranslateAxisDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/TranslatePlaneDragger"
    "C:/Libraries/OSG-3.4.0_64/include/osgManipulator/Version"
    )
endif()

