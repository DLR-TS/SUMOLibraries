
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was minizipConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set(_MINIZIP_supported_components "shared" "static")
include(CMakeFindDependencyMacro)

if(ON)
    find_dependency(BZip2)
endif(ON)

if(minizip_FIND_COMPONENTS)
    find_dependency(ZLIB CONFIG COMPONENTS ${minizip_FIND_COMPONENTS})

    foreach(_comp ${minizip_FIND_COMPONENTS})
        if(NOT _comp IN_LIST _MINIZIP_supported_components)
            set(minizip_FOUND False)
            set(minizip_NOT_FOUND_MESSAGE "Unsupported component: ${_comp}")
        endif(NOT _comp IN_LIST _MINIZIP_supported_components)

        include("${CMAKE_CURRENT_LIST_DIR}/minizip-${_comp}.cmake")
    endforeach(_comp ${ZLIB_FIND_COMPONENTS})
else(minizip_FIND_COMPONENTS)
    find_dependency(ZLIB CONFIG)

    foreach(_component_config IN LISTS _MINIZIP_supported_components)
        include("${CMAKE_CURRENT_LIST_DIR}/minizip-${_component_config}.cmake")
    endforeach(_component_config IN LISTS _MINIZIP_supported_components)
endif(minizip_FIND_COMPONENTS)
