file(GLOB SOURCE_FILES "*.cpp")
add_executable(p2p-server ${SOURCE_FILES})
include_directories(${CMAKE_SOURCE_DIR}/../../include/server)
target_link_libraries(p2p-server spdlog)