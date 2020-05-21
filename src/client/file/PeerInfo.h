#ifndef P2P_NETWORK_PEERINFO_H
#define P2P_NETWORK_PEERINFO_H
//created by Jakub

#include <vector>
#include "GeneralTypes.h"

class PeerInfo {
    Id id;
    IpV4Address ipV4Address;
    IpV6Address ipV6Address;
    Port port;
public:
    PeerInfo(Id id, IpV4Address ipV4, IpV6Address ipV6, Port port) {
        this->id = id;
        this->ipV4Address = std::move(ipV4);
        this->ipV6Address = std::move(ipV6);
        this->port = std::move(port);
    }
    Id getId() const {
        return id;
    }
    IpV4Address getIpV4Address() {
        return ipV4Address;
    }
    IpV6Address getIpV6Address() {
        return ipV6Address;
    }
    Port getPort() {
        return port;
    }
};


#endif //P2P_NETWORK_PEERINFO_H
