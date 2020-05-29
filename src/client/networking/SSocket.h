//created by Piotr
#ifndef P2P_NETWORK_SSOCKET_H
#define P2P_NETWORK_SSOCKET_H
#include <sys/socket.h>
#include <sys/types.h>
#include <utility>
#include <Torrent.h>
#include "sharedUtils.h"
#include "BaseSocket.h"

uint32_t GLOB_responder_port;

// client - single tracker communication
class SSocket : public BaseSocket {
public:
	SSocket(std::string trackerIp, uint trackerPort);
// TCP COMMUNICATION
	TcpCode sendOk();			// expected result: OK from server
	uint64_t sendNewTorrentRequest(Torrent torrent);	// returns 0 if adding was unsuccessful
	SeedlistResponse sendSeedlistRequest(uint64_t hashedTorrent);
	// no response required:
	void sendImSeed(Torrent torrent);
	void sendClientUnavailable(Ips ips);
};


#endif //P2P_NETWORK_SSOCKET_H
