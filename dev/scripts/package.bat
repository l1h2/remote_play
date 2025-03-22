@echo off
setlocal enabledelayedexpansion

set NO_CONFIRM_FLAG=-y
set NO_CONFIRM=0
if "%1"=="%NO_CONFIRM_FLAG%" (
    set NO_CONFIRM=1
)

set ROOT_DIR=%~dp0../..
set DIST_DIR=%ROOT_DIR%/dev/dist
if not exist "%DIST_DIR%" (
    mkdir "%DIST_DIR%"
)

if %NO_CONFIRM%==0 (
    choice /M "Do you want to run the CMake build script?"
    if !ERRORLEVEL!==1 (
        call "%~dp0builds/cmake_build.bat"
    ) else (
        echo Skipping CMake build script.
    )

    choice /M "Do you want to run the PyInstaller build script?"
    if !ERRORLEVEL!==1 (
        call "%~dp0builds/pyinstaller_build.bat"
    ) else (
        echo Skipping PyInstaller build script.
    )
) else (
    call "%~dp0builds/cmake_build.bat" %NO_CONFIRM_FLAG%
    call "%~dp0builds/pyinstaller_build.bat"
)

set PYINSTALLER_DIST_DIR=%ROOT_DIR%/dev/pyinstaller_build/dist
if not exist "%PYINSTALLER_DIST_DIR%" (
    echo Error: PyInstaller dist directory not found. Please run the PyInstaller build script first. >&2
    endlocal
    exit 1
)

REM Resolve the absolute path of the dist directory
for %%i in ("%DIST_DIR%") do set SOLVED_DIST_DIR=%%~fi
echo Copying PyInstaller dist files to the %SOLVED_DIST_DIR%

rmdir /s /q "%DIST_DIR%"
mkdir "%DIST_DIR%"
xcopy /y /i /e "%PYINSTALLER_DIST_DIR%" "%DIST_DIR%" >nul

if errorlevel 1 (
    echo Error: Failed to copy files. Please ensure the files are not in use and you have the necessary permissions. >&2
    endlocal
    exit 1
)

echo Packaging completed successfully.

if %NO_CONFIRM%==0 (
    choice /M "Do you want to run the executable?"
    if !ERRORLEVEL!==1 (
        call "%~dp0run.bat"
    ) else (
        echo Closing the script.
    )
) else (
    call "%~dp0run.bat"
)

endlocal
