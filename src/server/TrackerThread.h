#ifndef P2P_NETWORK_TRACKERTHREAD_H
#define P2P_NETWORK_TRACKERTHREAD_H

#include <cstdint>
#include <sharedUtils.h>
#include <Torrent.h>

#define CLIENT_MAX_MESSAGE_SIZE 128*1024	// in bytes (128*1024) = 128 KiB

class TrackerThread {
	void handleSeedNotification(uint64_t hashedTorrent);
	void handleUnavailables(Ips ips);
	void respond(intptr_t connFd, TcpMessage *msg);
public:
	void * run(void * arg);
};


#endif //P2P_NETWORK_TRACKERTHREAD_H
