#ifndef P2P_NETWORK_CSOCKET_H
#define P2P_NETWORK_CSOCKET_H

#include <sharedUtils.h>

// client - client communication;
class CSocket {
	intptr_t sockFd;
	eSocketState state;

public:
	CSocket(intptr_t sockFd);
	//TODO: zrobic wszystko
};


#endif //P2P_NETWORK_CSOCKET_H
