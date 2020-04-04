#ifndef P2P_NETWORK_CLIENTINFO_H
#define P2P_NETWORK_CLIENTINFO_H
#include <utility>
#include <vector>
#include <string>
#include "../utils/GeneralTypes.h"

class ClientInfo {
    Id id;
    IpV4Address ipV4Address;
    IpV6Address ipV6Address;
    Port port;
    std::vector<Filename> filesToShare;


public:
    ClientInfo(Id id, IpV4Address ipV4, IpV6Address ipV6, Port port, std::vector<std::string> files) {
        this->id = id;
        this->filesToShare = std::move(files);
        this->ipV4Address = std::move(ipV4);
        this->ipV6Address = std::move(ipV6);
        this->port = std::move(port);
    }
    
    void setFilesToShare(std::vector<std::string> newFiles) {
        this->filesToShare = std::move(newFiles);
    }
    std::vector<Filename> getFilesToShare() {
        return this->filesToShare;
    }
    
    bool checkIfHasFile(const std::string& filename) {
        for(auto& i: filesToShare)
            if(i == filename)
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
    void addFileToShare(Filename filename) {
        for (auto& f: filesToShare)
            if (f == filename) return;
        filesToShare.emplace_back(filename);
    }
    void deleteFileToShare(Filename filename) {
        filesToShare.erase(std::remove(filesToShare.begin(), filesToShare.end(), filename), filesToShare.end());
    }
};

#endif //P2P_NETWORK_CLIENTINFO_H