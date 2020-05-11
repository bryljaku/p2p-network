#ifndef P2P_NETWORK_BASESOCKET_H
#define P2P_NETWORK_BASESOCKET_H

#include <cstdint>
#include <sys/select.h>
#include <string>
#include <TcpMessage.pb.h>
#include <sharedUtils.h>

#define MAX_MESSAGE_SIZE 128*1024	// in bytes
#define DEFAULT_RECV_TIMEOUT 5				// in seconds

class BaseSocket {
protected:
	intptr_t sockFd;	// socket descriptor
	fd_set sockSelectSet;
	struct timeval recvTimeout;
	std::string trackerIpString;
	uint trackerPort;

	char readBuffer[MAX_MESSAGE_SIZE] = {0};
	TcpMessage lastMsg;
	eSocketState state;

public:
	BaseSocket(std::string trackerIp, uint trackerPort);
	eSocketState start();

protected:
	void receive();				// starts listening for data - puts it in lastMsg
	void send(TcpMessage *m);
	void handleSendError();
	void handleRecvError();
	void logTcpCode(TcpMessage *m);
};


#endif //P2P_NETWORK_BASESOCKET_H
