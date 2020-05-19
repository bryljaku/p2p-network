#ifndef P2P_NETWORK_RESPONDERTHREAD_H
#define P2P_NETWORK_RESPONDERTHREAD_H

#include <string>
#include <sharedUtils.h>
#include <cstdint>

class ResponderThread {
public:
	void run(intptr_t t);
	void respond(intptr_t connFd, TcpMessage *msg);
};


#endif //P2P_NETWORK_RESPONDERTHREAD_H
