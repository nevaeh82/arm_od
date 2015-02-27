@echo off

set PATH=%PATH%;%~dp0redist\nsis
set PATH=%PATH%;C:\Qt\4.8.6msvc;
set PATH=%PATH%;C:\Python27
set PATH=%PATH%;"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC";"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin";
set QTDIR=%QTDIR%;C:\Qt\4.8.6msvc;
set INCLUDE=%INCLUDE%;"c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\INCLUDE;c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\ATLMFC\INCLUDE;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\include"
set LIBPATH=%LIBPATH%;"C:\Windows\Microsoft.NET\Framework\v4.0.30319;C:\Windows\Microsoft.NET\Framework\v3.5;c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\LIB;c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\ATLMFC\LIB"
set LIB=%LIB%;"c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\LIB;c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\ATLMFC\LIB;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\lib"

cmake -E remove_directory build
cmake -E make_directory  build

cd build

cmake -E remove CMakeCache.txt

cmake .. -G "NMake Makefiles"

nmake
nmake install package