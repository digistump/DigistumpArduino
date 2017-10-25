@echo off
echo Detecting OS processor type
echo Launching Driver Installer
sleep 1

if "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto 64BIT

START "" "%~dp0\DPinst.exe"
GOTO END

:64BIT
START "" "%~dp0\DPinst64.exe"


:END
sleep 1