#include "iostream"
#include "pthread.h"
#include <sharedUtils.h>
#include "networking/CSocket.h"
#include "networking/RequestHandler.h"
#include "networking/SSocket.h"

#define PORT 59095
#define TRACKER_MAX_MESSAGE_SIZE 128*1024	// in bytes

int main(int argc, char const *argv[]) {
	initLogger("p2p-client");
	syslogger->info("p2p client starting");

	SSocket testTrackerSocket("127.0.0.1", PORT);
	testTrackerSocket.start();
	testTrackerSocket.sendOk();
	testTrackerSocket.sendSeedlistRequest();
}