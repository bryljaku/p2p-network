#include "sharedUtils.h"
#include "CSocket.h"
#include "RequestHandler.h"
#include "SSocket.h"

#define PORT 59095
#define TRACKER_MAX_MESSAGE_SIZE 128*1024	// in bytes

int main(int argc, char const *argv[]) {
	// initLogger wystarczy wywolac tylko raz na cale dzialanie programu
	initLogger("p2p-client");
	syslogger->info("p2p client starting");

	SSocket testTrackerSocket("127.0.0.1", PORT);
	testTrackerSocket.start();
	testTrackerSocket.sendOk();
}