project(client_download)
include_directories(../../shared)
include_directories(../.)
set(SOURCE_FILES
        DownloadManager.cpp DownloadManager.h DownloadWorker.cpp DownloadWorker.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})