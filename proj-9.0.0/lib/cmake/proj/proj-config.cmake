# Configure PROJ
#
# Set
#  PROJ_FOUND = 1
#  PROJ_INCLUDE_DIRS = /usr/local/include
#  PROJ_LIBRARIES = PROJ::proj
#  PROJ_LIBRARY_DIRS = /usr/local/lib
#  PROJ_BINARY_DIRS = /usr/local/bin
#  PROJ_VERSION = 4.9.1 (for example)

# Tell the user project where to find our headers and libraries
get_filename_component (_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
get_filename_component (_ROOT "${_DIR}/../../../" ABSOLUTE)
set (PROJ_INCLUDE_DIRS "${_ROOT}/include")
set (PROJ_LIBRARY_DIRS "${_ROOT}/lib")
set (PROJ_BINARY_DIRS "${_ROOT}/bin")

set (PROJ_LIBRARIES PROJ::proj)
# Read in the exported definition of the library
include ("${_DIR}/proj-targets.cmake")
include ("${_DIR}/proj4-targets.cmake")

unset (_ROOT)
unset (_DIR)

if ("PROJ" STREQUAL "PROJ4")
  # For backward compatibility with old releases of libgeotiff
  set (PROJ_INCLUDE_DIR
    ${PROJ_INCLUDE_DIRS})
endif ()
