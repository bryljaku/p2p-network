project(client_networking)
set(SOURCE_FILES CSocket.cpp SSocket.cpp RequestHandler.cpp BaseSocket.cpp ResponderThread.cpp)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} protobuf)