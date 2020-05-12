#include "BaseSocket.h"

BaseSocket::BaseSocket(std::string trackerIpString, uint trackerPort):trackerIpString(std::move(trackerIpString)), trackerPort(trackerPort) {
	sockFd = 0;
}

eSocketState BaseSocket::start() {
	struct sockaddr_in serv_addr;

	if(trackerPort<1024 || trackerPort>65535) {
		syslogger->warn("Invalid port " + std::to_string(trackerPort));
		return INVALID_ADDRESS;
	}

	if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return CANNOT_CONNECT;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(trackerPort);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, trackerIpString.c_str(), &serv_addr.sin_addr)<=0) {
		syslogger->warn("Invalid or not supported address " + trackerIpString);
		return INVALID_ADDRESS;
	}

	if (connect(sockFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return CANNOT_CONNECT;
	}

	// setup set for select() later on
	FD_ZERO(&sockSelectSet);
	FD_SET(sockFd, &sockSelectSet);
	recvTimeout.tv_sec = DEFAULT_RECV_TIMEOUT;
	recvTimeout.tv_usec = 0;

	return OPEN;
}

void BaseSocket::receive() {
	int rv = select(sockFd + 1, &sockSelectSet, NULL, NULL, &recvTimeout);
	if (rv == -1) {			// select error
		state = ERROR;
//		throw SockException("Select error when receiving");
		//TODO: select() error co zrobic?
	} else if (rv == 0) {	// timeout
		state = TIMED_OUT;
		syslogger->warn("track("+trackerIpString+") no response, timed out");
//		throw SockException("Recv timeout");
		//TODO: powtorzyc? nie wiem
	} else {				// received something
		int bytesRead = read(sockFd, readBuffer, MAX_MESSAGE_SIZE);
		if(bytesRead == -1) {
			handleRecvError();
		}
		lastMsg.ParseFromArray(readBuffer, bytesRead);
		logTcpCode(&lastMsg);
		state = RECVD;
	}
}

void BaseSocket::send(TcpMessage *m) {
	if (sendTcpMsg(sockFd, m) == -1) {
		handleSendError();
	}
	state = SENT;
	//TODO: co zwraca?
}

void BaseSocket::logTcpCode(TcpMessage *m) {
	syslogger->debug("track(" + trackerIpString + ") code: " + TcpCode_Name(m->code()));
}

void BaseSocket::handleSendError() {
	int error_code;
	socklen_t error_code_size = sizeof(error_code);
	getsockopt(sockFd, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
	if(error_code == ECONNRESET) {
		syslogger->warn("Connection reset by host " + trackerIpString);
	} else {
		syslogger->warn("Unknown send error");
	}
}

void BaseSocket::handleRecvError() {
	//TODO obsluga dla roznych ERRNO ? może lepiej nie przesadzac
	state = ERROR;
	syslogger->warn("Unknown receive error");
}

