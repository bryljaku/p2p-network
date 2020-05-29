#include "TrackerThread.h"

void TrackerThread::handleSeedNotification(IpAddress address, uint64_t hashedTorrent) {
	ClientInfo ci(true, address);
	for(auto t : *db->torrentVector())  {
		if(t.hashed == hashedTorrent) {
			db->addTorrentToClient(ci, t);
			return;
		}
	}
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
	} else if (code == CS_SEEDLIST_REQUEST) {
		response.set_code(CS_SEEDLIST_RESPONSE);
		auto t = new SeedlistResponse;			// delete(t) not needed, Protobuf does it when freeing response

		std::vector<ClientInfo> res = db->getClientsWith(msg->seedlistrequest().hashedtorrent());
		for(auto c : res) {
			if(c.getIsIpV4()) {
				IpPort *ipport = t->add_ipv4peers();
				ipport->set_ip(c.getAddress().ip);
				ipport->set_port(c.getAddress().port);
			} else {
				IpPort *ipport = t->add_ipv6peers();
				ipport->set_ip(c.getAddress().ip);
				ipport->set_port(c.getAddress().port);
			}
		}

		response.set_allocated_seedlistresponse(t);
		sendTcpMsg(connFd, &response);
	} else if (code == CS_NEW_REQUEST) {
		Torrent newTorrent(msg->newrequest().torrentmsg());
		newTorrent.genDefaultHash();

		db->addTorrent(newTorrent);
		ClientInfo ci(true, IpAddress(getConnectedIp(connFd), getConnectedPort(connFd)));
		db->addTorrentToClient(ci, newTorrent);

		response.set_code(CS_NEW_RESPONSE);
		auto t = new NewResponse;
		t->set_newhash(newTorrent.hashed);
		t->set_filecode(FileCode::F_FINE);
		response.set_allocated_newresponse(t);
		sendTcpMsg(connFd, &response);
	} else if (code == CS_IM_A_SEED) {
		IpAddress ad(getConnectedIp(connFd), getConnectedPort(connFd));
		handleSeedNotification(ad, msg->imaseed().hashedtorrent());
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

void * TrackerThread::run(intptr_t connFd, Database* db) {
	this->db = db;
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
