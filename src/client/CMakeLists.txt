project(client)
set(SOURCE_FILES main.cpp)

add_executable(client ${SOURCE_FILES})
add_subdirectory(download)
add_subdirectory(database)
add_subdirectory(file)
add_subdirectory(networking)
target_link_libraries(client spdlog ${PROTOBUF_LIBRARY} client_download client_database client_file client_networking)
