#ifndef P2P_NETWORK_CCMESSAGE_H
#define P2P_NETWORK_CCMESSAGE_H

// Client-Client message

#include "TcpMessage.h"
#include <vector>
#include <variant>
#include <MessageUtils.h>

using namespace std;
class CCMessage : public TcpMessage {
	vector<variant<int,string>> data;
public:
	CCMessage(eTcpCode code);
	size_t getRequiredBufSize() override;
	size_t writeToBuf(void *buffer) override;
};


#endif //P2P_NETWORK_CCMESSAGE_H
