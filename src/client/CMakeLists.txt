file(GLOB SOURCE_FILES "*.cpp")
file(GLOB SHARED_SOURCES "../shared/*.cpp")
file(GLOB PROTO_FILES "../../include/shared/protoc/*.proto")

find_package(Protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
add_executable(p2p-client ${SOURCE_FILES} ${SHARED_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

include_directories(${CMAKE_SOURCE_DIR}/../../include/client ${CMAKE_SOURCE_DIR}/../../include/shared)
target_link_libraries(p2p-client spdlog)
target_link_libraries(p2p-client ${PROTOBUF_LIBRARY})