@echo off
setlocal

set ROOT_DIR=%~dp0../..

set DIST_DIR=%ROOT_DIR%/dev/dist
if not exist "%DIST_DIR%" (
    mkdir "%DIST_DIR%"
)

choice /M "Do you want to run the CMake build script?"
if %ERRORLEVEL%==1 (
    call "%~dp0builds/cmake_build.bat"
) else (
    echo Skipping CMake build script.
)

choice /M "Do you want to run the PyInstaller build script?"
if %ERRORLEVEL%==1 (
    call "%~dp0builds/pyinstaller_build.bat"
) else (
    echo Skipping PyInstaller build script.
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
xcopy /y /i /e "%PYINSTALLER_DIST_DIR%" "%DIST_DIR%" >nul

echo Packaging completed successfully.

endlocal
