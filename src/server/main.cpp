#include "iostream"
#include "pthread.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
void syslog_example()
{
    // w konsoli "tail -f /var/log/syslog" i powinno byc widac ze wchodzi
    std::string ident = "spdlog-example";
    auto syslog_logger = spdlog::syslog_logger_mt("syslog", ident, LOG_PID);
    syslog_logger->warn("This is warning that will end up in syslog.");
}
int main() {
    spdlog::warn("server");
    syslog_example();
    return 0;
}