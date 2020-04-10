project(server)
set(SOURCE_FILES main.cpp utils/GeneralTypes.h)

add_executable(server ${SOURCE_FILES})

add_subdirectory(database)
include_directories(../shared)
target_link_libraries(server spdlog ${PROTOBUF_LIBRARY} shared server_database)