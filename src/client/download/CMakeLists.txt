project(client_download)

set(SOURCE_FILES
        DownloadManager.cpp DownloadManager.h DownloadWorker.cpp DownloadWorker.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})