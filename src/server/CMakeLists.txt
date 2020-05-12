project(server)
file(GLOB SOURCE_FILES "./*")

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(../shared)
add_subdirectory(database)

SET_SOURCE_FILES_PROPERTIES(
        ${PROTO_SRCS}
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
)
SET_SOURCE_FILES_PROPERTIES(
        ${PROTO_HDRS}
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
)

add_executable(server ${SOURCE_FILES})
add_dependencies(server protobuf)
target_sources(server PUBLIC ${PROTO_HDRS} ${PROTO_SRCS})
target_link_libraries(server ${PROTOBUF_LIBRARY})
target_link_libraries(server spdlog server_database shared)