cmake_minimum_required(VERSION 3.13)
project(p2p-network)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -pthread)

##### LIBS
# spdlog
add_library(spdlog STATIC IMPORTED)
set_target_properties(spdlog PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/spdlog/build/libspdlog.a)
include_directories(lib/spdlog/include)

#### SOURCES
add_subdirectory(src/client)
add_subdirectory(src/server)
