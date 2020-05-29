#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include "sharedUtils.h"
#include "TrackerThread.h"

struct trackerThreadArgs {
	uintptr_t connFd;
	Database* db;
};

void * runTrackerThread(void * args) {
	auto* tta = (trackerThreadArgs*) args;
	TrackerThread t;
	t.run(tta->connFd, tta->db);
}

int main(int argc, char *argv[]) {
	initLogger("p2p-server");
	int port = SERVER_DEFAULT_PORT;
	Database db;
	for(;;) {
		switch(getopt(argc, argv, "p:")) {
			case 'p': {
				int potentialPort = (int) strtol(optarg, nullptr, 10);
				if(potentialPort<1024 || potentialPort>65535) {
					perror("Invalid port number");
					exit(1);
				}
			}
				continue;
			case -1:
				break;
		}
		break;
	}
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
	struct timeval tv;
	tv.tv_sec = SOCKET_DEFAULT_TIMEOUT;
	tv.tv_usec = 0;

    syslogger->info("Listening on port " + std::to_string(ntohs(addr.sin_port)));
    for (;;) {
        intptr_t conn_fd = guard(accept(socketFd, NULL, NULL), "Could not accept");
//		setsockopt(conn_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        pthread_t thread_id;
        trackerThreadArgs args;
        args.connFd = conn_fd;
        args.db = &db;
        int ret = pthread_create(&thread_id, NULL, runTrackerThread, (void*) &args);
        if (ret != 0) {
        	syslogger->error("Error from pthread: %d\n", ret); exit(1);
        }
        syslogger->debug("main: created thread to handle connection " + std::to_string(conn_fd));
    }
    return 0;
}
