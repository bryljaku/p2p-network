#ifndef P2P_NETWORK_DOWNLOADWORKER_H
#define P2P_NETWORK_DOWNLOADWORKER_H
//created by Jakub

#include <thread>
#include <utility>
#include <database/Database.h>

#include <Torrent.h>
#include <networking/CSocket.h>

class DownloadWorker {
    Database database;
    std::shared_ptr<File> file;
    std::shared_ptr<PeerInfo> peer;
    Torrent torrent;
    bool finished;
public:

    DownloadWorker(Database database1, std::shared_ptr<File> file1, std::shared_ptr<PeerInfo> peer1):database(database1), file(std::move(file1)), peer(peer1) {
        syslogger->info("DownloadW for file {} created", file->getId());
    }
    
    ~DownloadWorker();
    std::shared_ptr<PeerInfo> getPeer();
    std::thread startWorker();
    bool isDone();
    void work();
};


#endif //P2P_NETWORK_DOWNLOADWORKER_H