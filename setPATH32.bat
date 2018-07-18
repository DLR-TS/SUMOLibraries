REM Set the main libraries
setx LIBFFMPEG "%SUMO_LIBRARIES%\FFMPEG-3.4.1\bin"
setx LIBFOX "%SUMO_LIBRARIES%\fox-1.6.55\lib"
setx LIBGL2PS "%SUMO_LIBRARIES%\gl2ps-1.4.0\Release"
setx LIBPROJ_GDAL "%SUMO_LIBRARIES%\proj_gdal-1911\bin"
setx LIBXERCES "%SUMO_LIBRARIES%\xerces-c-3.2.0\bin"

REM Set the 3rd partylibraries
setx LIBPNG "%SUMO_LIBRARIES%\3rdPartyLibs\libpng-1.6.34\Release"
setx LIBPNGD "%SUMO_LIBRARIES%\3rdPartyLibs\libpng-1.6.34\Debug"
setx LIBTIFF "%SUMO_LIBRARIES%\3rdPartyLibs\tiff-4.0.9\libtiff\Release"
setx LIBTIFFD "%SUMO_LIBRARIES%\3rdPartyLibs\tiff-4.0.9\libtiff\Debug"
setx LIBBZIP2 "%SUMO_LIBRARIES%\3rdPartyLibs\bzip2-1.0.6"
setx LIBZLIB "%SUMO_LIBRARIES%\3rdPartyLibs\zlib-1.2.11\Release"
setx LIBZLIBD "%SUMO_LIBRARIES%\3rdPartyLibs\zlib-1.2.11\Debug"

REM Set PATH
setx PATH "%LIBFFMPEG%;%LIBFOX%;%LIBGL2PS%;%LIBPROJ_GDAL%;%LIBXERCES%;%LIBOSG%;%LIBPNG%;%LIBPNGD%;%LIBTIFF%;%LIBTIFFD%;%LIBBZIP2%;%LIBZLIB%;%LIBZLIBD%;%LIBGRPC%;%LIBGRPCD%;%LIBSWIG%;%LIBGTEST%;%LIBGTESTD%"

pause