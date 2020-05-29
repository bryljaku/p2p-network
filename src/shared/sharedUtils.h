// created by Piotr
#ifndef P2P_NETWORK_LOGGER_H
#define P2P_NETWORK_LOGGER_H

#include <TcpMessage.pb.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "GeneralTypes.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>

extern std::shared_ptr<spdlog::logger> syslogger;

struct IpAddress {
	std::string ip;
	uint32_t port;

	IpAddress() {
		port = 0;
	}
	IpAddress(std::string ip, uint32_t port): ip(std::move(ip)), port(port) {};
	bool operator==(const IpAddress& other) {
		return this->ip == other.ip && this->port == other.port;
	}
};

enum eSocketState {
	ERROR,
	CANNOT_CONNECT,
	INVALID_ADDRESS,
	CLOSED,
	OPEN,
	TIMED_OUT,
	SENT,
	RECVD
};

class SockException: public std::exception {
private:
	std::string message_;
public:
	explicit SockException(std::string  message);
	virtual const char* what() const noexcept {
		return message_.c_str();
	}
};

struct Ips {
	std::vector<IpV4Address> ipv4s;
	std::vector<IpV6Address> ipv6s;
};

int guard(int r, const std::string& err);
void initLogger(std::string ident);
std::string getConnectedIp(intptr_t socketFd);
uint32_t getConnectedPort(intptr_t socketFd);
int sendTcpMsg(intptr_t connFd, TcpMessage *msg);
#endif //P2P_NETWORK_LOGGER_H
