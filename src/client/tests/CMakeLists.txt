set(BOOST_ROOT "../../../boost-built-1_73_0")
set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS} ../../shared)

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

file(GLOB SOURCE_FILES "./*")
add_executable (tests_client ${SOURCE_FILES})
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(../.)
add_dependencies(tests_client protobuf)
target_sources(tests_client PUBLIC ${PROTO_HDRS} ${PROTO_SRCS})
target_link_libraries(tests_client ${PROTOBUF_LIBRARY})
target_link_libraries(tests_client spdlog client_download client_database client_file client_networking shared)

target_link_libraries (tests_client ${Boost_LIBRARIES} )