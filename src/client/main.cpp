#include "iostream"
#include "pthread.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "database/Database.h"


int main() {
    spdlog::warn("client");
    auto db = Database();
    Id testId = 0;
    db.addFile(File(testId, 50000, "sys/path"));
    auto f = db.getFile(testId);
    db.removeFile(testId);
    return 0;
}