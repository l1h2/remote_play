set(EXECUTABLE_NAME udp_client)

set(SOURCES
    src/main.cpp
    src/udp_client.cpp
    src/input_capture.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE include)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE common ${SOCKET_LIB} sfml-system sfml-window)
