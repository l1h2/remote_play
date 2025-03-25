set(LIB_NAME common)

set(SOURCES
    src/common.cpp
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC include)
