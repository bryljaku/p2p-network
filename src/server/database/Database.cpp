#include "Database.h"

#include <utility>

std::vector<std::string> Database::getIpAddressesForFilename(const std::string& filename) {
    std::vector<std::string> ipAddresses = std::vector<std::string>();
    for(auto &i : clients) {
        if(i.checkIfHasFile(filename))
            ipAddresses.push_back(i.getIpV4Address());
    }
    return ipAddresses;
}

void Database::addClient(ClientInfo clientInfo) {
    clients.emplace_back(clientInfo);
}

void Database::deleteClient() {
    spdlog::info("delete client");
}
