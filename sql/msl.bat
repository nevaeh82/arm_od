echo.
set password=root
set installer="C:\Program Files (x86)\MySQL\MySQL Installer for Windows\MySQLInstallerConsole.exe"
set version=5.6.28
echo -----------------------
echo Details
echo -----------------------
echo Server User: root
echo.
echo Server Password: %password%
echo.
echo Configuration Folder: %installer%
echo.
echo Version: %version%
echo -----------------------
echo Start Installation... (si prega di attendere alcuni minuti)
echo.

msiexec /i "%~dp0\mysql-installer-community-%version%.0.msi" /qb

%installer% community install server;%version%;X64:*:port=3306;openfirewall=true;passwd=%password% -silent

pause