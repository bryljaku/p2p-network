file(GLOB SOURCE_FILES "*.cpp")
add_executable(p2p-client ${SOURCE_FILES})
include_directories(${CMAKE_SOURCE_DIR}/../../include/client)
target_link_libraries(p2p-client spdlog)