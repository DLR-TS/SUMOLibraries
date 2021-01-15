#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Libssh2::libssh2" for configuration "Release"
set_property(TARGET Libssh2::libssh2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Libssh2::libssh2 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;RC"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "C:/Librerias/SUMOLibraries/3rdPartyLibs/openssl-1.1.1/libssl.lib;C:/Librerias/SUMOLibraries/3rdPartyLibs/openssl-1.1.1/libcrypto.lib;crypt32;C:/Librerias/SUMOLibraries/3rdPartyLibs/zlib-1.2.11/lib/zlib.lib;ws2_32"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libssh2.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Libssh2::libssh2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Libssh2::libssh2 "${_IMPORT_PREFIX}/lib/libssh2.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
