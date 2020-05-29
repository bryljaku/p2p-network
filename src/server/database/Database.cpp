#include "Database.h"
// created by Jakub
#include <utility>


void Database::addOrUpdateClient(ClientInfo clientInfo) {
	mutex.lock();
    for (auto& c: clients)
        if (clientInfo.getAddress() == c.getAddress()) {
            c.setTorrents(clientInfo.getTorrents());
            return;
        }
    clients.emplace_back(clientInfo);
	mutex.unlock();
}
void Database::deleteClient(const Id& clientId) {
    auto oldSize = clients.size();
    std::vector<ClientInfo>::iterator new_end;
    mutex.lock();
    new_end = std::remove_if(clients.begin(), clients.end(),
                             [clientId](const ClientInfo &client) { return client.getId() == clientId; });
    clients.erase(new_end, clients.end());
    
    if (clients.size() != oldSize)
        spdlog::info("Deleted client with id {}", clientId);
    else
        spdlog::info("Client with id {} not found. Not deleted", clientId);
    mutex.unlock();
}

std::vector<ClientInfo> Database::getClients() {
	mutex.lock_shared();
	std::vector<ClientInfo> ret = clients;
	mutex.unlock_shared();
    return ret;
}

bool Database::isHashUnique(size_t hash) {
	mutex.lock_shared();
	for(const auto& torrent : torrents)
	    if (torrent.hashed == hash)
			return false;
	mutex.unlock_shared();
	return true;
}

size_t Database::addTorrent(Torrent t) {
	uint32_t salt = 1;
	t.genDefaultHash();
	while(!isHashUnique(t.hashed)) {
		t.genSaltedHash(salt);
		salt++;
	}

	mutex.lock();
	torrents.push_back(t);
	mutex.unlock();

	return t.hashed;
}

std::vector<ClientInfo> Database::getClientsWith(Torrent torrent) {
    std::vector<ClientInfo> clientsToReturn;
	mutex.lock_shared();
    for (auto i: clients)
        if (i.hasTorrent(torrent))
            clientsToReturn.emplace_back(i);

    if (clientsToReturn.empty())
        syslogger->warn("no peers for torrent {}", torrent.hashed);
    mutex.unlock_shared();
    return clientsToReturn;
}

bool Database::hasTorrent(Torrent torrent) {
	mutex.lock_shared();
	for(auto t : torrents) {
		if (t == torrent) {
			return true;
		}
	}
	mutex.unlock_shared();
	return false;
}

