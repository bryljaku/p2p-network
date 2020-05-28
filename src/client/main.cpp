#include <iostream>
#include <utility>
#include <pthread.h>
#include <unistd.h>
#include <sharedUtils.h>
#include "networking/ResponderThread.h"
#include "download/DownloadManager.h"
#include "networking/CSocket.h"
#include "networking/RequestHandler.h"
#include "networking/SSocket.h"
#include "file/PeerInfo.h"
#include "database/Database.h"

#define TRACKER_PORT 59095
#define TRACKER_ADDRESS "172.28.1.1"
#define CLIENT_DEFAULT_PORT 59096
#define LISTENER_DEFAULT_TIMEOUT 5;
#define CLIENT_SEED_TEST_ADDRESS "172.28.1.2"

void runMenu();

void * runResponderThread(void * arg) {
	intptr_t connFd = (uintptr_t) arg;
	ResponderThread res;
	res.run(connFd);
}

void connListen(int port) {
	std::string portS  = std::to_string(port);
	int socketFd = guard(socket(AF_INET, SOCK_STREAM, 0), "Could not create TCP listening socket");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	socklen_t addr_bytes = sizeof(addr);

	guard(bind(socketFd, (struct sockaddr *) &addr, addr_bytes), "Could not bind socket to port " + portS);
	guard(listen(socketFd, 100), "Could not listen on port " + portS);

	// default tracker-client socket timeout
//	struct timeval tv;
//	tv.tv_sec = LISTENER_DEFAULT_TIMEOUT;
//	tv.tv_usec = 0;

	syslogger->info("Listening on port " + std::to_string(ntohs(addr.sin_port)));
	for (;;) {
		intptr_t conn_fd = guard(accept(socketFd, NULL, NULL), "Could not accept");
//		setsockopt(conn_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
		pthread_t thread_id;
		int ret = pthread_create(&thread_id, NULL, runResponderThread, (void*) conn_fd);
		if (ret != 0) {
			syslogger->error("Error from pthread: %d\n", ret); exit(1);
		}
		syslogger->debug("main: created thread to handle connection " + std::to_string(conn_fd));
	}
}

void test() {
	auto database = std::make_shared<Database>();
	FileManager fileManager = FileManager(database);
	Torrent testTorrent(1337,10, "./path");
	File file  = File(testTorrent, "./path");
	file.addPeer(PeerInfo(1, CLIENT_SEED_TEST_ADDRESS, "", CLIENT_DEFAULT_PORT));
	database->addFile(file);
	SSocket sSocket(TRACKER_ADDRESS, TRACKER_PORT);
	DownloadManager manager(database, database->getFile(1337), fileManager, sSocket);
	auto mngThread = manager.start_manager();
	mngThread.join();

	SSocket testTrackerSocket(TRACKER_ADDRESS, TRACKER_PORT);
	testTrackerSocket.start();
	testTrackerSocket.sendOk();
	testTrackerSocket.sendSeedlistRequest(1);
	testTrackerSocket.sendNewTorrentRequest(testTorrent);
}

// 0 - ok, 1 - error opening file
int addTorrentFile(Database db, std::string filename) {
	Torrent nTor(std::move(filename));
	if(nTor.size == -1) {
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[]) {
	initLogger("p2p-client");
	syslogger->info("p2p client starting");

	int port = CLIENT_DEFAULT_PORT;
	std::string trackerIp = TRACKER_ADDRESS;
	//int trackerPort = TRACKER_PORT;

	bool doTest = true;

	Database db;

	int option;
	while((option = getopt(argc, argv, ":p:sa:t:a:")) != -1) {
    	switch(option) {
			case 'p': {
				int potentialPort = (int) strtol(optarg, nullptr, 10);
				if(potentialPort<1024 || potentialPort>65535) {
					perror("Invalid port number");
					exit(1);
				}
				port = potentialPort;
				break;
			}
			case 's':
				doTest = false;
				break;
			case 'a': {
				int result = addTorrentFile(db, optarg);
				if (result == 0) {
					std::cout << "Added torrent file successfully";
				} else if (result == 1) {
					std::cout << "Can't open such a file";
				}
				break;
			}
			case 't': {
				trackerIp = optarg;
				break;
			}
			case ':':
            	printf("option needs a value\n");
            	break;
         	case '?': //used for some unknown options
            	printf("unknown option: %c\n", optopt);
            	break;
		}
	}

	for(; optind < argc; optind++){ //when some extra arguments are passed
    	printf("Given extra arguments: %s\n", argv[optind]);
    }

	if(doTest) {
		test();
	}

	//runMenu();
	connListen(port);

	return 0;
}

void runMenu() {
	printf("___ Welcome to Concrete Torrent ___ \n\n");
	printf("Choose option: \n");
	printf("1. Add torrent to DB\n");
	printf("2. Create new local file\n");
	printf("3. Request download\n");
}