#include "iostream"
#include "pthread.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "database/Database.h"

int main() {
    spdlog::warn("server");
    auto db = new Database();
    std::vector<std::string> files;
    files.emplace_back("file1");
    db->addClient("192.192.192.1", files);
    files.emplace_back("file2");
    db->addClient("192.192.192.2", files);
    files.emplace_back("file3");
    db->addClient("192.192.192.3", files);
    auto addresses = db->getIpAddressesForFilename("file2");
    std::string msg;
    for(auto &i: addresses){
        msg += " " + i;
    }
    spdlog::info("addresses {}", msg);
    return 0;
}