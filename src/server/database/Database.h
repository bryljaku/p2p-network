#ifndef P2P_NETWORK_DATABASE_H
#define P2P_NETWORK_DATABASE_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "ClientInfo.h"

class Database {
    std::vector<ClientInfo> clients;
public:
    std::vector<IpV4Address> getIpV4AddressesForFilename(const Filename& filename);
    std::vector<IpV6Address> getIpV6AddressesForFilename(const Filename& filename);
    
    std::vector<ClientInfo> getClients();
    void addClient(ClientInfo clientInfo);
    void deleteClient(const Id& clientId);
};


#endif //P2P_NETWORK_DATABASE_H
