project(shared)
file(GLOB SOURCE_FILES "./*")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})