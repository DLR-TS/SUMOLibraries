#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Libssh2::libssh2" for configuration "Debug"
set_property(TARGET Libssh2::libssh2 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Libssh2::libssh2 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;RC"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "C:/Librerias/SUMOLibraries/3rdPartyLibs/openssl-1.1.1/libssl.lib;C:/Librerias/SUMOLibraries/3rdPartyLibs/openssl-1.1.1/libcrypto.lib;crypt32;C:/Librerias/SUMOLibraries/3rdPartyLibs/zlib-1.2.11/lib/zlibd.lib;ws2_32"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libssh2.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Libssh2::libssh2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Libssh2::libssh2 "${_IMPORT_PREFIX}/lib/libssh2.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
