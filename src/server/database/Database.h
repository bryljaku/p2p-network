#ifndef P2P_NETWORK_DATABASE_H
#define P2P_NETWORK_DATABASE_H
// created by Jakub
#include "ClientInfo.h"
#include "Torrent.h"
#include <sharedUtils.h>
#include <shared_mutex>

class Database {
    std::vector<ClientInfo> clients;
    std::vector<Torrent> torrents;
    std::shared_mutex mutex;
public:
	std::vector<ClientInfo> getClientsWith(Hash hash);
	void addTorrentToClient(ClientInfo& clientInfo, Torrent& torrent);
	std::vector<Torrent>* torrentVector();
    bool isHashUnique(size_t hash, bool lockAcquired = false);
    size_t addTorrent(Torrent t);		// returns new hash <- even if t has a set hash, it can be added with a different one (when there would be duplicate hashes)
};


#endif //P2P_NETWORK_DATABASE_H
