# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgTerrain

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgTerraind.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgTerrain.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgTerraind.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgTerrain.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgTerrainrd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgTerraind.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgTerrain" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/DisplacementMappingTechnique"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/Locator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/Layer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/TerrainTile"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/TerrainTechnique"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/Terrain"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/GeometryTechnique"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/GeometryPool"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/ValidDataOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgTerrain/Version"
    )
endif()

