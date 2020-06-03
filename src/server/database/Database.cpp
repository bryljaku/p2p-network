#include "Database.h"
// created by Jakub

void Database::addTorrentToClient(ClientInfo& clientInfo, Torrent& torrent) {
	std::unique_lock<std::shared_mutex> lk(mutex);
	for(auto& c : clients) {
		if (c.getAddress() == clientInfo.getAddress()) {
			c.addTorrent(torrent);
			return;
		}
	}
	ClientInfo& ci = clients.emplace_back(clientInfo);
	ci.addTorrent(torrent);
}

void Database::deleteClient(const ClientInfo& client) {
    auto oldSize = clients.size();
    std::vector<ClientInfo>::iterator new_end;
	std::unique_lock<std::shared_mutex> lk(mutex);
    new_end = std::remove_if(clients.begin(), clients.end(),
                             [client](const ClientInfo &compared) { return client == compared; });
    clients.erase(new_end, clients.end());
    
    if (clients.size() != oldSize)
        syslogger->info("Deleted client with address {}", client.getAddress().ip, client.getAddress().port);
    else
        syslogger->info("Client with address {}:{} not found. Not deleted", client.getAddress().ip, client.getAddress().port);
}


bool Database::isHashUnique(size_t hash, bool lockAcquired) {
	if(!lockAcquired) {
		std::shared_lock<std::shared_mutex> lk(mutex);
	}
	for (const auto &torrent : torrents) {
		if (torrent.hashed == hash) {
			return false;
		}
	}
	return true;
}

size_t Database::addTorrent(Torrent t) {
	uint32_t salt = 1;
	t.genDefaultHash();

	std::unique_lock<std::shared_mutex> lk(mutex);
	while(!isHashUnique(t.hashed, true)) {
		t.genSaltedHash(salt);
		salt++;
	}
	torrents.push_back(t);
	return t.hashed;
}

std::vector<ClientInfo> Database::getClientsWith(Hash hash) {
	std::vector<ClientInfo> clientsToReturn;
	std::shared_lock<std::shared_mutex> lk(mutex);
	for (auto i: clients)
		if (i.hasTorrent(hash))
			clientsToReturn.emplace_back(i);

	if (clientsToReturn.empty())
		syslogger->warn("no peers for torrent {}", hash);
	return clientsToReturn;
}

bool Database::hasTorrent(Torrent torrent) {
	for(auto t : torrents) {
		if (t.hashed == torrent.hashed) {
			return true;
		}
	}
	return false;
}

std::vector<Torrent>* Database::torrentVector() {
	return &torrents;
}

