set(EXECUTABLE_NAME stun_client)

set(SOURCES
    src/main.cpp
    src/stun_client.cpp
    src/stun_response_validator.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE include)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "MSYS")
    target_link_libraries(${EXECUTABLE_NAME} ws2_32)
endif()
