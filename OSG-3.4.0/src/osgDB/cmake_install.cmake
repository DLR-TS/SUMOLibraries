# Install script for directory: C:/Libraries/OSG-3.4.0_64/src/osgDB

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgDBd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgDB.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgDBs.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgDBrd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgDBd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgDB.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgDBs.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgDBrd.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgDB" TYPE FILE FILES
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/DataTypes"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/StreamOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Serializer"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ObjectWrapper"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/InputStream"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/OutputStream"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Archive"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/AuthenticationMap"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Callbacks"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ClassInterface"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ConvertBase64"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ConvertUTF"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/DatabasePager"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/DatabaseRevisions"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/DotOsgWrapper"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/DynamicLibrary"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Export"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ExternalFileWriter"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/FileCache"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/FileNameUtils"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/FileUtils"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/fstream"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ImageOptions"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ImagePager"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ImageProcessor"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Input"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ObjectCache"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Output"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Options"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ParameterOutput"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/PluginQuery"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ReaderWriter"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/ReadFile"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Registry"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/SharedStateManager"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/Version"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/WriteFile"
    "C:/Libraries/OSG-3.4.0_64/include/osgDB/XmlParser"
    )
endif()

