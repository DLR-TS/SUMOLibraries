:: ------------------------------------------------------
:: TroubleChute [tcno.co]'s OpenSSL build script
:: For x64, from: https://youtu.be/PMHEoBkxYaQ
:: Builds:
::   - x64 Shared <Debug/Release>
::   - x64 Static <Debug/Release>
:: ------------------------------------------------------
:: Remember to replace (((FOLDER))) with a real directory
:: ------------------------------------------------------

nmake clean
perl Configure VC-WIN64A --debug --prefix=C:\Librerias\SUMOLibraries\3rdPartyLibs\openssl-1.1.1_t\DLL\x64\Debug --openssldir=C:\Librerias\SUMOLibraries\3rdPartyLibs\openssl-1.1.1_t\SSL
nmake test
nmake install_sw

nmake clean
perl Configure VC-WIN64A --prefix=C:\Librerias\SUMOLibraries\3rdPartyLibs\openssl-1.1.1_t\DLL\x64\Release --openssldir=C:\Librerias\SUMOLibraries\3rdPartyLibs\openssl-1.1.1_t\SSL
nmake test
nmake install_sw

nmake clean
perl Configure VC-WIN64A --debug --prefix=C:\Librerias\SUMOLibraries\3rdPartyLibs\openssl-1.1.1_t\Lib\x64\Debug --openssldir=C:\Librerias\SUMOLibraries\3rdPartyLibs\openssl-1.1.1_t\SSL no-shared
nmake test
nmake install_sw

nmake clean
perl Configure VC-WIN64A --prefix=C:\Librerias\SUMOLibraries\3rdPartyLibs\openssl-1.1.1_t\Lib\x64\Release --openssldir=C:\Librerias\SUMOLibraries\3rdPartyLibs\openssl-1.1.1_t\SSL no-shared
nmake test
nmake install_sw