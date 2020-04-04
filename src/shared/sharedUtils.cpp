#include "sharedUtils.h"

std::shared_ptr<spdlog::logger> syslogger;

void initLogger(std::string ident) {
	// w konsoli "tail -f /var/log/syslog" i powinno byc widac ze wchodzi
	// TODO: PONIZSZA WYWALIC, LOGUJE TYLKO DO STDOUT
	syslogger = spdlog::default_logger();
	syslogger->set_level(spdlog::level::debug);
	// TODO: PRZYWROCIC PONIZSZA LINIJKE NA KONIEC (TERAZ LOGUJE TO STDOUT, A NIE SYSLOGA
	//syslogger = spdlog::syslog_logger_mt("syslog", ident, LOG_PID);
}

std::string getConnectedIp(intptr_t socketFd) {
	struct sockaddr_in connectedAddress;
	socklen_t peerLen = sizeof(connectedAddress);
	getpeername(socketFd, (sockaddr*) &connectedAddress, &peerLen);
	return inet_ntoa(connectedAddress.sin_addr);
}

int sendTcpMsg(intptr_t socketFd, TcpMessage *msg) {
	char outputBuf[msg->ByteSizeLong()];
	msg->SerializeToArray(outputBuf, msg->ByteSizeLong());
	int bytesSent = send(socketFd , outputBuf , msg->ByteSizeLong() , 0);
	if(bytesSent==-1) {
		syslogger->warn("Unable to respond to " + TcpCode_Name(msg->code()) + " from " + getConnectedIp(socketFd) + ".");
	}
	return bytesSent;
}

