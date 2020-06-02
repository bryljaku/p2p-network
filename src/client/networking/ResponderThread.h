#ifndef P2P_NETWORK_RESPONDERTHREAD_H
#define P2P_NETWORK_RESPONDERTHREAD_H

#include <string>
#include <sharedUtils.h>
#include <cstdint>
#include <database/Database.h>

class ResponderThread {
	Database* db;
	intptr_t connFd;
public:
	void run(intptr_t t, Database* db);
	void respond(TcpMessage *msg);
};


#endif //P2P_NETWORK_RESPONDERTHREAD_H
