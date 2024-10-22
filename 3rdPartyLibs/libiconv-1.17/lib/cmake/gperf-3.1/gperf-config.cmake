set(GPERF_VERSION "3.1")
set(GPERF_VERSION_MAJOR "3")
set(GPERF_VERSION_MINOR "1")


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was gperf-config.cmake.in                            ########

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

include("${CMAKE_CURRENT_LIST_DIR}/gperf-export.cmake")

macro(select_executable_location target basename)
	foreach(property IN ITEMS IMPORTED_LOCATION)
		get_target_property(${basename}_${property}_DEBUG ${target} ${property}_DEBUG)
		get_target_property(${basename}_${property}_MINSIZEREL ${target} ${property}_MINSIZEREL)
		get_target_property(${basename}_${property}_NOCONFIG ${target} ${property}_NOCONFIG)
		get_target_property(${basename}_${property}_RELEASE ${target} ${property}_RELEASE)
		get_target_property(${basename}_${property}_RELWITHDEBINFO ${target} ${property}_RELWITHDEBINFO)
		
		if(${basename}_${property}_RELEASE)
			set(${basename}_EXECUTABLE ${${basename}_${property}_RELEASE})
		elseif(${basename}_${property}_RELWITHDEBINFO)
			set(${basename}_EXECUTABLE ${${basename}_${property}_RELWITHDEBINFO})
		elseif(${basename}_${property}_MINSIZEREL)
			set(${basename}_EXECUTABLE ${${basename}_${property}_MINSIZEREL})
		elseif(${basename}_${property}_DEBUG)
			set(${basename}_EXECUTABLE ${${basename}_${property}_DEBUG})
		elseif(${basename}_${property}_NOCONFIG)
			set(${basename}_EXECUTABLE ${${basename}_${property}_NOCONFIG})
		endif()
	endforeach()
endmacro()

select_executable_location(gperf::gperf GPERF)
