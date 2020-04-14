#include "CSocket.h"

CSocket::CSocket(intptr_t sockFd) {
	this->sockFd = sockFd;
	this->state = OPEN;
}
