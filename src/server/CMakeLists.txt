set(SOURCE_FILES main.cpp)

add_executable(p2p-server ${SOURCE_FILES})

target_link_libraries(p2p-server spdlog)