#ifndef P2P_NETWORK_CSMESSAGE_H
#define P2P_NETWORK_CSMESSAGE_H

// Client-Server message

#include "TcpMessage.h"
#include <vector>
#include <variant>

class CSMessage : public TcpMessage {
	std::vector<std::variant<int,std::string>> data;
public:
	size_t getRequiredBufSize() override;
	size_t writeToBuf(void *buffer) override;
};


#endif //P2P_NETWORK_CSMESSAGE_H
