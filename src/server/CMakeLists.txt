project(server)
set(SOURCE_FILES main.cpp utils/GeneralTypes.h)

add_executable(server ${SOURCE_FILES})

add_subdirectory(database)

target_link_libraries(server spdlog server_database)