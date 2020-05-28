// created by Piotr
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

SeedlistResponse SSocket::sendSeedlistRequest(uint64_t hashedTorrent) {
	TcpMessage t;
	t.set_code(TcpCode::CS_SEEDLIST_REQUEST);
	auto n = new SeedlistRequest;
	n->set_hashedtorrent(hashedTorrent);
	t.set_allocated_seedlistrequest(n);
	send(&t);

	if(state == SENT) {
		receive();
		if(state == RECVD && lastMsg.code()==CS_SEEDLIST_RESPONSE) {
			syslogger->info(lastMsg.seedlistresponse().ipv4peers().Get(0).ip());
			return lastMsg.seedlistresponse();
		}
	}
	return SeedlistResponse();
}

SSocket::SSocket(std::string trackerIp, uint trackerPort) : BaseSocket(trackerIp, trackerPort) {
}

void SSocket::sendClientUnavailable(Ips ips) {
	TcpMessage t;
	t.set_code(CS_CLIENT_UNAVAILABLE);
	auto n = new ClientUnavailable;
	for(int i=0; ips.ipv4s.size(); i++) {
		n->set_ipv4addresses(i, ips.ipv4s[i]);
	}
	for(int i=0; ips.ipv6s.size(); i++) {
		n->set_ipv6addresses(i, ips.ipv6s[i]);
	}
	t.set_allocated_clientunavailable(n);
	send(&t);
}

void SSocket::sendImSeed(Torrent torrent) {
	TcpMessage t;
	t.set_code(CS_IM_A_SEED);
	auto n = new ImASeed;
	n->set_hashedtorrent(torrent.hashed);
	t.set_allocated_imaseed(n);
	send(&t);
}
