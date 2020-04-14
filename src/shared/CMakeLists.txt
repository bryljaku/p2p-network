project(shared)

set(SOURCE_FILES sharedUtils.cpp sharedUtils.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})