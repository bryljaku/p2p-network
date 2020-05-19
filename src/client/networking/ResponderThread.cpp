#include <database/Database.h>
#include "ResponderThread.h"

#define SOCKET_DEFAULT_TIMEOUT 5			//TODO: zwiekszyc potem
#define CLIENT_MAX_MESSAGE_SIZE 128*1024	// in bytes

void ResponderThread::run(intptr_t connFd) {
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
}

void ResponderThread::respond(intptr_t connFd, TcpMessage *msg) {
	TcpCode code = msg->code();
	TcpMessage response;

	//TEST

	Database testdb;
	Torrent testTorrent(1337,10, "./path");
	testdb.addFile(File(1, 10, testTorrent, "./path"));

	//END TEST
	if(code == OK) {
		response.set_code(OK);
		sendTcpMsg(connFd, &response);
	} else if (code == CC_LIST_REQUEST) {
		response.set_code(CC_LIST_RESPONSE);
		auto lr = new ListResponse;
		int totalSegs = 0;
		for(auto s : testdb.getFiles()) {
			if (msg->listrequest().hashedtorrent() == s->getTorrent().hashed) {
				for(int i=0; i<s->getNumOfSegments(); i++) {
					if(s->getSegmentState(i) == SegmentState::COMPLETE) {
						lr->set_fragments(totalSegs++, i);
					}
				}
			}
		}
		lr->set_filecode(F_FRAG_COMPLETE);		// TODO: complete jak wysylamy liste kompletnych, missing jak listę brakujacych
		lr->set_hashedtorrent(msg->listrequest().hashedtorrent());
		response.set_allocated_listresponse(lr);
		sendTcpMsg(connFd, &response);
	} else if (code == CC_FRAGMENT_REQUEST) {
		response.set_code(CC_FRAGMENT_RESPONSE);
		auto fr = new FragmentResponse;
		fr->set_filecode(F_NO_FILE);
		for(auto s : testdb.getFiles()) {
			if(s->getTorrent().hashed == msg->fragmentrequest().hashedtorrent()) {
				if(s->getSegmentState(msg->fragmentrequest().fragnum()) == SegmentState::COMPLETE) {
					fr->set_filecode(F_FINE);
					fr->set_fragnum(msg->fragmentrequest().fragnum());
					fr->set_hashedtorrent(msg->mutable_fragmentrequest()->hashedtorrent());
					fr->set_fragment((const char*)s->getSegment(msg->fragmentrequest().fragnum()).getDataPtr());
				}
			}
		}
		response.set_allocated_fragmentresponse(fr);
		sendTcpMsg(connFd, &response);
	}
}
