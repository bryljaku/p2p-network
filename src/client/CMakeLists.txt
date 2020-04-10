project(client)
set(SOURCE_FILES main.cpp)

add_executable(client ${SOURCE_FILES})
add_subdirectory(download)
add_subdirectory(database)
add_subdirectory(file)
target_link_libraries(client spdlog client_download client_database client_file)
