#include "TcpMessage.h"

TcpMessage::TcpMessage() {
	code = OK;
}

TcpMessage::TcpMessage(eTcpCode _code) {
	code = _code;
}

void TcpMessage::writeToBuf(void *buffer) {
	memcpy(buffer, &code, sizeof(eTcpCode));
}
