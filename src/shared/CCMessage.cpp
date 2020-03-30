#include "CCMessage.h"

CCMessage::CCMessage(eTcpCode code) {
	this->code = code;
}
size_t CCMessage::getRequiredBufSize() {
	return TcpMessage::getRequiredBufSize();
}

size_t CCMessage::writeToBuf(void *buffer) {
	size_t offset = TcpMessage::writeToBuf(buffer);

	for(auto& v : data) {
		offset += visit(VarVisitWriter(static_cast<char*>(buffer)+offset), v);
	}
}
