@echo off

make

if %ERRORLEVEL% NEQ 0 (
    echo:
    echo Failed to build ROM
    exit /b %errorlevel%
)

color 0A
echo Build succesful, running ROM ...

%GBA_EMU% gba-first-project.gba