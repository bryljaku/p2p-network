project(client_networking)
set(SOURCE_FILES CSocket.cpp SSocket.cpp BaseSocket.cpp ResponderThread.cpp RequestHandler.cpp RequestHandler.h)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} protobuf)