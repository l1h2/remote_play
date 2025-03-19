@echo off
setlocal

REM Root dir in relation to the cmake_build directory
REM Using %~dp0 confuses CMake with absolute and relative path resolution
set ROOT_DIR=../..

set BUILD_DIR=%~dp0../../cmake_build
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)
cd "%BUILD_DIR%"

cmake "%ROOT_DIR%/core"
cmake --build .

endlocal
