#ifndef P2P_NETWORK_CSOCKET_H
#define P2P_NETWORK_CSOCKET_H
// created by Piotr
#include <sharedUtils.h>
#include <Torrent.h>
#include "BaseSocket.h"

#define DEFAULT_RECV_TIMEOUT 5	//TODO: zmienić na wiecej

// client - client communication
class CSocket : public BaseSocket {
public:
	ListResponse requestFragmentsList(const Torrent& torrent);
	FragmentResponse requestFragment(Torrent torrent, uint32_t fragNums);
	CSocket(std::string ip, uint port);
};


#endif //P2P_NETWORK_CSOCKET_H
