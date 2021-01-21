# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgUtil

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgUtild.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgUtil.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgUtild.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgUtil.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgUtilrd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgUtild.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgUtil" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/ConvertVec"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/CubeMapGenerator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/CullVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/DelaunayTriangulator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/DisplayRequirementsVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/DrawElementTypeSimplifier"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/EdgeCollector"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/GLObjectsVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/HalfWayMapGenerator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/HighlightMapGenerator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/IntersectionVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/IntersectVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/IncrementalCompileOperation"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/LineSegmentIntersector"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/MeshOptimizers"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/OperationArrayFunctor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/Optimizer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/PerlinNoise"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/PlaneIntersector"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/PolytopeIntersector"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/PositionalStateContainer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/PrintVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/RayIntersector"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/ReflectionMapGenerator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/RenderBin"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/RenderLeaf"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/RenderStage"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/ReversePrimitiveFunctor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/SceneView"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/SceneGraphBuilder"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/ShaderGen"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/Simplifier"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/SmoothingVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/StateGraph"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/Statistics"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/TangentSpaceGenerator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/Tessellator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/TransformAttributeFunctor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/TransformCallback"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/TriStripVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/UpdateVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgUtil/Version"
    )
endif()

