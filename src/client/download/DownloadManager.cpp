#include <sharedUtils.h>
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
    
    for (const auto& peer: peers)
        workers.push_back(std::make_shared<DownloadWorker>(database, file, peer));
    
}

void DownloadManager::startWorkers() {
    for (const auto& w: workers)
        worker_threads.push_back(w->startWorker());
}


void DownloadManager::manageWorkers() {
    while(checkIfWorkersWork()) {
        if (file->isComplete()) {
            syslogger->info("Download completed for file {}", file->getPath());
            return;
        }
        updatePeers();
        // todo - check if there are any problems with workers
        sleep(1);
    }
}
void DownloadManager::updatePeers() {
    // add new workers for new peers
    auto filePeers = file->getPeers();
    auto myPeers = std::vector<std::shared_ptr<PeerInfo>>();
    for (const auto& w: workers)
        myPeers.emplace_back(w->getPeer());
    for (const auto& fp: filePeers)
        if (std::find(myPeers.begin(), myPeers.end(), fp) == myPeers.end())
            startWorkerThreadForPeer(fp);
    // todo smth with timeouted workers
}

void DownloadManager::startWorkerThreadForPeer(const std::shared_ptr<PeerInfo>& peer) {
    workers.emplace_back(std::make_shared<DownloadWorker>(database, file, peer));
    worker_threads.emplace_back(workers.back()->startWorker());
    worker_threads.back().join();
}
bool DownloadManager::checkIfWorkersWork() {
    for (auto w: workers)
        if (!w->isDone())
            return true;
    return false;
}

DownloadManager::~DownloadManager() {
    joinWorkers();
}

