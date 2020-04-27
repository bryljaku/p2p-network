#include <src/shared/sharedUtils.h>
#include "DownloadManager.h"

std::thread DownloadManager::start_manager() {
    return std::thread([&] {try {
        createWorkers();
        startWorkers();
        joinWorkers();
        manageWorkers();
        closeWorkers();
    } catch (std::exception &e) {
        syslogger->error("DownloadManager->{}\n{}", file->getPath(), e.what());
    }
    });
}
void DownloadManager::closeWorkers() {
    for (auto &w: workers)
        w->close_connection();
}

void DownloadManager::joinWorkers() {
    for (auto &w: worker_threads)
        w.join();
}

void DownloadManager::createWorkers() {
    auto peers = file->getPeers();
    
    if (peers.empty())
        throw::std::runtime_error("No peers possesing file");
    
    for (auto peer: peers)
        workers.push_back(std::make_shared<DownloadWorker>(database, file, peer));
    
}

void DownloadManager::startWorkers() {
    for (const auto& w: workers)
        worker_threads.push_back(w->startWorker());
}


void DownloadManager::manageWorkers() {
//todo
}

