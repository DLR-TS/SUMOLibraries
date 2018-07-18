# Install script for directory: C:/Libraries/OSG-3.4.0_64/src/osgAnimation

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgAnimationd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgAnimation.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgAnimations.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgAnimationrd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgAnimationd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgAnimation.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgAnimations.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgAnimationrd.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgAnimation" TYPE FILE FILES
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Action"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/ActionAnimation"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/ActionBlendIn"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/ActionBlendOut"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/ActionCallback"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/ActionStripAnimation"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/ActionVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Animation"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/AnimationManagerBase"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/AnimationUpdateCallback"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/BasicAnimationManager"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Bone"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/BoneMapVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Channel"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/CubicBezier"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/EaseMotion"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Export"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/FrameAction"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Interpolator"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Keyframe"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/LinkVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/MorphGeometry"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/RigGeometry"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/RigTransform"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/RigTransformHardware"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/RigTransformSoftware"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Sampler"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Skeleton"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StackedMatrixElement"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StackedQuaternionElement"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StackedRotateAxisElement"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StackedScaleElement"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StackedTransformElement"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StackedTranslateElement"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StackedTransform"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StatsVisitor"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/StatsHandler"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Target"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Timeline"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/TimelineAnimationManager"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/UpdateBone"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/UpdateMaterial"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/UpdateMatrixTransform"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/UpdateUniform"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/Vec3Packed"
    "C:/Libraries/OSG-3.4.0_64/include/osgAnimation/VertexInfluence"
    )
endif()

