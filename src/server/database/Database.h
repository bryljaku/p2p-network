#ifndef P2P_NETWORK_DATABASE_H
#define P2P_NETWORK_DATABASE_H
// created by Jakub
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "ClientInfo.h"
#include "Torrent.h"
#include <sharedUtils.h>

class Database {
    std::vector<ClientInfo> clients;
    std::vector<Torrent> torrents;
public:
    std::vector<ClientInfo> getClientsWith(Torrent torrent);

    std::vector<ClientInfo> getClients();
    void addOrUpdateClient(ClientInfo clientInfo);
    void deleteClient(const Id& clientId);

    bool isHashUnique(size_t hash);
    size_t addTorrent(Torrent t);		// returns new hash <- even if t has a set hash, it can be added with a different one (when there would be duplicate hashes)
};


#endif //P2P_NETWORK_DATABASE_H
