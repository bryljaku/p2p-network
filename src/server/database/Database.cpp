#include "Database.h"
// created by Jakub
#include <utility>


void Database::addOrUpdateClient(ClientInfo clientInfo) {
	mutex.lock();
    for (auto& c: clients)
        if (clientInfo.getAddress() == c.getAddress()) {
            c.setTorrents(clientInfo.getTorrents());
            mutex.unlock();
            return;
        }
    clients.emplace_back(clientInfo);
	mutex.unlock();
}

void Database::addTorrentToClient(ClientInfo& clientInfo, Torrent& torrent) {
	mutex.lock();
	for(auto c : clients) {
		if (c.getAddress() == clientInfo.getAddress()) {
			c.addTorrent(torrent);
			mutex.unlock();
			return;
		}
	}
	ClientInfo ci = clients.emplace_back(clientInfo);
	ci.addTorrent(torrent);
	mutex.unlock();
}

void Database::deleteClient(const ClientInfo& client) {
    auto oldSize = clients.size();
    std::vector<ClientInfo>::iterator new_end;
    mutex.lock();
    new_end = std::remove_if(clients.begin(), clients.end(),
                             [client](const ClientInfo &compared) { return client == compared; });
    clients.erase(new_end, clients.end());
    
    if (clients.size() != oldSize)
        spdlog::info("Deleted client with address {}", client.getAddress().ip, client.getAddress().port);
    else
        spdlog::info("Client with address {}:{} not found. Not deleted", client.getAddress().ip, client.getAddress().port);
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
	for (const auto &torrent : torrents) {
		if (torrent.hashed == hash) {
			mutex.unlock_shared();
			return false;
		}
	}

	mutex.unlock_shared();
	return true;
}

size_t Database::addTorrent(Torrent t) {
	uint32_t salt = 1;
	t.genDefaultHash();

	mutex.lock();
	while(!isHashUnique(t.hashed)) {
		t.genSaltedHash(salt);
		salt++;
	}
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

std::vector<ClientInfo> Database::getClientsWith(Hash hash) {
	std::vector<ClientInfo> clientsToReturn;
	mutex.lock_shared();
	for (auto i: clients)
		if (i.hasTorrent(hash))
			clientsToReturn.emplace_back(i);

	if (clientsToReturn.empty())
		syslogger->warn("no peers for torrent {}", hash);
	mutex.unlock_shared();
	return clientsToReturn;
}

bool Database::hasTorrent(Torrent torrent) {
	mutex.lock_shared();
	for(auto t : torrents) {
		if (t == torrent) {
			mutex.unlock_shared();
			return true;
		}
	}
	mutex.unlock_shared();
	return false;
}

std::vector<Torrent>* Database::torrentVector() {
	return &torrents;
}

