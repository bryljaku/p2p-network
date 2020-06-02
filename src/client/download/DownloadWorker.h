#ifndef P2P_NETWORK_DOWNLOADWORKER_H
#define P2P_NETWORK_DOWNLOADWORKER_H
//created by Jakub
#include <sharedUtils.h>
#include <utility>
#include "networking/CSocket.h"

#include <thread>
#include <utility>
#include <database/Database.h>
#include <file/FileManager.h>

#include <Torrent.h>
#include <networking/CSocket.h>

class DownloadWorker {
    std::shared_ptr<Database> database;
    std::shared_ptr<File> file;
    FileManager& fileManager;
    std::shared_ptr<PeerInfo> peer;
    Torrent torrent;
    bool finished;
public:

    DownloadWorker(std::shared_ptr<Database> database1, std::shared_ptr<File> file1, std::shared_ptr<PeerInfo> peer1, FileManager& fileManager1);
    ~DownloadWorker();
    std::shared_ptr<PeerInfo> getPeer();
    std::thread startWorker();
    bool isDone();
    void work();
};


#endif //P2P_NETWORK_DOWNLOADWORKER_H