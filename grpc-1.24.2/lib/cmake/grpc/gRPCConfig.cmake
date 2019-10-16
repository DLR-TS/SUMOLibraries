# Depend packages
if(NOT ZLIB_FOUND)
  find_package(ZLIB)
endif()
if(NOT Protobuf_FOUND AND NOT PROTOBUF_FOUND)
  find_package(Protobuf MODULE)
endif()
if(NOT OPENSSL_FOUND)
  find_package(OpenSSL)
endif()
if(NOT c-ares_FOUND)
  find_package(c-ares CONFIG)
endif()

# Targets
include(${CMAKE_CURRENT_LIST_DIR}/gRPCTargets.cmake)
