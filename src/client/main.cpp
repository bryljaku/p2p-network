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

#define PORT 59095
#define CLIENT_DEFAULT_PORT 59096
#define LISTENER_DEFAULT_TIMEOUT 5;

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
	Database database = Database();
	Torrent testTorrent(1337,10, "./path");
	File file  = File(testTorrent, "./path");
	file.addPeer(PeerInfo(1, "127.0.0.3", "", 9999));// gdy są peers to coś throwuje, obstawiam że to coś z jakimiś shared_ptr
	file.addPeer(PeerInfo(2, "127.0.0.2", "", 9992));
	file.addPeer(PeerInfo(3, "127.0.0.4", "", 9993));
	database.addFile(file);
	DownloadManager manager(database, database.getFile(1337));
	auto mngThread = manager.start_manager();
	mngThread.join();

	SSocket testTrackerSocket("127.0.0.1", PORT);
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
	//TODO: generate a new File from just a Torrent object
	return 0;
}

int main(int argc, char *argv[]) {
	initLogger("p2p-client");
	syslogger->info("p2p client starting");

	int port = CLIENT_DEFAULT_PORT;
	bool doTest = true;

	Database db;

	for(;;) {
		switch(getopt(argc, argv, "sp:a:")) {
			case 'p': {
				int potentialPort = (int) strtol(optarg, nullptr, 10);
				if(potentialPort<1024 || potentialPort>65535) {
					perror("Invalid port number");
					exit(1);
				}
				port = potentialPort;
				continue;
			}
			case 's':
				doTest = false;
				continue;
			case 'a': {
				int result = addTorrentFile(db, optarg);
				if (result == 0) {
					std::cout << "Added torrent file successfully";
				} else if (result == 1) {
					std::cout << "Can't open such a file";
				}
				continue;
			}
			case -1:
				break;
		}
		break;
	}

	if(doTest) {
		test();
	}

	connListen(port);

	return 0;
}