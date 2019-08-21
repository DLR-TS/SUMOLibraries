# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgDB

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgDBd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgDB.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgDBd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgDB.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgDBrd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgDBd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgDB" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/DataTypes"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/StreamOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Serializer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ObjectWrapper"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/InputStream"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/OutputStream"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Archive"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/AuthenticationMap"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Callbacks"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ClassInterface"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ConvertBase64"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ConvertUTF"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/DatabasePager"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/DatabaseRevisions"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/DotOsgWrapper"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/DynamicLibrary"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ExternalFileWriter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/FileCache"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/FileNameUtils"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/FileUtils"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/fstream"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ImageOptions"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ImagePager"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ImageProcessor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Input"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ObjectCache"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Output"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Options"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ParameterOutput"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/PluginQuery"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ReaderWriter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/ReadFile"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Registry"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/SharedStateManager"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/Version"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/WriteFile"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgDB/XmlParser"
    )
endif()

