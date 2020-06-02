#ifndef P2P_NETWORK_DOWNLOADMANAGER_H
#define P2P_NETWORK_DOWNLOADMANAGER_H
//created by Jakub
#include <bits/shared_ptr.h>
#include <database/Database.h>
#include <file/File.h>

#include <utility>
#include <Torrent.h>
#include <file/FileManager.h>
#include <networking/SSocket.h>
#include "DownloadWorker.h"

class DownloadManager {
    IpAddress trackerAddress;
    std::shared_ptr<Database> database;
    std::shared_ptr<File> file;
    FileManager& fileManager;
    std::vector<DownloadWorker> workers;
    std::vector<std::thread> worker_threads;

public:

    DownloadManager(std::shared_ptr<Database> database1, std::shared_ptr<File> file1, FileManager& fileManager1, IpAddress &trackerAddress)
    : trackerAddress(trackerAddress), database(std::move(database1)), file(std::move(file1)),  fileManager(fileManager1) {
        syslogger->info("DownloadManager for file {} created", file->getId());
    }
    
    ~DownloadManager();
    std::thread start_manager();
    void manageWorkers();
    void joinWorkers();
    void updatePeersAndStartWorkers();
    bool checkIfWorkersWork();
    void startWorkerThreadForPeer(const std::shared_ptr<PeerInfo>& peer);
    bool checkIfFileContainsPeerWithGivenIpV4(IpV4Address address);
    bool checkIfFileContainsPeerWithGivenIpV6(IpV4Address address);
    static bool checkIfWorkersWorkWithPeer(std::vector<std::shared_ptr<PeerInfo>> myPeers, std::shared_ptr<PeerInfo> peer);
    void addPeersToFile(const SeedlistResponse& response);
	void startWorkersForNewPeers();
};


#endif //P2P_NETWORK_DOWNLOADMANAGER_H