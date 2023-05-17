# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgGA

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgGAd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgGA.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgGAd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgGA.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgGArd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgGAd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgGA" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/AnimationPathManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/CameraViewSwitchManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/DriveManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/Device"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/Event"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/EventHandler"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/EventQueue"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/EventVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/FirstPersonManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/FlightManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/GUIActionAdapter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/GUIEventAdapter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/GUIEventHandler"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/KeySwitchMatrixManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/CameraManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/MultiTouchTrackballManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/NodeTrackerManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/OrbitManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/StandardManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/SphericalManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/StateSetManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/TerrainManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/TrackballManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/UFOManipulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/Widget"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgGA/Version"
    )
endif()

