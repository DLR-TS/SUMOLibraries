REM Set the main libraries
setx LIBFFMPEG "%SUMO_LIBRARIES%\FFMPEG-3.4.1_64\bin"
setx LIBFOX "%SUMO_LIBRARIES%\fox-1.6.55_64\lib"
setx LIBGL2PS "%SUMO_LIBRARIES%\gl2ps-1.4.0_64\Release"
setx LIBPROJ_GDAL "%SUMO_LIBRARIES%\proj_gdal-1911_64\bin"
setx LIBXERCES "%SUMO_LIBRARIES%\xerces-c-3.2.0_64\bin"
setx LIBOSG "%SUMO_LIBRARIES%\OSG-3.4.0_64\bin"
setx LIBGRPC "%SUMO_LIBRARIES%\grpc-1.10.0_64\Release"
setx LIBGRPCD "%SUMO_LIBRARIES%\grpc-1.10.0_64\Debug"
setx LIBSWIG "%SUMO_LIBRARIES%\swigwin-3.0.12"
setx LIBGTEST "%SUMO_LIBRARIES%\googletest-1.8.0_64\googlemock\gtest\Release"
setx LIBGTESTD "%SUMO_LIBRARIES%\googletest-1.8.0_64\googlemock\gtest\Debug"

REM Set the 3rd partylibraries
setx LIBPNG "%SUMO_LIBRARIES%\3rdPartyLibs\libpng-1.6.34_64\Release"
setx LIBPNGD "%SUMO_LIBRARIES%\3rdPartyLibs\libpng-1.6.34_64\Debug"
setx LIBTIFF "%SUMO_LIBRARIES%\3rdPartyLibs\tiff-4.0.9_64\libtiff\Release"
setx LIBTIFFD "%SUMO_LIBRARIES%\3rdPartyLibs\tiff-4.0.9_64\libtiff\Debug"
setx LIBBZIP2 "%SUMO_LIBRARIES%\3rdPartyLibs\bzip2-1.0.6_64"
setx LIBZLIB "%SUMO_LIBRARIES%\3rdPartyLibs\zlib-1.2.11_64\Release"
setx LIBZLIBD "%SUMO_LIBRARIES%\3rdPartyLibs\zlib-1.2.11_64\Debug"
setx LIBPROTOBUF "%SUMO_LIBRARIES%\3rdPartyLibs\protobuf-3.5.1_64\cmake\Release"
setx LIBPROTOBUFD "%SUMO_LIBRARIES%\3rdPartyLibs\protobuf-3.5.1_64\cmake\Debug"

REM Set PATH
setx PATH "%LIBFFMPEG%;%LIBFOX%;%LIBGL2PS%;%LIBPROJ_GDAL%;%LIBXERCES%;%LIBOSG%;%LIBPNG%;%LIBPNGD%;%LIBTIFF%;%LIBTIFFD%;%LIBBZIP2%;%LIBZLIB%;%LIBZLIBD%;%LIBGRPC%;%LIBGRPCD%;%LIBSWIG%;%LIBGTEST%;%LIBGTESTD%;%LIBPROTOBUF%;%LIBPROTOBUFD%"

pause