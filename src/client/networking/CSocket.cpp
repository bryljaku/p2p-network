#include "CSocket.h"

ListResponse CSocket::requestFragmentsList(const Torrent& torrent) {
	TcpMessage t;
	t.set_code(CC_LIST_REQUEST);
	auto lr = new ListRequest;
	lr->set_hashedtorrent(torrent.hashed);
	t.set_allocated_listrequest(lr);
	send(&t);

	std::vector<uint32_t> result;

	if(state == SENT) {
		receive();
		if(state == RECVD && lastMsg.code()==CC_LIST_RESPONSE) {
			ListResponse res = lastMsg.listresponse();
			if(res.filecode() == F_DENY) {				// other client doesnt allow downloading this torrent
				syslogger->warn(trackerIpString+" denied list request");
			} else if (res.filecode() == F_NO_FILE) {	// other client doesnt have that torrent anymore
				syslogger->warn(trackerIpString+" doesn't own " + torrent.fileName + " anymore");
			}
			return res;
		}
	}

	ListResponse err;
	err.set_filecode(F_ERROR);
	return err;
}

FragmentResponse CSocket::requestFragment(Torrent torrent, uint32_t fragNum) {
	TcpMessage t;
	t.set_code(CC_FRAGMENT_REQUEST);
	auto fr = new FragmentRequest;
	fr->set_hashedtorrent(torrent.hashed);
	fr->set_fragnum(fragNum);
	t.set_allocated_fragmentrequest(fr);
	send(&t);

	if(state == SENT) {
		receive();
		if (state == RECVD && lastMsg.code() == CC_FRAGMENT_RESPONSE) {
			return lastMsg.fragmentresponse();
		}
	}

	FragmentResponse err;
	err.set_filecode(F_ERROR);
	return err;
}
