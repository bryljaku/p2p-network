project(server)
file(GLOB SOURCE_FILES "./*")
file(GLOB PROTO_FILES "../shared/protoc/*.proto")

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

include_directories(../shared)
add_subdirectory(database)

add_executable(server ${SOURCE_FILES} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(server spdlog ${PROTOBUF_LIBRARY} server_database shared)