#include <sharedUtils.h>
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
    finished = true;
}
void DownloadWorker::connect() {

}

void DownloadWorker::close_connection() {

}

std::shared_ptr<PeerInfo> DownloadWorker::getPeer() {
    return peer;
}

bool DownloadWorker::isDone() {
    return finished;
}

DownloadWorker::~DownloadWorker() {
    close_connection();
}

