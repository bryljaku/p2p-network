#ifndef P2P_NETWORK_DATABASE_H
#define P2P_NETWORK_DATABASE_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "ClientInfo.h"
#include "Torrent.h"

class Database {
    std::vector<ClientInfo> clients;
    std::vector<Torrent> torrents;
public:
    std::vector<IpV4Address> getIpV4AddressesForFilename(const Filename& filename);		//TODO moze lepiej nie leciec po filename tylko po Torrent
    std::vector<IpV6Address> getIpV6AddressesForFilename(const Filename& filename);
    
    std::vector<ClientInfo> getClients();
    void addClient(ClientInfo clientInfo);
    void deleteClient(const Id& clientId);

    bool isHashUnique(size_t hash);
    size_t addTorrent(Torrent t);		// returns new hash <- even if t has a set hash, it can be added with a different one (when there would be duplicate hashes)
};


#endif //P2P_NETWORK_DATABASE_H
