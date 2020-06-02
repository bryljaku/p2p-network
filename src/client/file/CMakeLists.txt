project(client_file)

set(SOURCE_FILES File.cpp File.h Segment.cpp Segment.h PeerInfo.h FileManager.cpp FileManager.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
