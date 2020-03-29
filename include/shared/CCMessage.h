#ifndef P2P_NETWORK_CCMESSAGE_H
#define P2P_NETWORK_CCMESSAGE_H

// Client-Client message

#include "TcpMessage.h"
#include <vector>
#include <variant>

class CCMessage : public TcpMessage {
	std::vector<std::variant<int,std::string>> data;
public:
	void writeToBuf(void *buffer) override;
};


#endif //P2P_NETWORK_CCMESSAGE_H
