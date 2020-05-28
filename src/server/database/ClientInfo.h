#ifndef P2P_NETWORK_CLIENTINFO_H
#define P2P_NETWORK_CLIENTINFO_H
//created by Jakub
#include <utility>
#include <vector>
#include <string>
#include "GeneralTypes.h"

class ClientInfo {
    Id id;
    IpV4Address ipV4Address;
    IpV6Address ipV6Address;
    Port port;
    std::vector<Hash> torrentHashesToShare;


public:
    ClientInfo(Id id, IpV4Address ipV4, IpV6Address ipV6, Port port, std::vector<Hash> hashes) {
        this->id = id;
        this->torrentHashesToShare = std::move(hashes);
        this->ipV4Address = std::move(ipV4);
        this->ipV6Address = std::move(ipV6);
        this->port = port;
    }
    
    void setFilesToShare(std::vector<Hash> newHashes) {
        this->torrentHashesToShare = std::move(newHashes);
    }
    std::vector<Hash> getHashesToShare() {
        return this->torrentHashesToShare;
    }
    
    bool checkIfSharesTorrent(const Hash& hash) {
        for(auto& i: torrentHashesToShare)
            if(i == hash)
                return true;
        return false;
    }
    IpV4Address getIpV4Address() {
        return ipV4Address;
    }
    const Id getId() const {
        return id;
    }
    IpV6Address getIpV6Address() {
        return ipV6Address;
    }
    Port getPort() {
        return port;
    }

    void addFileToShare(Hash hash) {
        for (auto& f: torrentHashesToShare)
            if (f == hash) return;
        torrentHashesToShare.emplace_back(hash);
    }
    void deleteFileToShare(Hash hash) {
        torrentHashesToShare.erase(std::remove(torrentHashesToShare.begin(), torrentHashesToShare.end(), hash), torrentHashesToShare.end());
    }
};

#endif //P2P_NETWORK_CLIENTINFO_H