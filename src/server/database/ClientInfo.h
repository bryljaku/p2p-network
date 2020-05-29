#ifndef P2P_NETWORK_CLIENTINFO_H
#define P2P_NETWORK_CLIENTINFO_H
//created by Jakub
#include <utility>
#include <vector>
#include <string>
#include <Torrent.h>
#include "GeneralTypes.h"

struct IpAddress {
	std::string ip;
	uint32_t port;

	IpAddress() {
		port = 0;
	}
	IpAddress(std::string ip, uint32_t port): ip(std::move(ip)), port(port) {};
	bool operator==(const IpAddress& other) {
		return this->ip == other.ip && this->port == other.port;
	}
};

class ClientInfo {
	bool isIpV4;
	IpAddress address;
	std::vector<Torrent> torrents;


public:
	ClientInfo(bool isIpV4, IpAddress address) {
		this->isIpV4 = isIpV4;
		this->address = std::move(address);
	}

	ClientInfo(bool isIpV4, IpAddress address, std::vector<Torrent> torrents) {
		this->isIpV4 = isIpV4;
		this->address = std::move(address);
		this->torrents = std::move(torrents);
	}

	void setTorrents(std::vector<Torrent> newTorrents) {
		this->torrents = std::move(newTorrents);
	}

	std::vector<Torrent> getTorrents() {
		return this->torrents;
	}

	bool hasTorrent(Torrent torrent) const {
		for(auto& i: torrents)
			if(i.hashed == torrent.hashed)
				return true;
		return false;
	}

	bool hasTorrent(Hash hash) const {
		for(auto& i: torrents)
			if(i.hashed == hash)
				return true;
		return false;
	}

	IpAddress getAddress() const {
		return address;
	}
	bool getIsIpV4() const {
		return isIpV4;
	}
	void addTorrent(const Torrent& torrent) {
		for (auto& f: torrents)
			if (f.hashed == torrent.hashed) return;
		torrents.emplace_back(torrent);
	}
	void deleteFileToShare(const Torrent& filename) {
		torrents.erase(std::remove(torrents.begin(), torrents.end(), filename), torrents.end());
	}

	bool operator==(const ClientInfo& other) const {
		return this->getAddress()==other.getAddress() && this->getIsIpV4()==other.getIsIpV4();
	}
};

#endif //P2P_NETWORK_CLIENTINFO_H