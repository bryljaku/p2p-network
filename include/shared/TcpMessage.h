#ifndef P2P_NETWORK_TCPMESSAGE_H
#define P2P_NETWORK_TCPMESSAGE_H

#include <string>
#include <cstring>

enum eTcpCode {
    UNKNOWN_ERROR, OK,
    CS_SEEDLIST_REQUEST,		// ask server for a list of users with a specified file
	CS_SEEDLIST_RESPONSE,
	CC_LIST_REQUEST,			// ask a client for list of his completed file fragments
	CC_LIST_RESPONSE,
	CC_FRAGMENT_REQUEST,		// request another client to send the selected fragment
	CC_FRAGMENT_RESPONSE
};
class TcpMessage {
public:
    eTcpCode code;
	TcpMessage();
	TcpMessage(eTcpCode _code);

	virtual void writeToBuf(void *buffer);
};


#endif //P2P_NETWORK_TCPMESSAGE_H
