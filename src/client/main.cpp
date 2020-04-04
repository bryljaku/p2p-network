#include "iostream"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "TcpMessage.pb.h"
#include "sharedUtils.h"

#define PORT 59095

int main(int argc, char const *argv[]) {
	// initLogger wystarczy wywolac tylko raz na cale dzialanie programu
	initLogger("p2p-client");
	syslogger->info("p2p client starting");
    int sock = 0, valread;



    struct sockaddr_in serv_addr;
    //char *hello = "Hello from client";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    std::string trackerIp = getConnectedIp(sock);

    // test message
	TcpMessage t;
	t.set_code(TcpCode::OK);
    sendTcpMsg(sock, &t);

    valread = read( sock , buffer, 1024);
    t.ParseFromArray(buffer, 1024);
    syslogger->info("From: " + trackerIp + " Code: " + TcpCode_Name(t.code()));
    return 0;
}