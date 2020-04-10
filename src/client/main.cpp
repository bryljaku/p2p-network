#include "iostream"
#include "pthread.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "database/Database.h"


int main() {
    spdlog::warn("client");
//    syslog_example();
    
//    auto db = Database();
    Id testId = 0;
    
//    db.addFile(File(testId, 50000, "sys/path"));
//    spdlog::info("{}", db.getFiles());
//    auto f = db.getFile(testId);
//    db.removeFIle(testId);
//    spdlog::info("{}", db.getFiles().size());
    
    return 0;
}