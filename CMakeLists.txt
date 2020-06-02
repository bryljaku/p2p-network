cmake_minimum_required(VERSION 3.13)
project(p2p-network)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -pthread)

#### GLOBAL FILES
file(GLOB SHARED_SOURCES "./src/shared/*.cpp")
file(GLOB PROTO_FILES "./src/shared/protoc/*.proto")

#### LIBS

# protobuf
find_package( Protobuf REQUIRED)
if ( Protobuf_FOUND )
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
    message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
    message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
else()
    message( WARNING "Protobuf package not found -> specify search path via PROTOBUF_ROOT variable")
endif()
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
add_custom_target(protobuf ALL DEPENDS ${PROTO_HDRS} ${PROTO_SRCS})

# spdlog
add_library(spdlog STATIC IMPORTED)
set_target_properties(spdlog PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/spdlog/build/libspdlog.a)
include_directories(lib/spdlog/include include/server include/client include/shared)

#### SOURCES
add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(src/shared)

#### TESTS
add_subdirectory(src/client/tests)
add_subdirectory(src/server/tests)

#### THREADS
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads)
target_link_libraries(client Threads::Threads)