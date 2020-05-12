#include <sharedUtils.h>
#include "DownloadManager.h"
// created by Jakub
std::thread DownloadManager::start_manager() {
    return std::thread([&] {try {
        createWorkers();
        startWorkers();
        joinWorkers();
        manageWorkers();
    } catch (std::exception &e) {
        syslogger->error("DownloadManager->{}\n{}", file->getPath(), e.what());
    }
    });
}


void DownloadManager::joinWorkers() {
    for (auto &w: worker_threads)
        w.join();
    syslogger->info("DownloadManager joined workers for file {}", file->getId());
}

void DownloadManager::createWorkers() {
    auto peers = file->getPeers();
    
    if (peers.empty())
        throw::std::runtime_error("No peers possesing file");
    
    for (auto& peer: peers)
        workers.push_back(DownloadWorker(database, file, torrent, peer));
    syslogger->info("DownloadManager created workers for file {}", file->getId());
}

void DownloadManager::startWorkers() {
    for (auto& w: workers)
        worker_threads.push_back(w.startWorker());
    syslogger->info("DownloadManager started workers for file {}", file->getId());
    
}


void DownloadManager::manageWorkers() {
    while(checkIfWorkersWork()) {
        if (file->isComplete()) {
            syslogger->info("Download completed for file {}", file->getPath());
            return;
        }
        updatePeers();
        // todo - check if there are any problems with workers
        syslogger->info("Manager check");
        sleep(5);
    }
}
void DownloadManager::updatePeers() {
    // add new workers for new peers
    auto filePeers = file->getPeers();
    auto myPeers = std::vector<std::shared_ptr<PeerInfo>>();
    for (auto &w: workers)
        myPeers.emplace_back(w.getPeer());
    for (auto& fp: filePeers)
        if (std::find(myPeers.begin(), myPeers.end(), fp) == myPeers.end())
            startWorkerThreadForPeer(fp);
    // todo smth with timeouted workers
}

void DownloadManager::startWorkerThreadForPeer(const std::shared_ptr<PeerInfo>& peer) {
    workers.emplace_back(DownloadWorker(database, file, torrent, peer));
    worker_threads.emplace_back(workers.back().startWorker());
    worker_threads.back().join();
    syslogger->info("DownloadManager added new worker for file {} for new peer {}", file->getId(), peer->getId());
    
}
bool DownloadManager::checkIfWorkersWork() {
    for (auto &w: workers)
        if (!w.isDone())
            return true;
    return false;
}

DownloadManager::~DownloadManager() {
}
