#ifndef P2P_NETWORK_SSOCKET_H
#define P2P_NETWORK_SSOCKET_H
#include <sys/socket.h>
#include <sys/types.h>
#include <utility>
#include <Torrent.h>
#include "sharedUtils.h"

#define TRACKER_MAX_MESSAGE_SIZE 128*1024	// in bytes
#define DEFAULT_RECV_TIMEOUT 5				// in seconds

// client - single tracker communication
class SSocket {
	intptr_t sockFd;	// socket descriptor
	fd_set sockSelectSet;
	struct timeval recvTimeout;
	std::string trackerIpString;
	uint trackerPort;

	char readBuffer[TRACKER_MAX_MESSAGE_SIZE] = {0};
	TcpMessage lastMsg;
	eSocketState state;

public:
	SSocket(std::string trackerIp, uint trackerPort);
	eSocketState start();

// TCP COMMUNICATION
	TcpCode sendOk();			// expected result: OK from server
	Ips sendSeedlistRequest();	//TODO: testowe, trzeba dodac jakis identyfikator torrenta jako argument do przeslania
	size_t sendNewTorrentRequest(Torrent t);	// returns 0 if adding was unsuccessful
private:
	void receive();				// starts listening for data - puts it in lastMsg
	void send(TcpMessage *m);
	void handleSendError();
	void handleRecvError();
	void logTcpCode(TcpMessage *m);
};


#endif //P2P_NETWORK_SSOCKET_H
