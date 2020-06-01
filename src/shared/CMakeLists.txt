project(shared)
file(GLOB SOURCE_FILES "./*")

include_directories(${PROTOBUF_INCLUDE_DIRS})
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} protobuf)