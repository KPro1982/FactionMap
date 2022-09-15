@echo off

cd /D "%~dp0"

IF exist "P:\FactionMap\" (
	echo Removing existing link P:\FactionMap
	rmdir "P:\FactionMap\"
)

echo mklink /J "P:\FactionMap\" "%~dp0"
mklink /J "P:\FactionMap\" "%~dp0"

echo Done
