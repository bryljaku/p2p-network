cmake_minimum_required(VERSION 3.13)
project(p2p-network)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -pthread)

target_link_libraries(p2p-network )
set(SOURCE_FILES src/main.cpp)

add_executable(p2p-network ${SOURCE_FILES})