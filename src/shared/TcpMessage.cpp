#include "TcpMessage.h"

TcpMessage::TcpMessage() {
	code = OK;
}

TcpMessage::TcpMessage(eTcpCode _code) {
	code = _code;
}

size_t TcpMessage::getRequiredBufSize() {
	return sizeof(eTcpCode);
}

size_t TcpMessage::writeToBuf(void *buffer) {
	memcpy(buffer, &code, sizeof(eTcpCode));
	return getRequiredBufSize();
}
