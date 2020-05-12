//created by Piotr
#include "RequestHandler.h"
#define MAX_ACCEPT_QUEUE 100

int RequestHandler::openListener(std::string ip, uint port) {

	std::string portS = std::to_string(port);
	acceptorFd = socket(AF_INET, SOCK_STREAM, 0);
	if(acceptorFd == -1) {
		syslogger->error("Could not create socket!");
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	socklen_t addr_bytes = sizeof(addr);

	if(bind(acceptorFd, (struct sockaddr *) &addr, addr_bytes) == -1) {
		syslogger->error("Could not bind socket to local address!");
		return -1;
	}

	if(listen(acceptorFd, MAX_ACCEPT_QUEUE) == -1) {
		syslogger->error("Could not listen on: " + ip + ":" + portS);
		return -1;
	}

	// default tracker-client socket timeout
//	struct timeval tv;
//	tv.tv_sec = SOCKET_DEFAULT_TIMEOUT;
//	tv.tv_usec = 0;

	syslogger->info("Listening on port " + std::to_string(ntohs(addr.sin_port)));
	for (;;) {
		intptr_t conn_fd = guard(accept(acceptorFd, NULL, NULL), "Could not accept");
		//TODO: raczej to niżej wywalic, tu chyba nie chcemy tworzyc watkow
//		setsockopt(conn_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
//		pthread_t thread_id;
//		int ret = pthread_create(&thread_id, NULL, addClient, (void*) acceptorFd);
//		if (ret != 0) {
//			syslogger->error("Error from pthread: %d\n", ret); exit(1);
//		}
//		syslogger->debug("main: created thread to handle connection " + std::to_string(conn_fd));
	}
	return 0;
}
