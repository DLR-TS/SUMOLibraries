# Install script for directory: C:/Libraries/OSG-3.4.0_64/src/osgGA

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgGAd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgGA.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgGAs.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgGArd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgGAd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgGA.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgGAs.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgGArd.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgGA" TYPE FILE FILES
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/AnimationPathManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/CameraViewSwitchManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/DriveManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/Device"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/Event"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/EventHandler"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/EventQueue"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/EventVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/Export"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/FirstPersonManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/FlightManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/GUIActionAdapter"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/GUIEventAdapter"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/GUIEventHandler"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/KeySwitchMatrixManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/CameraManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/MultiTouchTrackballManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/NodeTrackerManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/OrbitManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/StandardManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/SphericalManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/StateSetManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/TerrainManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/TrackballManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/UFOManipulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/Widget"
    "C:/Libraries/OSG-3.4.0_64/include/osgGA/Version"
    )
endif()

