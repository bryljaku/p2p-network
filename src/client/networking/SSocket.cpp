#include "SSocket.h"

TcpCode SSocket::sendOk() {
	TcpMessage t;
	t.set_code(TcpCode::OK);
	send(&t);

	if(state == SENT) {
		receive();
		if(state == RECVD) {
			return lastMsg.code();
		}
	}

	return CONNERROR;
}



uint64_t SSocket::sendNewTorrentRequest(Torrent torrent) {
	TcpMessage t;
	t.set_code(TcpCode::CS_NEW_REQUEST);
	auto n = new NewRequest;
	auto msg = torrent.toMsg();
	n->set_allocated_torrentmsg(msg);
	t.set_allocated_newrequest(n);
	send(&t);

	if(state == SENT) {
		receive();
		if(state == RECVD && lastMsg.code()==CS_NEW_RESPONSE) {
			size_t recvdHash = lastMsg.newresponse().newhash();
			syslogger->debug("Received new hash: " + std::to_string(recvdHash));
			return recvdHash;
		}
	}
	return 0;
}

Ips SSocket::sendSeedlistRequest(uint64_t hashedTorrent) {
	TcpMessage t;
	t.set_code(TcpCode::CS_SEEDLIST_REQUEST);
	auto n = new SeedlistRequest;
	n->set_hashedtorrent(hashedTorrent);
	t.set_allocated_seedlistrequest(n);
	send(&t);

	if(state == SENT) {
		receive();
		if(state == RECVD && lastMsg.code()==CS_SEEDLIST_RESPONSE) {
			syslogger->debug(lastMsg.seedlistresponse().ipv4peers().at(0)); //TODO usunac bo test
			// TODO: zrobic strukture Ips z tych otrzymanych adresow
		}
	}
	return Ips();
}

SSocket::SSocket(std::string trackerIp, uint trackerPort) : BaseSocket(trackerIp, trackerPort) {
}
