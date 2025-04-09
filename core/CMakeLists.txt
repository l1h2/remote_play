cmake_minimum_required(VERSION 3.10)
project(remote_play VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(common/include virtual_keyboard/include)

# Platform-specific settings
if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "MSYS")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    endif()
    set(SOCKET_LIB ws2_32)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(SOCKET_LIB pthread)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(SOCKET_LIB "")
endif()

# Add subdirectories for each library
add_subdirectory(common)
add_subdirectory(virtual_keyboard)

# Add subdirectories for each executable
add_subdirectory(stun_client)
add_subdirectory(udp_connection)
add_subdirectory(udp_server)
add_subdirectory(udp_client)
