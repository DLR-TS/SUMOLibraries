# SUMOLibraries
A repository with current SUMO Libraries used in Windows compatibles with Visual Studio 2017. Contains:

* FFMPEG-3.4.1
* OSG-3.4.0
* fox-1.6.55
* gl2ps-1.4.0
* googletest-1.8.0
* grpc-1.10.0
* OSG-3.4.0
* proj_gdal-1911
* swigwin-3.0.12
* xerces-c-3.2.0

And the third party libs:

* bzip2-1.0.6
* freetype-2.9
* jpeg-9c
* libpng-1.6.34
* tiff-4.0.9
* zlib-1.2.11

# How to configure

* 1 - Define enviroment variable SUMO_LIBRARIES with the directory in which this repository was cloned (For Example: "c:\SUMOLibraries)

* 2 - For 32 bits version, add the following line to PATH

%SUMO_LIBRARIES%\fox-1.6.55\lib;%SUMO_LIBRARIES%\gl2ps-1.4.0\Release;%SUMO_LIBRARIES%\proj_gdal-1911\bin;%SUMO_LIBRARIES%\xerces-c-3.2.0\bin;%SUMO_LIBRARIES%\3rdPartyLibs\libpng-1.6.34\Release;%SUMO_LIBRARIES%\3rdPartyLibs\libpng-1.6.34\Debug;%SUMO_LIBRARIES%\3rdPartyLibs\tiff-4.0.9\libtiff\Release;%SUMO_LIBRARIES%\3rdPartyLibs\tiff-4.0.9\libtiff\Debug;%SUMO_LIBRARIES%\3rdPartyLibs\bzip2-1.0.6;%SUMO_LIBRARIES%\3rdPartyLibs\zlib-1.2.11\Release;%SUMO_LIBRARIES%\3rdPartyLibs\zlib-1.2.11\Debug

* 3 - for 64 bits versions, add the following line to PATH

%SUMO_LIBRARIES%\FFMPEG-3.4.1_64\bin;%SUMO_LIBRARIES%\fox-1.6.55_64\lib;%SUMO_LIBRARIES%\gl2ps-1.4.0_64\Release;%SUMO_LIBRARIES%\proj_gdal-1911_64\bin;%SUMO_LIBRARIES%\xerces-c-3.2.0_64\bin;%SUMO_LIBRARIES%\OSG-3.4.0_64\bin;%SUMO_LIBRARIES%\3rdPartyLibs\libpng-1.6.34_64\Release;%SUMO_LIBRARIES%\3rdPartyLibs\libpng-1.6.34_64\Debug;%SUMO_LIBRARIES%\3rdPartyLibs\tiff-4.0.9_64\libtiff\Release;%SUMO_LIBRARIES%\3rdPartyLibs\tiff-4.0.9_64\libtiff\Debug;%SUMO_LIBRARIES%\3rdPartyLibs\bzip2-1.0.6_64;%SUMO_LIBRARIES%\3rdPartyLibs\zlib-1.2.11_64\Release;%SUMO_LIBRARIES%\3rdPartyLibs\zlib-1.2.11_64\Debug;%SUMO_LIBRARIES%\grpc-1.10.0_64\Release;%SUMO_LIBRARIES%\grpc-1.10.0_64\Debug;%SUMO_LIBRARIES%\swigwin-3.0.12;%SUMO_LIBRARIES%\3rdPartyLibs\protobuf-3.5.1_64\cmake\Release;%SUMO_LIBRARIES%\3rdPartyLibs\protobuf-3.5.1_64\cmake\Debug
