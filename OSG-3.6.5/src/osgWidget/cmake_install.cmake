# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgWidget

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgWidgetd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgWidget.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgWidgetd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgWidget.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgWidgetrd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgWidgetd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgWidget" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Box"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Browser"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/PdfReader"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/VncClient"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Canvas"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/EventInterface"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Frame"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Input"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Label"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Lua"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Python"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/ScriptEngine"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/StyleInterface"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/StyleManager"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Table"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Types"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/UIObjectParent"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Util"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Version"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/ViewerEventHandlers"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Widget"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/Window"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgWidget/WindowManager"
    )
endif()

