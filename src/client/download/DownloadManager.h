#ifndef P2P_NETWORK_DOWNLOADMANAGER_H
#define P2P_NETWORK_DOWNLOADMANAGER_H
//created by Jakub
#include <bits/shared_ptr.h>
#include <database/Database.h>
#include <file/File.h>

#include <utility>
#include <Torrent.h>
#include <src/client/file/FileManager.h>
#include "DownloadWorker.h"

// manages workers for one resource
class DownloadManager {
//FileManager fileManager;
    
    std::shared_ptr<Database> database;
    std::shared_ptr<File> file;
    FileManager& fileManager;
    std::vector<DownloadWorker> workers;
    std::vector<std::thread> worker_threads;
public:

    DownloadManager(std::shared_ptr<Database> database1, std::shared_ptr<File> file1, FileManager& fileManager1)
    :database(std::move(database1)), file(std::move(file1)), fileManager(fileManager1) {
        syslogger->info("DownloadManager for file {} created", file->getId());
    }
    
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