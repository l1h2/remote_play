@echo off
setlocal

set ROOT_DIR=%~dp0../../..
set DEV_DIR=%ROOT_DIR%/dev

set BUILD_DIR=%DEV_DIR%/pyinstaller_build
if not exist "%BUILD_DIR%" (
    echo Error: PyInstaller build directory not found. Please run the PyInstaller setup script first. >&2
    endlocal
    exit 1
)

set SPEC_FILE=%BUILD_DIR%/RemotePlay.spec
if not exist "%SPEC_FILE%" (
    echo Error: PyInstaller spec file not found. Please run the PyInstaller setup script first. >&2
    endlocal
    exit 1
)

pyinstaller ^
    --clean ^
    --noconfirm ^
    --distpath "%BUILD_DIR%/dist" ^
    --workpath "%BUILD_DIR%/build" ^
    "%SPEC_FILE%"

endlocal
