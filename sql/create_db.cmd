@echo off

rem create origin database
"C:\Program Files\MySQL\MySQL Server 5.6\bin\mysql.exe" -u%1 -p%2 --default-character-set=utf8 < %~dp0\arm_od_uav_tables.sql

rem apply updates
for /f %%f in ('dir /b /on %~dp0\updates') do (
	"C:\Program Files\MySQL\MySQL Server 5.6\bin\mysql.exe" -u%1 -p%2 --default-character-set=utf8 --database=uav < %~dp0\updates\%%f
)