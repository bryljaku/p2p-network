#ifndef P2P_NETWORK_LOGGER_H
#define P2P_NETWORK_LOGGER_H

#include <TcpMessage.pb.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern std::shared_ptr<spdlog::logger> syslogger;

void initLogger(std::string ident);
std::string getConnectedIp(intptr_t socketFd);
int sendTcpMsg(intptr_t connFd, TcpMessage *msg);
#endif //P2P_NETWORK_LOGGER_H
