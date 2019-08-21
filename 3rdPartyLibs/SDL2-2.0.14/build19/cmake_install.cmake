# Install script for directory: C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/Debug/SDL2-staticd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/Release/SDL2-static.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/Debug/SDL2d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/Release/SDL2.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/Debug/SDL2d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/Release/SDL2.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/Debug/SDL2maind.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/Release/SDL2main.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2Targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2Targets.cmake"
         "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/CMakeFiles/Export/cmake/SDL2Targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2Targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2Targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/CMakeFiles/Export/cmake/SDL2Targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/CMakeFiles/Export/cmake/SDL2Targets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/CMakeFiles/Export/cmake/SDL2Targets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/SDL2Config.cmake"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/SDL2ConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_assert.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_atomic.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_audio.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_bits.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_blendmode.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_clipboard.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_android.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_iphoneos.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_macosx.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_minimal.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_os2.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_pandora.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_psp.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_windows.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_winrt.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_config_wiz.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_copying.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_cpuinfo.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_egl.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_endian.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_error.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_events.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_filesystem.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_gamecontroller.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_gesture.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_haptic.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_hints.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_joystick.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_keyboard.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_keycode.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_loadso.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_locale.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_log.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_main.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_messagebox.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_metal.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_misc.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_mouse.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_mutex.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_name.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_opengl.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_opengl_glext.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_opengles.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_opengles2.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_opengles2_gl2.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_opengles2_gl2ext.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_opengles2_gl2platform.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_opengles2_khrplatform.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_pixels.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_platform.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_power.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_quit.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_rect.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_render.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_revision.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_rwops.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_scancode.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_sensor.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_shape.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_stdinc.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_surface.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_system.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_syswm.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_assert.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_common.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_compare.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_crc32.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_font.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_fuzzer.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_harness.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_images.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_log.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_md5.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_memory.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_test_random.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_thread.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_timer.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_touch.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_types.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_version.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_video.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/SDL_vulkan.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/begin_code.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/include/close_code.h"
    "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/include/SDL_config.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
