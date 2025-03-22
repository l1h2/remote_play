@echo off
setlocal

set ROOT_DIR=%~dp0../../..
set DEV_DIR=%ROOT_DIR%/dev

set CMAKE_BUILD_DIR=%DEV_DIR%/cmake_build
if not exist "%CMAKE_BUILD_DIR%" (
    echo Error: CMake build files not found. Please run the CMake build script first. >&2
    endlocal
    exit 1
)

set BUILD_DIR=%DEV_DIR%/pyinstaller_build
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"

pyinstaller ^
    --name RemotePlay ^
    --windowed ^
    --distpath "%BUILD_DIR%/dist" ^
    --workpath "%BUILD_DIR%/build" ^
    --specpath "%BUILD_DIR%" ^
    --add-data "%CMAKE_BUILD_DIR%/bin;./bin" ^
    "%ROOT_DIR%/gui/main.py"

endlocal
