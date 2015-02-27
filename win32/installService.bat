echo off
Pushd %~dp0

set argR=%1
echo %argR%

if "%argR%" == "-rt" (
	echo "Uninstall Tun"	
	
	sc stop "SkyHobbit Tun Land"
	SkyHobbitTunLand.exe -u
) else if "%argR%" == "-rs" (
	echo "Uninstall Server"	
	
	sc stop "SkyHobbit Land Server"
	SkyHobbitLandServer.exe -u
) else if "%argR%" == "-it" (
	echo "Install only Tun"	
	
	SkyHobbitTunLand.exe -i
	sc start "SkyHobbit Tun Land"
) else if "%argR%" == "-is" (
	echo "Install only Server"	
	
	SkyHobbitLandServer.exe -i
	sc start "SkyHobbit Land Server"
) else (
	echo "Wrong args"	
) 
