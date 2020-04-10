<<<<<<< HEAD
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
=======
#include "sharedUtils.h"
#include "networking/CSocket.h"
#include "networking/RequestHandler.h"
#include "networking/SSocket.h"

#define PORT 59095
#define TRACKER_MAX_MESSAGE_SIZE 128*1024	// in bytes

int main(int argc, char const *argv[]) {
	// initLogger wystarczy wywolac tylko raz na cale dzialanie programu
	initLogger("p2p-client");
	syslogger->info("p2p client starting");

	SSocket testTrackerSocket("127.0.0.1", PORT);
	testTrackerSocket.start();
	testTrackerSocket.sendOk();
>>>>>>> master
}