project(server)
file(GLOB SOURCE_FILES "*.cpp" "utils/GeneralTypes.h")
file(GLOB SHARED_SOURCES "../shared/*.cpp")
file(GLOB PROTO_FILES "../../include/shared/protoc/*.proto")

find_package(Protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
add_executable(server ${SOURCE_FILES} ${SHARED_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

include_directories(${CMAKE_SOURCE_DIR}/../../include/server ${CMAKE_SOURCE_DIR}/../../include/shared)
add_subdirectory(database)

target_link_libraries(server spdlog server_database)
target_link_libraries(server ${PROTOBUF_LIBRARY})
