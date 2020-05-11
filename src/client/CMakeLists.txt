project(client)
file(GLOB SOURCE_FILES "./*")

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

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(../shared .)
add_subdirectory(download)
add_subdirectory(database)
add_subdirectory(file)
add_subdirectory(networking)
add_executable(client ${SOURCE_FILES})
add_dependencies(client protobuf)
target_sources(client PUBLIC ${PROTO_HDRS} ${PROTO_SRCS})
target_link_libraries(client ${PROTOBUF_LIBRARY})
target_link_libraries(client spdlog shared client_download client_database client_file client_networking )
