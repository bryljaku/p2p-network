file(GLOB SOURCE_FILES "*.cpp")
file(GLOB SHARED_SOURCES "../shared/*.cpp")
file(GLOB PROTO_FILES "../../include/shared/protoc/*.proto")

find_package(Protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
add_executable(p2p-server ${SOURCE_FILES} ${SHARED_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

include_directories(${CMAKE_SOURCE_DIR}/../../include/server ${CMAKE_SOURCE_DIR}/../../include/shared)
target_link_libraries(p2p-server spdlog)
target_link_libraries(p2p-server ${PROTOBUF_LIBRARY})