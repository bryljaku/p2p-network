#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include "sharedUtils.h"

#define SERVER_DEFAULT_PORT 59095

int guard(int r, const std::string& err) {if (r == -1) { perror(err.c_str()); exit(1); } return r; }

void * thread_func(void * arg) {
    intptr_t conn_fd = (uintptr_t) arg;
    printf("thread: serving fd %ld\n", conn_fd);
    char buf[1024];
    for (;;) {
        int bytes_received = guard(recv(conn_fd, buf, sizeof(buf), 0), (char*) "Could not recv");
        if (bytes_received == 0) { goto stop_serving; }
        int bytes_sent = 0;
        while (bytes_sent < bytes_received) {
            ssize_t bytes_sent_this_call = send(conn_fd, buf+bytes_sent, bytes_received-bytes_sent, 0);
            if (bytes_sent_this_call == -1) { goto stop_serving; }
            bytes_sent += bytes_sent_this_call;
        }
    }
    stop_serving:
    guard(close(conn_fd), (char*) "Could not close socket");
    printf("thread: finished serving %ld\n", conn_fd);
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

				}
			}
				continue;
			case -1:
				break;
		}
		break;
	}
	std::string portS  = std::to_string(port);
	syslogger->info("Listening on port " + portS);
    int socketFd = guard(socket(AF_INET, SOCK_STREAM, 0), "Could not create TCP listening socket");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	socklen_t addr_bytes = sizeof(addr);

    guard(bind(socketFd, (struct sockaddr *) &addr, addr_bytes), "Could not bind socket to port " + portS);
    guard(listen(socketFd, 100), "Could not listen on port " + portS);

    std::cout << ("Listening on port %d\n", ntohs(addr.sin_port));
    for (;;) {
        intptr_t conn_fd = guard(accept(socketFd, NULL, NULL), "Could not accept");
        pthread_t thread_id;
        int ret = pthread_create(&thread_id, NULL, thread_func, (void*) conn_fd);
        if (ret != 0) { printf("Error from pthread: %d\n", ret); exit(1); }
        printf("main: created thread to handle connection %ld\n", conn_fd);
    }
    return 0;
}
