# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/OpenThreads/win32

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenthreads-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/OpenThreadsd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/OpenThreads.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenthreadsx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/ot21-OpenThreadsd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/ot21-OpenThreads.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenthreads-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenThreads" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Atomic"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Affinity"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Barrier"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Block"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Condition"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Exports"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Mutex"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/ReadWriteMutex"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/ReentrantMutex"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/ScopedLock"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Thread"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Version"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/OpenThreads/Config"
    )
endif()

