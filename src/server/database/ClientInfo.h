#include <utility>
#include <vector>
#include <string>

#ifndef P2P_NETWORK_CLIENTINFO_H
#define P2P_NETWORK_CLIENTINFO_H

#endif //P2P_NETWORK_CLIENTINFO_H
class ClientInfo {
    std::string ipV4Address;
    std::string ipV6Address;
    std::string port;
    std::vector<std::string> filesToShare;


public:
    ClientInfo(std::string ipV4, std::string ipV6, std::string port, std::vector<std::string> files, ) {
        this->filesToShare = std::move(files);
        this->ipV4Address = std::move(ipV4);
        this->ipV6Address = std::move(ipV6);
        this->port = std::move(port);
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
    std::string getIpV4Address() {
        return ipV4Address;
    }
    std::string getIpV6Address() {
        return ipV6Address;
    }
};
