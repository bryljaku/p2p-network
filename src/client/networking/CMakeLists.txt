project(client_networking)
set(SOURCE_FILES CSocket.cpp CSocket.h SSocket.cpp SSocket.h RequestHandler.cpp RequestHandler.h BaseSocket.cpp BaseSocket.h)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})