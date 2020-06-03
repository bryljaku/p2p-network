//created by Piotr
#ifndef P2P_NETWORK_REQUESTHANDLER_H
#define P2P_NETWORK_REQUESTHANDLER_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include "sharedUtils.h"
#include "CSocket.h"

//TODO: coś to ma robić, jak na razie bezuzyteczne
class RequestHandler {
    intptr_t acceptorFd;
    struct sockaddr_in addr;
    std::vector<CSocket> clients;

private:
    //void addClient(intptr_t clientFd);
public:
    int openListener(std::string ip, uint port);	// returns -1 if failed, 0 otherwise

};


#endif //P2P_NETWORK_REQUESTHANDLER_H