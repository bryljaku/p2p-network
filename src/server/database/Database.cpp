#include "Database.h"

#include <utility>

std::vector<IpV4Address> Database::getIpV4AddressesForFilename(const Filename& filename) {
    std::vector<IpV4Address> ipV4Addresses = std::vector<IpV4Address>();
    for(auto &i : clients)
        if(i.checkIfHasFile(filename))
            ipV4Addresses.push_back(i.getIpV4Address());
        
    return ipV4Addresses;
}

std::vector<IpV6Address> Database::getIpV6AddressesForFilename(const Filename &filename) {
    std::vector<IpV6Address> ipV6Addresses = std::vector<IpV6Address>();
    for(auto &i : clients)
        if(i.checkIfHasFile(filename))
            ipV6Addresses.push_back(i.getIpV6Address());
    
    return ipV6Addresses;
}
void Database::addClient(ClientInfo clientInfo) { // todo
    for (auto& c: clients)
        if (clientInfo.getIpV4Address() == c.getIpV4Address() || c.getIpV6Address() == clientInfo.getIpV6Address()) {
            c.setFilesToShare(clientInfo.getFilesToShare());
            return;
        }
    clients.emplace_back(clientInfo);
}

void Database::deleteClient(const Id& clientId) {
    auto oldSize = clients.size();
    std::vector<ClientInfo>::iterator new_end;
    new_end = std::remove_if(clients.begin(), clients.end(),
                             [clientId](const ClientInfo &client) { return client.getId() == clientId; });
    clients.erase(new_end, clients.end());
    
    if (clients.size() != oldSize)
        spdlog::info("Deleted client with id {}", clientId);
    else
        spdlog::info("Client with id {} not found. Not deleted", clientId);
}

std::vector<ClientInfo> Database::getClients() {
    return clients;
}

bool Database::isHashUnique(size_t hash) {
	for(auto torrent : torrents) {
		if (torrent.hashed == hash) {
			return false;
		}
	}
	return true;
}

size_t Database::addTorrent(Torrent t) {
	uint32_t salt = 1;
	t.genDefaultHash();
	while(!isHashUnique(t.hashed)) {
		t.genSaltedHash(salt);
		salt++;
	}

	torrents.push_back(t);

	return t.hashed;
}

