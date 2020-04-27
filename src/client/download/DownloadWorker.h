#ifndef P2P_NETWORK_DOWNLOADWORKER_H
#define P2P_NETWORK_DOWNLOADWORKER_H


#include <thread>
#include <utility>
#include <src/client/database/Database.h>

class DownloadWorker {
    Database database;
    std::shared_ptr<File> file;
    std::shared_ptr<PeerInfo> peer;
public:
    DownloadWorker(
            Database &database,
            std::shared_ptr<File> file,
            std::shared_ptr<PeerInfo> peer
            ):
            database(database),
            file(std::move(file)),
            peer(std::move(peer)){}
    ~DownloadWorker() = default;
    
    std::thread startWorker();
    void connect();
    void work();
    void close_connection();
};


#endif //P2P_NETWORK_DOWNLOADWORKER_H
