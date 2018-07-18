# Install script for directory: C:/Libraries/OSG-3.4.0_64/src/osgWidget

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgWidgetd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgWidget.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgWidgets.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgWidgetrd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgWidgetd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgWidget.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgWidgets.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgWidgetrd.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgWidget" TYPE FILE FILES
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Export"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Box"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Browser"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/PdfReader"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/VncClient"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Canvas"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/EventInterface"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Frame"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Input"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Label"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Lua"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Python"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/ScriptEngine"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/StyleInterface"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/StyleManager"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Table"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Types"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/UIObjectParent"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Util"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Version"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/ViewerEventHandlers"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Widget"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/Window"
    "C:/Libraries/OSG-3.4.0_64/include/osgWidget/WindowManager"
    )
endif()

