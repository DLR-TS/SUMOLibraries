# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osgParticle

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgParticled.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgParticle.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgParticled.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgParticle.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgParticlerd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgParticled.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgParticle" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/AccelOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/AngularAccelOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/BoxPlacer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/CenteredPlacer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ConnectedParticleSystem"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ConstantRateCounter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Counter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Emitter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ExplosionDebrisEffect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ExplosionEffect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/FireEffect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/FluidFrictionOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/FluidProgram"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ForceOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Interpolator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/LinearInterpolator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ModularEmitter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ModularProgram"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/MultiSegmentPlacer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Operator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Particle"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ParticleEffect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ParticleProcessor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ParticleSystem"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ParticleSystemUpdater"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Placer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/PointPlacer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/PrecipitationEffect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Program"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/RadialShooter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/RandomRateCounter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/range"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/SectorPlacer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/SegmentPlacer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Shooter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/SmokeEffect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/SmokeTrailEffect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/VariableRateCounter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/Version"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/CompositePlacer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/AngularDampingOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/DampingOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/ExplosionOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/OrbitOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/DomainOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/BounceOperator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osgParticle/SinkOperator"
    )
endif()

