set(EXECUTABLE_NAME stun_client)

set(SOURCES
    src/main.cpp
    src/stun_client.cpp
    src/stun_response_validator.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE include)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE common ${SOCKET_LIB})
