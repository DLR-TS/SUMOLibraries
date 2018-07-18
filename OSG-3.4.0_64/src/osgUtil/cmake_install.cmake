# Install script for directory: C:/Libraries/OSG-3.4.0_64/src/osgUtil

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgUtild.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgUtil.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgUtils.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgUtilrd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgUtild.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgUtil.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgUtils.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgUtilrd.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgUtil" TYPE FILE FILES
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/ConvertVec"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/CubeMapGenerator"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/CullVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/DelaunayTriangulator"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/DisplayRequirementsVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/DrawElementTypeSimplifier"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/EdgeCollector"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/Export"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/GLObjectsVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/HalfWayMapGenerator"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/HighlightMapGenerator"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/IntersectionVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/IntersectVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/IncrementalCompileOperation"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/LineSegmentIntersector"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/MeshOptimizers"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/OperationArrayFunctor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/Optimizer"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/PerlinNoise"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/PlaneIntersector"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/PolytopeIntersector"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/PositionalStateContainer"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/PrintVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/RayIntersector"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/ReflectionMapGenerator"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/RenderBin"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/RenderLeaf"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/RenderStage"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/ReversePrimitiveFunctor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/SceneView"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/SceneGraphBuilder"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/ShaderGen"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/Simplifier"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/SmoothingVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/StateGraph"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/Statistics"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/TangentSpaceGenerator"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/Tessellator"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/TransformAttributeFunctor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/TransformCallback"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/TriStripVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/UpdateVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgUtil/Version"
    )
endif()

