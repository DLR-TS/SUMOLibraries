#  Copyright (C) 1999-2023 Markus Rickert.
#  This file is part of the GNU LIBICONV Library.
#
#  The GNU LIBICONV Library is free software; you can redistribute it
#  and/or modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  The GNU LIBICONV Library is distributed in the hope that it will be
#  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with the GNU LIBICONV Library; see the file COPYING.LIB.
#  If not, see <https://www.gnu.org/licenses/>.  */

set(Iconv_VERSION "1.17")
set(Iconv_VERSION_MAJOR "1")
set(Iconv_VERSION_MINOR "17")

set(Charset_VERSION "1.5")
set(Charset_VERSION_MAJOR "1")
set(Charset_VERSION_MINOR "5")


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was iconv-config.cmake.in                            ########

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

include("${CMAKE_CURRENT_LIST_DIR}/iconv-export.cmake")

set(Iconv_DEFINITIONS "")
set(Iconv_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include")
set(Iconv_LIBRARY_DIRS "${PACKAGE_PREFIX_DIR}/lib")

set(Charset_DEFINITIONS "")
set(Charset_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include")
set(Charset_LIBRARY_DIRS "${PACKAGE_PREFIX_DIR}/lib")

macro(select_library_location target basename)
	foreach(property IN ITEMS IMPORTED_LOCATION IMPORTED_IMPLIB)
		get_target_property(${basename}_${property}_DEBUG ${target} ${property}_DEBUG)
		get_target_property(${basename}_${property}_MINSIZEREL ${target} ${property}_MINSIZEREL)
		get_target_property(${basename}_${property}_NOCONFIG ${target} ${property}_NOCONFIG)
		get_target_property(${basename}_${property}_RELEASE ${target} ${property}_RELEASE)
		get_target_property(${basename}_${property}_RELWITHDEBINFO ${target} ${property}_RELWITHDEBINFO)
		
		if(${basename}_${property}_DEBUG AND ${basename}_${property}_RELEASE)
			set(${basename}_LIBRARIES debug ${${basename}_${property}_DEBUG} optimized ${${basename}_${property}_RELEASE})
		elseif(${basename}_${property}_DEBUG AND ${basename}_${property}_RELWITHDEBINFO)
			set(${basename}_LIBRARIES debug ${${basename}_${property}_DEBUG} optimized ${${basename}_${property}_RELWITHDEBINFO})
		elseif(${basename}_${property}_DEBUG AND ${basename}_${property}_MINSIZEREL)
			set(${basename}_LIBRARIES debug ${${basename}_${property}_DEBUG} optimized ${${basename}_${property}_MINSIZEREL})
		elseif(${basename}_${property}_RELEASE)
			set(${basename}_LIBRARIES ${${basename}_${property}_RELEASE})
		elseif(${basename}_${property}_RELWITHDEBINFO)
			set(${basename}_LIBRARIES ${${basename}_${property}_RELWITHDEBINFO})
		elseif(${basename}_${property}_MINSIZEREL)
			set(${basename}_LIBRARIES ${${basename}_${property}_MINSIZEREL})
		elseif(${basename}_${property}_DEBUG)
			set(${basename}_LIBRARIES ${${basename}_${property}_DEBUG})
		elseif(${basename}_${property}_NOCONFIG)
			set(${basename}_LIBRARIES ${${basename}_${property}_NOCONFIG})
		endif()
	endforeach()
endmacro()

select_library_location(Iconv::Iconv Iconv)
select_library_location(Iconv::Charset Charset)
