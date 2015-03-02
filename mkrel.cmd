@echo off

set PATH=%PATH%;%~dp0redist\nsis
set PATH=%PATH%;C:\Qt\4.8.6msvc\bin;
set PATH=%PATH%;C:\Python27;
set QTDIR=%QTDIR%;C:\Qt\4.8.6msvc;

cmake -E remove_directory build
cmake -E make_directory  build

cd build
call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\vc\bin\vcvars32.bat"

cmake -E remove CMakeCache.txt
cmake -G "NMake Makefiles" ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./tmprel/bin/

nmake all
nmake install package