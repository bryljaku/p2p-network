project(client_file)

set(SOURCE_FILES File.cpp File.h Segment.cpp Segment.h PeerInfo.cpp PeerInfo.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})