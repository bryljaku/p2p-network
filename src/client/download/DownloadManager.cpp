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
        workers.emplace_back(database, file, peer, fileManager);
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
        syslogger->info("Manager check");
        sleep(30);
    }
}
void DownloadManager::updatePeers() {
    auto listResponse = sSocket.sendSeedlistRequest(file->getTorrent().hashed);
    for (auto i: listResponse.ipv4s)
        if (!checkIfFileContainsPeerWithGivenIpV4(i))
            file->addPeer(PeerInfo(file->getPeers().size(),i, "", CLIENT_DEFAULT_PORT));
    for (auto i: listResponse.ipv6s)
        if (!checkIfFileContainsPeerWithGivenIpV6(i))
            file->addPeer(PeerInfo(file->getPeers().size(), "", i, CLIENT_DEFAULT_PORT));

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
    workers.emplace_back(DownloadWorker(database, file, peer, fileManager));
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
bool DownloadManager::checkIfFileContainsPeerWithGivenIpV4(IpV4Address address) {
    return !(std::find_if(
            file->getPeers().begin(), file->getPeers().end(),
            [&](auto& x) { return x->getIpV4Address() == address;}) == file->getPeers().end());
}
bool DownloadManager::checkIfFileContainsPeerWithGivenIpV6(IpV4Address address) {
    return !(std::find_if(
            file->getPeers().begin(), file->getPeers().end(),
            [&](auto& x) { return x->getIpV6Address() == address;}) == file->getPeers().end());
}

DownloadManager::~DownloadManager() = default;

