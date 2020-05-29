#include "Database.h"
// created by Jakub
#include <utility>


void Database::addOrUpdateClient(ClientInfo clientInfo) {
    for (auto& c: clients)
        if (clientInfo.getAddress() == c.getAddress()) {
            c.setTorrents(clientInfo.getTorrents());
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
	for(const auto& torrent : torrents)
	    if (torrent.hashed == hash)
			return false;
		
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

std::vector<ClientInfo> Database::getClientsWith(Torrent torrent) {
    std::vector<ClientInfo> clientsToReturn;
    for (auto i: clients)
        if (i.hasTorrent(torrent))
            clientsToReturn.emplace_back(i);

    if (clientsToReturn.empty())
        syslogger->warn("no peers for torrent {}", torrent.hashed);
    return clientsToReturn;

}

