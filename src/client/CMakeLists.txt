set(SOURCE_FILES main.cpp)

add_executable(p2p-client ${SOURCE_FILES})

target_link_libraries(p2p-client spdlog)