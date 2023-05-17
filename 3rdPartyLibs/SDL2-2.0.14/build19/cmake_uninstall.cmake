if (NOT EXISTS "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/install_manifest.txt\"")
endif(NOT EXISTS "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/install_manifest.txt")

file(READ "C:/Librerias/SUMOLibraries/3rdPartyLibs/SDL2-2.0.14/build19/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach (file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    execute_process(
        COMMAND C:/CMake/bin/cmake.exe -E remove "$ENV{DESTDIR}${file}"
        OUTPUT_VARIABLE rm_out
        RESULT_VARIABLE rm_retval
    )
    if(NOT ${rm_retval} EQUAL 0)
        message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif (NOT ${rm_retval} EQUAL 0)
endforeach(file)

