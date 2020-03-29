cmake_minimum_required(VERSION 3.13)
project(p2p-network)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -pthread)

#### LIBS
# spdlog
add_library(spdlog STATIC IMPORTED)
set_target_properties(spdlog PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/spdlog/build/libspdlog.a)
include_directories(lib/spdlog/include include/server include/client include/shared)

#### SOURCES
add_subdirectory(src/client)
add_subdirectory(src/server)

#### THREADS
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(p2p-server Threads::Threads)
target_link_libraries(p2p-client Threads::Threads)