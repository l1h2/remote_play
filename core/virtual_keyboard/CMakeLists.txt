set(LIB_NAME virtual_keyboard)

set(SOURCES
    src/input_simulator.cpp
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PRIVATE include)
