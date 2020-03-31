#ifndef P2P_NETWORK_LOGGER_H
#define P2P_NETWORK_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"

extern std::shared_ptr<spdlog::logger> syslogger;

void initLogger(std::string ident);

#endif //P2P_NETWORK_LOGGER_H
