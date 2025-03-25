set(EXECUTABLE_NAME udp_connection)

set(SOURCES
    src/main.cpp
    src/udp_connection.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE include)
target_link_libraries(${EXECUTABLE_NAME} common ${SOCKET_LIB})
