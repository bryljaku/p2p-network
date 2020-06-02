#include <sharedUtils.h>
#include "DownloadManager.h"
// created by Jakub
std::thread DownloadManager::start_manager() {
    return std::thread([&] {
        try {
            updatePeersAndStartWorkers();
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

void DownloadManager::manageWorkers() {
    while(checkIfWorkersWork()) {
        if (file->isComplete()) {
            syslogger->info("Download completed for file {}", file->getPath());
            return;
        }
		updatePeersAndStartWorkers();
        syslogger->info("Manager check");
        sleep(30);
    }
}
void DownloadManager::updatePeersAndStartWorkers() {

    syslogger->info("DownloadManager updating peers for torrent {}", file->getTorrent().hashed);
	SSocket sSocket(trackerAddress.ip, trackerAddress.port);	//TODO: tu lepiej w wątkach, bo może chwile wisieć na połączeniu
	auto state = sSocket.start();
	if (state != OPEN) {
		syslogger->warn("DownloadManager problem occured while connecting to {}:{}", trackerAddress.ip, trackerAddress.port);
		return;
	}
    auto listResponse = sSocket.sendSeedlistRequest(file->getTorrent().hashed);
    addPeersToFile(listResponse);

    syslogger->info("successfully updated peers for file {}", file->getTorrent().fileName);
	startWorkersForNewPeers();
}
void DownloadManager::startWorkersForNewPeers()
{
	auto filePeers = file->getPeers();
	auto myPeers = std::vector<std::shared_ptr<PeerInfo>>();
	for (auto &w: workers)
		myPeers.emplace_back(w.getPeer());
	for (auto& fp: filePeers)
		if (!checkIfWorkersWorkWithPeer(myPeers, fp))
			startWorkerThreadForPeer(fp);
}
void DownloadManager::startWorkerThreadForPeer(const std::shared_ptr<PeerInfo>& peer) {
    workers.emplace_back(DownloadWorker(file, peer, fileManager));
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

void DownloadManager::addPeersToFile(const SeedlistResponse& response) {
    for (auto i: response.ipv6peers())
        if (checkIfFileContainsPeerWithGivenIpV6(i.ip()))
            file->addPeer(PeerInfo(file->getPeers().size() + 1, "", i.ip(), i.port()));
    for (auto i: response.ipv6seeds())
        if (checkIfFileContainsPeerWithGivenIpV6(i.ip()))
            file->addPeer(PeerInfo(file->getPeers().size() + 1, "", i.ip(), i.port()));
    for (auto i: response.ipv4seeds())
        if (checkIfFileContainsPeerWithGivenIpV4(i.ip()))
            file->addPeer(PeerInfo(file->getPeers().size() + 1, i.ip(), "", i.port()));
    for (auto i: response.ipv4peers())
        if (checkIfFileContainsPeerWithGivenIpV4(i.ip()))
            file->addPeer(PeerInfo(file->getPeers().size() + 1, i.ip(),"", i.port()));
}

DownloadManager::~DownloadManager() = default;

