# Install script for directory: C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/tiff")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/tiff/share/man/man1/fax2ps.1;C:/Program Files/tiff/share/man/man1/fax2tiff.1;C:/Program Files/tiff/share/man/man1/pal2rgb.1;C:/Program Files/tiff/share/man/man1/ppm2tiff.1;C:/Program Files/tiff/share/man/man1/raw2tiff.1;C:/Program Files/tiff/share/man/man1/tiff2bw.1;C:/Program Files/tiff/share/man/man1/tiff2pdf.1;C:/Program Files/tiff/share/man/man1/tiff2ps.1;C:/Program Files/tiff/share/man/man1/tiff2rgba.1;C:/Program Files/tiff/share/man/man1/tiffcmp.1;C:/Program Files/tiff/share/man/man1/tiffcp.1;C:/Program Files/tiff/share/man/man1/tiffcrop.1;C:/Program Files/tiff/share/man/man1/tiffdither.1;C:/Program Files/tiff/share/man/man1/tiffdump.1;C:/Program Files/tiff/share/man/man1/tiffgt.1;C:/Program Files/tiff/share/man/man1/tiffinfo.1;C:/Program Files/tiff/share/man/man1/tiffmedian.1;C:/Program Files/tiff/share/man/man1/tiffset.1;C:/Program Files/tiff/share/man/man1/tiffsplit.1")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files/tiff/share/man/man1" TYPE FILE FILES
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/fax2ps.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/fax2tiff.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/pal2rgb.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/ppm2tiff.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/raw2tiff.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiff2bw.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiff2pdf.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiff2ps.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiff2rgba.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffcmp.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffcp.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffcrop.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffdither.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffdump.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffgt.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffinfo.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffmedian.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffset.1"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/tiffsplit.1"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/tiff/share/man/man3/libtiff.3tiff;C:/Program Files/tiff/share/man/man3/TIFFbuffer.3tiff;C:/Program Files/tiff/share/man/man3/TIFFClose.3tiff;C:/Program Files/tiff/share/man/man3/TIFFcodec.3tiff;C:/Program Files/tiff/share/man/man3/TIFFcolor.3tiff;C:/Program Files/tiff/share/man/man3/TIFFDataWidth.3tiff;C:/Program Files/tiff/share/man/man3/TIFFError.3tiff;C:/Program Files/tiff/share/man/man3/TIFFFieldDataType.3tiff;C:/Program Files/tiff/share/man/man3/TIFFFieldName.3tiff;C:/Program Files/tiff/share/man/man3/TIFFFieldPassCount.3tiff;C:/Program Files/tiff/share/man/man3/TIFFFieldReadCount.3tiff;C:/Program Files/tiff/share/man/man3/TIFFFieldTag.3tiff;C:/Program Files/tiff/share/man/man3/TIFFFieldWriteCount.3tiff;C:/Program Files/tiff/share/man/man3/TIFFFlush.3tiff;C:/Program Files/tiff/share/man/man3/TIFFGetField.3tiff;C:/Program Files/tiff/share/man/man3/TIFFmemory.3tiff;C:/Program Files/tiff/share/man/man3/TIFFOpen.3tiff;C:/Program Files/tiff/share/man/man3/TIFFPrintDirectory.3tiff;C:/Program Files/tiff/share/man/man3/TIFFquery.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadDirectory.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadEncodedStrip.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadEncodedTile.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadRawStrip.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadRawTile.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadRGBAImage.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadRGBAStrip.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadRGBATile.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadScanline.3tiff;C:/Program Files/tiff/share/man/man3/TIFFReadTile.3tiff;C:/Program Files/tiff/share/man/man3/TIFFRGBAImage.3tiff;C:/Program Files/tiff/share/man/man3/TIFFSetDirectory.3tiff;C:/Program Files/tiff/share/man/man3/TIFFSetField.3tiff;C:/Program Files/tiff/share/man/man3/TIFFsize.3tiff;C:/Program Files/tiff/share/man/man3/TIFFstrip.3tiff;C:/Program Files/tiff/share/man/man3/TIFFswab.3tiff;C:/Program Files/tiff/share/man/man3/TIFFtile.3tiff;C:/Program Files/tiff/share/man/man3/TIFFWarning.3tiff;C:/Program Files/tiff/share/man/man3/TIFFWriteDirectory.3tiff;C:/Program Files/tiff/share/man/man3/TIFFWriteEncodedStrip.3tiff;C:/Program Files/tiff/share/man/man3/TIFFWriteEncodedTile.3tiff;C:/Program Files/tiff/share/man/man3/TIFFWriteRawStrip.3tiff;C:/Program Files/tiff/share/man/man3/TIFFWriteRawTile.3tiff;C:/Program Files/tiff/share/man/man3/TIFFWriteScanline.3tiff;C:/Program Files/tiff/share/man/man3/TIFFWriteTile.3tiff")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files/tiff/share/man/man3" TYPE FILE FILES
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/libtiff.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFbuffer.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFClose.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFcodec.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFcolor.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFDataWidth.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFError.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFFieldDataType.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFFieldName.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFFieldPassCount.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFFieldReadCount.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFFieldTag.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFFieldWriteCount.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFFlush.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFGetField.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFmemory.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFOpen.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFPrintDirectory.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFquery.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadDirectory.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadEncodedStrip.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadEncodedTile.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadRawStrip.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadRawTile.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadRGBAImage.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadRGBAStrip.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadRGBATile.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadScanline.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFReadTile.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFRGBAImage.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFSetDirectory.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFSetField.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFsize.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFstrip.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFswab.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFtile.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFWarning.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFWriteDirectory.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFWriteEncodedStrip.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFWriteEncodedTile.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFWriteRawStrip.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFWriteRawTile.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFWriteScanline.3tiff"
    "C:/Libraries/3rdPartyLibs/tiff-4.0.9_64/man/TIFFWriteTile.3tiff"
    )
endif()

