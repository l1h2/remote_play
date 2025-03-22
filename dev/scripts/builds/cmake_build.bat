@echo off
setlocal

set NO_CONFIRM=0
if "%1"=="-y" (
    set NO_CONFIRM=1
)

REM Root dir in relation to the cmake_build directory
REM Using %~dp0 confuses CMake with absolute and relative path resolution
set ROOT_DIR=../..
set BUILD_DIR=%~dp0../../cmake_build
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

cmake "%ROOT_DIR%/core"
if errorlevel 1 (
    echo Error: Failed to configure CMake. >&2
    endlocal
    exit 1
)
cmake --build .
if errorlevel 1 (
    echo Error: Failed to build the project. >&2
    endlocal
    exit 1
)

if %NO_CONFIRM%==0 (
    choice /M "Do you want to copy the compiled files to the GUI directory?"
    if errorlevel 2 (
        echo Skipping copying compiled files to the GUI directory.
        endlocal
        exit /b
    )
)

set GUI_DIR=%~dp0../../../gui
if not exist "%GUI_DIR%" (
    mkdir "%GUI_DIR%"
)

for %%i in ("%GUI_DIR%") do set SOLVED_GUI_DIR=%%~fi
echo Copying files to the %SOLVED_GUI_DIR%

xcopy /y /i "%BUILD_DIR%/bin" "%GUI_DIR%/bin" >nul
if errorlevel 1 (
    echo Error: Failed to copy files. Please ensure the files are not in use and you have the necessary permissions. >&2
    endlocal
    exit 1
)

echo Files copied successfully.

endlocal
