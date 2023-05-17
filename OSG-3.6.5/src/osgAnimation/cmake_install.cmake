# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgAnimation

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgAnimationd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgAnimation.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgAnimationd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgAnimation.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgAnimationrd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgAnimationd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgAnimation" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Action"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/ActionAnimation"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/ActionBlendIn"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/ActionBlendOut"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/ActionCallback"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/ActionStripAnimation"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/ActionVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Animation"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/AnimationManagerBase"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/AnimationUpdateCallback"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/BasicAnimationManager"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Bone"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/BoneMapVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Channel"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/CubicBezier"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/EaseMotion"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/FrameAction"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Interpolator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Keyframe"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/LinkVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/MorphGeometry"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/RigGeometry"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/RigTransform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/RigTransformHardware"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/RigTransformSoftware"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/MorphTransformHardware"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/MorphTransformSoftware"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Sampler"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Skeleton"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StackedMatrixElement"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StackedQuaternionElement"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StackedRotateAxisElement"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StackedScaleElement"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StackedTransformElement"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StackedTranslateElement"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StackedTransform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StatsVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/StatsHandler"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Target"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Timeline"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/TimelineAnimationManager"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/UpdateBone"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/UpdateMaterial"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/UpdateMatrixTransform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/UpdateUniform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/Vec3Packed"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgAnimation/VertexInfluence"
    )
endif()

