#ifndef P2P_NETWORK_TRACKERTHREAD_H
#define P2P_NETWORK_TRACKERTHREAD_H

#include <cstdint>
#include <sharedUtils.h>
#include <Torrent.h>
#include "database/Database.h"

#define CLIENT_MAX_MESSAGE_SIZE 128*1024	// in bytes (128*1024) = 128 KiB

class TrackerThread {
	Database* db;

	void handleSeedNotification(IpAddress ad, uint64_t hashedTorrent);
	void handleUnavailables(Ips ips);
	void respond(intptr_t connFd, TcpMessage *msg);
public:
	void * run(intptr_t connFd, Database* db);
};


#endif //P2P_NETWORK_TRACKERTHREAD_H
