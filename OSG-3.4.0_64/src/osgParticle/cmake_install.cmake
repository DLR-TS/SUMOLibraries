# Install script for directory: C:/Libraries/OSG-3.4.0_64/src/osgParticle

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgParticled.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgParticle.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgParticles.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Libraries/OSG-3.4.0_64/lib/osgParticlerd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgParticled.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgParticle.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgParticles.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Libraries/OSG-3.4.0_64/bin/osg130-osgParticlerd.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgParticle" TYPE FILE FILES
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/AccelOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/AngularAccelOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/BoxPlacer"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/CenteredPlacer"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ConnectedParticleSystem"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ConstantRateCounter"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Counter"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Emitter"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ExplosionDebrisEffect"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ExplosionEffect"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Export"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/FireEffect"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/FluidFrictionOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/FluidProgram"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ForceOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Interpolator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/LinearInterpolator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ModularEmitter"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ModularProgram"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/MultiSegmentPlacer"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Operator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Particle"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ParticleEffect"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ParticleProcessor"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ParticleSystem"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ParticleSystemUpdater"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Placer"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/PointPlacer"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/PrecipitationEffect"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Program"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/RadialShooter"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/RandomRateCounter"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/range"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/SectorPlacer"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/SegmentPlacer"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Shooter"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/SmokeEffect"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/SmokeTrailEffect"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/VariableRateCounter"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/Version"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/CompositePlacer"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/AngularDampingOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/DampingOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/ExplosionOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/OrbitOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/DomainOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/BounceOperator"
    "C:/Libraries/OSG-3.4.0_64/include/osgParticle/SinkOperator"
    )
endif()

