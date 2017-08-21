@echo off

rem change console outpul lang to en_US
chcp 437

rem arg1 - build type
rem arg2 - version
set build_type=%1
set version=%2

rem prepare build type
if "%build_type%" == "" set build_type=debug
if "%build_type%" == "Debug" set build_type=debug
if "%build_type%" == "Release" set build_type=release

rem init environment vars for Visual C
if not "%DevEnvDir%" == "" goto do_build
set VCVARS_BAT="c:\Program Files (x86)\Microsoft Visual Studio 10.0\vc\vcvarsall.bat"
if not exist %VCVARS_BAT% ( set VCVARS_BAT="c:\Program Files\Microsoft Visual Studio 10.0\vc\vcvarsall.bat" )
call %VCVARS_BAT%
if %errorlevel% neq 0 goto error

:do_build
rem clean build folder
cmake -E remove_directory build
if %errorlevel% neq 0 goto error

cmake -E make_directory build
if %errorlevel% neq 0 goto error

cd build
if %errorlevel% neq 0 goto error

rem configure build
cmake -G "NMake Makefiles" .. -DCMAKE_BUILD_TYPE=%build_type% -DCMAKE_INSTALL_PREFIX=tmprel -DVERSION=%version%
if %errorlevel% neq 0 goto error2

rem run build
jom all
if %errorlevel% neq 0 goto error2

cpack

:no_error
cd ..
exit /B 0

:error
exit /B %errorlevel%

:error2
set error=%errorlevel%
cd ..
exit /B %error%