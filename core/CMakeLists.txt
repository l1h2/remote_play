cmake_minimum_required(VERSION 3.10)
project(remote_play)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(include)

add_executable(main src/main.cpp)

# Add other executables or libraries
# add_library(other_module SHARED src/other_module.cpp)

# Link libraries to executables if needed
# target_link_libraries(main_exe other_module)
