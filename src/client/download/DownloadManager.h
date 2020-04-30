#ifndef P2P_NETWORK_DOWNLOADMANAGER_H
#define P2P_NETWORK_DOWNLOADMANAGER_H

#include <bits/shared_ptr.h>
#include <src/client/file/File.h>
#include <src/client/database/Database.h>

#include <utility>
#include "DownloadWorker.h"

// manages workers for one resource
class DownloadManager {
//FileManager fileManager;

    Database database;
    std::shared_ptr<File> file;
    std::vector<std::shared_ptr<DownloadWorker>> workers;
    std::vector<std::thread> worker_threads;
public:
    DownloadManager(Database database, std::shared_ptr<File> file):database(database), file(std::move(file)) {};
    
    ~DownloadManager();
    std::thread start_manager();
    void createWorkers();
    void manageWorkers();
    void startWorkers();
    void closeWorkers();
    void joinWorkers();
    
    void updatePeers();
    
    bool checkIfWorkersWork();
};


#endif //P2P_NETWORK_DOWNLOADMANAGER_H
