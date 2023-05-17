# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgFX

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgFXd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgFX.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgFXd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgFX.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgFXrd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgFXd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgFX" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/AnisotropicLighting"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/BumpMapping"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Cartoon"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Effect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/MultiTextureControl"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Outline"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Registry"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Scribe"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/SpecularHighlights"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Technique"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Validator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgFX/Version"
    )
endif()

