#include <sharedUtils.h>
#include "DownloadManager.h"
// created by Jakub
std::thread DownloadManager::start_manager() {
    return std::thread([&] {try {
        updatePeers();
        createWorkers();
        startWorkers();
        manageWorkers();
        joinWorkers();
    } catch (std::exception &e) {
        syslogger->error("Exception caught in DownloadManager for torrent {}\n{}", file->getTorrent().hashed, e.what());
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
    syslogger->info("DownloadManager updating peers for torrent {}", file->getTorrent().hashed);
    auto listResponse = sSocket.sendSeedlistRequest(file->getTorrent().hashed);
    for (int i = 0; i < listResponse.ipv4s.size(); i++)
        if (!checkIfFileContainsPeerWithGivenIpV4(listResponse.ipv4s[i]) || checkIfFileContainsPeerWithGivenIpV6(listResponse.ipv6s[i]))
            file->addPeer(PeerInfo(file->getPeers().size() + 1, listResponse.ipv4s[i], listResponse.ipv6s[i], CLIENT_DEFAULT_PORT)); // todo change default port to port from response
    syslogger->info("successfully updated peers for file {}", file->getTorrent().fileName);
    auto filePeers = file->getPeers();
    auto myPeers = std::vector<std::shared_ptr<PeerInfo>>();
    for (auto &w: workers)
        myPeers.emplace_back(w.getPeer());
    for (auto& fp: filePeers)
        if (!checkIfWorkersWorkWithPeer(myPeers, fp))
            startWorkerThreadForPeer(fp);
}

void DownloadManager::startWorkerThreadForPeer(const std::shared_ptr<PeerInfo>& peer) {
    workers.emplace_back(DownloadWorker(database, file, peer, fileManager));
    worker_threads.emplace_back(workers.back().startWorker());
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
bool DownloadManager::checkIfWorkersWorkWithPeer(std::vector<std::shared_ptr<PeerInfo>> myPeers, std::shared_ptr<PeerInfo> peer) {
    return !(std::find_if(
            myPeers.begin(), myPeers.end(),
            [&](auto& x) { return x->getIpV6Address() == peer->getIpV6Address() || x->getIpV4Address() == peer->getIpV4Address();}) == myPeers.end());
}

DownloadManager::~DownloadManager() = default;

