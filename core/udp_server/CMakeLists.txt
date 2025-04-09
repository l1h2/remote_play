set(EXECUTABLE_NAME udp_server)

set(SOURCES
    src/main.cpp
    src/udp_server.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE include)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE common virtual_keyboard ${SOCKET_LIB})
