#include "Database.h"
// created by Jakub

void Database::addTorrentToClient(ClientInfo& clientInfo, Torrent& torrent) {
	std::unique_lock<std::shared_mutex> lk(mutex);
	for(auto& c : clients) {
		if (c.getAddress() == clientInfo.getAddress()) {
			c.addTorrent(torrent);
			syslogger->info("Updated client {} with torrent {}", c.getAddress().ip, torrent.hashed);
			return;
		}
	}
    ClientInfo& ci = clients.emplace_back(clientInfo);
    ci.addTorrent(torrent);
    syslogger->info("Added new client {} connected with torrent {}", ci.getAddress().ip, torrent.hashed);
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
	syslogger->info("Successfully added torrent {}", t.hashed);
	return t.hashed;
}

std::vector<ClientInfo> Database::getClientsWith(Hash hash) {
	std::vector<ClientInfo> clientsToReturn;
	std::shared_lock<std::shared_mutex> lk(mutex);
	for (const auto& i: clients)
		if (i.hasTorrent(hash))
			clientsToReturn.emplace_back(i);

	if (clientsToReturn.empty())
		syslogger->warn("no peers for torrent {}", hash);
	return clientsToReturn;
}

std::vector<Torrent>* Database::torrentVector() {
	return &torrents;
}

