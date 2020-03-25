#include <utility>
#include <vector>
#include <string>

#ifndef P2P_NETWORK_CLIENTINFO_H
#define P2P_NETWORK_CLIENTINFO_H

#endif //P2P_NETWORK_CLIENTINFO_H
class ClientInfo {
    std::vector<std::string> filesToShare;
    std::string ipAddress;

public:
    ClientInfo(std::string ip, std::vector<std::string> files) {
        this->filesToShare = std::move(files);
        this->ipAddress = std::move(ip);
    }
    
    void setFilesToShare(std::vector<std::string> newFiles) {
        this->filesToShare = std::move(newFiles);
    }
    
    bool checkIfHasFile(const std::string& filename) {
        for(auto& i: this->filesToShare)
            if(i == filename)
                return true;
        return false;
    }
    std::string getIpAddress() {
        return ipAddress;
    }
};
