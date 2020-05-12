#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sharedUtils.h>
#include "download/DownloadManager.h"
#include "networking/CSocket.h"
#include "networking/RequestHandler.h"
#include "networking/SSocket.h"
#include "file/PeerInfo.h"
#include "database/Database.h"

#define PORT 59095
#define CLIENT_DEFAULT_PORT 59096
#define SOCKET_DEFAULT_TIMEOUT 5			//TODO: zwiekszyc potem
#define CLIENT_MAX_MESSAGE_SIZE 128*1024	// in bytes

void respond(intptr_t connFd, TcpMessage *msg) {
	TcpCode code = msg->code();
	TcpMessage response;

	if(code == OK) {
		response.set_code(OK);
		sendTcpMsg(connFd, &response);
	} else if (code == CC_LIST_REQUEST) {
		response.set_code(CC_LIST_RESPONSE);
		auto lr = new ListResponse;
		lr->set_filecode(F_FRAG_COMPLETE);		// TODO: complete jak wysylamy liste kompletnych, missing jak listę brakujacych
		lr->set_hashedtorrent(msg->listrequest().hashedtorrent());
//		lr.set_fragments(index, numer fragmentu)	// TODO: powstawiac posiadane numery fragmentow
		response.set_allocated_listresponse(lr);
		sendTcpMsg(connFd, &response);
	} else if (code == CC_FRAGMENT_REQUEST) {
		response.set_code(CC_FRAGMENT_RESPONSE);
		auto fr = new FragmentResponse;
		fr->set_filecode(F_FINE);
		fr->set_fragnum(msg->fragmentrequest().fragnum());
		fr->set_hashedtorrent(msg->mutable_fragmentrequest()->hashedtorrent());
//		fr->set_fragment(tutaj bajty)	;			// TODO: wstawic bajty fragmentu
		response.set_allocated_fragmentresponse(fr);
		sendTcpMsg(connFd, &response);
	}
}

void * clientResponderMainThread(void * arg) {
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

int main(int argc, char *argv[]) {
	initLogger("p2p-client");
	syslogger->info("p2p client starting");

	int port = CLIENT_DEFAULT_PORT;
	int doTest = true;
	for(;;) {
		switch(getopt(argc, argv, "sp:")) {
			case 'p': {
				int potentialPort = (int) strtol(optarg, nullptr, 10);
				if(potentialPort<1024 || potentialPort>65535) {
					perror("Invalid port number");
					exit(1);
				}
			}
				continue;
			case 's':
				doTest = false;
				continue;
			case -1:
				break;
		}
		break;
	}

	if(doTest) {
		Database database = Database();
		File file  = File(1, 10, "./path");
//		file.addPeer(PeerInfo(1, "127.0.0.3", "", "9999"));// gdy są peers to coś throwuje, obstawiam że to coś z jakimiś shared_ptr 
//		file.addPeer(PeerInfo(2, "127.0.0.2", "", "9992"));
//		file.addPeer(PeerInfo(3, "127.0.0.4", "", "9993"));
        database.addFile(File(1, 10, "./path"));
        Torrent testTorrent(123, "test.txt");
        DownloadManager manager(database, file, testTorrent);
        auto mngThread = manager.start_manager();
        mngThread.join();
        SSocket testTrackerSocket("127.0.0.1", PORT);
        testTrackerSocket.start();
        testTrackerSocket.sendOk();
        testTrackerSocket.sendSeedlistRequest(1);
		testTrackerSocket.sendNewTorrentRequest(testTorrent);
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
		int ret = pthread_create(&thread_id, NULL, clientResponderMainThread, (void*) conn_fd);
		if (ret != 0) {
			syslogger->error("Error from pthread: %d\n", ret); exit(1);
		}
		syslogger->debug("main: created thread to handle connection " + std::to_string(conn_fd));
	}
	return 0;
}