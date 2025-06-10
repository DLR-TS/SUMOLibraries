# SUMOLibraries
This repository contains the current SUMO Libraries used in Windows and compatible with Visual Studio 2019. It includes:

| Library       | License                      | Link                                            |
|---------------|------------------------------|-------------------------------------------------|
| apache-maven  | Apache License 2.0           | <https://maven.apache.org/>                     |
| eigen         | MPL 2.0                      | <https://eigen.tuxfamily.org/>                  |
| FFMPEG        | LGPL 2.1 / GPL 2.0           | <https://ffmpeg.org/>                           |
| fmt           | MIT                          | <https://fmt.dev/>                              |
| fox           | LGPL 2.1 + Addendum          | <http://www.fox-toolkit.org/>                   |
| gdal          | MIT                          | <https://gdal.org/>                             |
| geos          | LGPL 2.1                     | <https://libgeos.org/>                          |
| gettext       | GPL 3.0                      | <https://www.gnu.org/software/gettext/>         |
| gl2ps         | LGPL 2.0 / GL2PS             | <https://geuz.org/gl2ps/>                       |
| glew          | BSD-3-Clause / MIT           | <http://glew.sourceforge.net/>                  |
| glm           | MIT                          | <https://github.com/g-truc/glm>                 |
| googletest    | BSD-3-Clause                 | <https://github.com/google/googletest>          |
| jupedsim      | LGPL 3.0                     | <https://jupedsim.org/>                         |
| libspatialite | MPL 1.1 / GPL 2.0 / LGPL 2.1 | <https://www.gaia-gis.it/fossil/libspatialite/> |
| OSG           | OSGPL 1.0                    | <https://www.openscenegraph.org/>               |
| proj          | MIT                          | <https://proj.org/>                             |
| sccache       | Apache License 2.0           | <https://github.com/mozilla/sccache>            |
| swig          | GPL 3.0                      | <http://www.swig.org/>                          |
| TextTest      | LGPL 2.1                     | <https://texttest.org/>                         |
| xerces-c      | Apache License 2.0           | <https://xerces.apache.org/xerces-c/>           |


And other auxiliary third-party libraries:

| Library    | License                                 | Link                                           |
|------------|-----------------------------------------|------------------------------------------------|
| bzip2      | BSD-style license                       | <https://sourceware.org/bzip2/>                |
| curl       | curl (MIT-like license)                 | <https://curl.se>                              |
| freeglut   | X-Consortium License                    | <http://freeglut.sourceforge.net>              |
| freetype   | FreeType License (FTL) / GPL 2.0        | <https://www.freetype.org>                     |
| freexl     | MPL 1.1 / GPL 2.0 / LGPL 2.1            | <https://www.gaia-gis.it/fossil/freexl/index>  |
| giflib     | X-Consortium-like License               | <http://giflib.sourceforge.net>                |
| icu4c      | Unicode License V3                      | <https://icu.unicode.org>                      |
| libexpat   | MIT                                     | <https://libexpat.github.io>                   |
| libiconv   | GPL 3.0                                 | <https://www.gnu.org/software/libiconv/>       |
| libjpeg    | Independent JPEG Group (IJG) License    | <http://www.ijg.org>                           |
| libpng     | PNG Reference Library License version 2 | <http://www.libpng.org>                        |
| librttopo  | GPL 2.0 or later                        | <https://git.osgeo.org/gitea/rttopo/librttopo> |
| libssh2    | BSD-3-Clause                            | <https://www.libssh2.org>                      |
| libtiff    | Modified BSD License                    | <http://www.simplesystems.org/libtiff/>        |
| libxml2    | MIT                                     | <http://xmlsoft.org>                           |
| minizip-ng | zlib License                            | <https://github.com/zlib-ng/minizip-ng>        |
| openssl    | Apache License 2.0                      | <https://www.openssl.org>                      |
| pcre2      | BSD-3-Clause with PCRE2-exception       | <https://www.pcre.org/>                        |
| protobuf   | Modified BSD License                    | <https://protobuf.dev>                         |
| sdl        | zlib License                            | <https://www.libsdl.org>                       |
| sqlite     | Public Domain                           | <https://www.sqlite.org>                       |
| tcl        | Tcl/Tk License                          | <https://www.tcl.tk>                           |
| zlib       | zlib License                            | <https://zlib.net>                             |


> [!NOTE]  
> For bzip2 we use the experimental version instead of the stable release

The `/runtimes` directory includes some files associated to the Microsoft Visual C++ Redistributable package.

# How to configure

* Define the SUMO_LIBRARIES environment variable with the path to which this repository was cloned to (For Example: "C:\SUMOLibraries")
