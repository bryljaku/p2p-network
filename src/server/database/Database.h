#ifndef P2P_NETWORK_DATABASE_H
#define P2P_NETWORK_DATABASE_H

#include <map>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "ClientInfo.h"

class Database {
    std::vector<ClientInfo> clients;
public:
    std::vector<std::string> getIpAddressesForFilename(const std::string& filename);
    
    void addClient(ClientInfo clientInfo);
    void deleteClient();
};


#endif //P2P_NETWORK_DATABASE_H
