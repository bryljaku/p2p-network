#ifndef P2P_NETWORK_DOWNLOADMANAGER_H
#define P2P_NETWORK_DOWNLOADMANAGER_H
//created by Jakub
#include <bits/shared_ptr.h>
#include <database/Database.h>
#include <file/File.h>

#include <utility>
#include <src/shared/Torrent.h>
#include "DownloadWorker.h"

// manages workers for one resource
class DownloadManager {
//FileManager fileManager;
    
    Database database;
    std::shared_ptr<File> file;
    std::vector<DownloadWorker> workers;
    std::vector<std::thread> worker_threads;
    Torrent torrent;
public:
    DownloadManager(Database database1, std::shared_ptr<File> file1, Torrent torrent1):database(std::move(database1)), file(std::move(file1)), torrent(std::move(torrent1)) {
        syslogger->info("DownloadManager for file {} created", file->getId());
    };
    
    ~DownloadManager();
    std::thread start_manager();
    void createWorkers();
    void manageWorkers();
    void startWorkers();
    void joinWorkers();
    
    void updatePeers();
    
    bool checkIfWorkersWork();
    
    void startWorkerThreadForPeer(const std::shared_ptr<PeerInfo>& peer);
};


#endif //P2P_NETWORK_DOWNLOADMANAGER_H