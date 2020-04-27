#include <src/shared/sharedUtils.h>
#include "DownloadWorker.h"

std::thread DownloadWorker::startWorker() {
    return std::thread([&] {
        try {
            connect();
            syslogger->info("new DownloadWorker initiated");
            work();
            syslogger->info("DownloadWorker finished");
        } catch (std::exception &e) {
            syslogger->error("DownloadWorker->{}\n{}", file->getPath(), e.what());
        }
        
    });
}

void DownloadWorker::work() {
    sleep(1);
}
void DownloadWorker::connect() {

}

void DownloadWorker::close_connection() {

}

