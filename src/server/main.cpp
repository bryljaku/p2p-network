#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include "sharedUtils.h"
#include "../../cmake-build-debug/src/client/TcpMessage.pb.h"

#define SERVER_DEFAULT_PORT 59095
#define SOCKET_DEFAULT_TIMEOUT 5	// in seconds TODO: zmienic na wyzszy timeout, bo tak to wywala klientow zaraz
#define CLIENT_MAX_MESSAGE_SIZE 128*1024	// in bytes (128*1024) = 128 KiB

int guard(int r, const std::string& err) {
	if (r == -1) {
		syslogger->error(err);
		perror(err.c_str());
		exit(1);
	}
	return r;
}

void respond(intptr_t connFd, TcpMessage *msg) {
	TcpCode code = msg->code();
	bool responseRequired = false;
	TcpMessage response;

	if(code == OK) {
		responseRequired = true;
		response.set_code(OK);
	}

	if(responseRequired) {
		sendTcpMsg(connFd, &response);
	}
}

void * trackerMainThread(void * arg) {
    intptr_t connFd = (uintptr_t) arg;

	std::string ipAddress = getConnectedIp(connFd);

    syslogger->debug("thread: serving " + ipAddress);
    std::string defaultTimeoutS = std::to_string(SOCKET_DEFAULT_TIMEOUT);


    char buf[CLIENT_MAX_MESSAGE_SIZE];
    for (;;) {
        int bytesReceived = recv(connFd, buf, sizeof(buf), 0);
        if (bytesReceived <= 0) {
        	syslogger->info("Client " + ipAddress + " disconnected.");
        	break;
        }
        std::string messageString(buf);
        TcpMessage message;
        message.ParseFromArray(buf, sizeof(buf));
        syslogger->debug("From: " + ipAddress + " Code: " + TcpCode_Name(message.code()));
        respond(connFd, &message);
    }

    guard(close(connFd),  "Could not close socket");
    syslogger->debug("thread: finished serving " + std::to_string(connFd));
    return NULL;
}

int main(int arg, char *argv[]) {
	initLogger("p2p-server");
	int port = SERVER_DEFAULT_PORT;
	for(;;) {
		switch(getopt(arg, argv, "p:")) {
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
		setsockopt(conn_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        pthread_t thread_id;
        int ret = pthread_create(&thread_id, NULL, trackerMainThread, (void*) conn_fd);
        if (ret != 0) {
        	syslogger->error("Error from pthread: %d\n", ret); exit(1);
        }
        syslogger->debug("main: created thread to handle connection " + std::to_string(conn_fd));
    }
    return 0;
}
