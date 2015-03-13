@echo off

rem create origin database
mysql.exe -u%1 -p%2 --default-character-set=utf8 < %~dp0\arm_od_uav_tablesTest.sql

rem apply updates
for /f %%f in ('dir /b /on %~dp0\updates') do (
	mysql.exe -u%1 -p%2 --default-character-set=utf8 --database=uavtest < %~dp0\updates\%%f
)