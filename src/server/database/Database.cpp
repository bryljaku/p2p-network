#include "Database.h"

#include <utility>

std::vector<std::string> Database::getIpAddressesForFilename(const std::string& filename) {
    std::vector<std::string> ipAddresses = std::vector<std::string>();
    for(auto &i : clients) {
        if(i.checkIfHasFile(filename))
            ipAddresses.push_back(i.getIpAddress());
    }
    return ipAddresses;
}

bool Database::addClient(std::string ip, std::vector<std::string> files) {
    clients.emplace_back(ip, std::move(files));
    return true;
}