#include "sharedUtils.h"

std::shared_ptr<spdlog::logger> syslogger;

void initLogger(std::string ident) {
	// w konsoli "tail -f /var/log/syslog" i powinno byc widac ze wchodzi
	// TODO: PONIZSZA WYWALIC, LOGUJE TYLKO DO STDOUT
	syslogger = spdlog::default_logger();
	// TODO: PRZYWROCIC PONIZSZA LINIJKE NA KONIEC (TERAZ LOGUJE TO STDOUT, A NIE SYSLOGA
	//syslogger = spdlog::syslog_logger_mt("syslog", ident, LOG_PID);
}

