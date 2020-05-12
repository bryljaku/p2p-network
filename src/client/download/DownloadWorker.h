#ifndef P2P_NETWORK_DOWNLOADWORKER_H
#define P2P_NETWORK_DOWNLOADWORKER_H


#include <thread>
#include <utility>
#include <database/Database.h>

#include <src/shared/Torrent.h>
#include <src/client/networking/CSocket.h>

class DownloadWorker {
    Database database;
    std::shared_ptr<File> file;
    std::shared_ptr<PeerInfo> peer;
    Torrent torrent;
    bool finished;
public:
    DownloadWorker(Database database1, std::shared_ptr<File> file1, Torrent torrent1, std::shared_ptr<PeerInfo> peer1):database(database1), file(std::move(file1)), torrent(std::move(torrent1)), peer(std::move(peer1)) {
        syslogger->info("DownloadW for file {} created", file->getId());}
    
    ~DownloadWorker();
    std::shared_ptr<PeerInfo> getPeer();
    std::thread startWorker();
    bool isDone();
    void work();
};


#endif //P2P_NETWORK_DOWNLOADWORKER_H