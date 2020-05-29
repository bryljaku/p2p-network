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
	Id id;
	bool isIpV4;
	IpAddress address;
	std::vector<Torrent> torrents;


public:
	ClientInfo(Id id, bool isIpV4, IpAddress address, std::vector<Torrent> torrents) {
		this->id = id;
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

	bool hasTorrent(Torrent torrent) {
		for(auto& i: torrents)
			if(i.hashed == torrent.hashed)
				return true;
		return false;
	}
	IpAddress getAddress() {
		return address;
	}
	Id getId() const {
		return id;
	}
	bool getIsIpV4() {
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
};

#endif //P2P_NETWORK_CLIENTINFO_H