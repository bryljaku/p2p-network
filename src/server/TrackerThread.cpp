#include "TrackerThread.h"

void TrackerThread::handleSeedNotification(uint64_t hashedTorrent) {
	//TODO: dorobić
}


void TrackerThread::handleUnavailables(Ips ips) {
	//TODO: cos wymyslic
}

void TrackerThread::respond(intptr_t connFd, TcpMessage *msg) {
	TcpCode code = msg->code();
	TcpMessage response;

	if(code == OK) {
		response.set_code(OK);
		sendTcpMsg(connFd, &response);
	} else if (code == CS_SEEDLIST_REQUEST) {	//TODO: jak na razie tylko test, ale dziala
		response.set_code(CS_SEEDLIST_RESPONSE);
		auto t = new SeedlistResponse;			// delete(t) not needed, Protobuf does it when freeing response

		IpPort *testIp1 = t->add_ipv4peers();
		IpPort *testIp2 = t->add_ipv4peers();
		testIp1->set_ip("127.0.0.1");
		testIp1->set_port(123);
		testIp2->set_ip("192.168.0.1");
		testIp2->set_port(8000);

		response.set_allocated_seedlistresponse(t);
		syslogger->debug(response.SerializeAsString());
		sendTcpMsg(connFd, &response);
	} else if (code == CS_NEW_REQUEST) {
		Torrent newTorrent(msg->newrequest().torrentmsg());	//TODO: jeszcze musimy dodac do bazy danych
		newTorrent.genDefaultHash();
		response.set_code(CS_NEW_RESPONSE);
		auto t = new NewResponse;
		t->set_newhash(newTorrent.hashed);
		response.set_allocated_newresponse(t);
		sendTcpMsg(connFd, &response);
	} else if (code == CS_IM_A_SEED) {
		handleSeedNotification(msg->imaseed().hashedtorrent());
	} else if (code == CS_CLIENT_UNAVAILABLE) {
		Ips ips;
		for(auto ip4 : msg->clientunavailable().ipv4addresses()) {
			ips.ipv4s.push_back(ip4);
		}
		for(auto ip6 : msg->clientunavailable().ipv6addresses()) {
			ips.ipv6s.push_back(ip6);
		}
		handleUnavailables(ips);
	}
}

void * TrackerThread::run(intptr_t connFd) {
	std::string ipAddress = getConnectedIp(connFd);

	syslogger->debug("thread: serving " + ipAddress);
	std::string defaultTimeoutS = "TODO: display timeout seconds"; // TODO

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
