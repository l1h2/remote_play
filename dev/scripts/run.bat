@echo off
setlocal

set ROOT_DIR=%~dp0../..

set APP_EXECUTABLE=%ROOT_DIR%/dev/dist/RemotePlay/RemotePlay.exe
if not exist "%APP_EXECUTABLE%" (
    echo Error: RemotePlay executable not found. Please run the packaging script first. >&2
    endlocal
    exit 1
)

echo Running the RemotePlay application...
start "" "%APP_EXECUTABLE%"

endlocal
