#include "iostream"
#include "pthread.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/syslog_sink.h"
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "TcpMessage.pb.h"
#include "sharedUtils.h"

#define PORT 43557

int main(int argc, char const *argv[]) {
	// initLogger wystarczy wywolac tylko raz na cale dzialanie programu
	initLogger("p2p-client");
	syslogger->info("p2p client starting");
    int sock = 0, valread;

    TcpMessage* t = new TcpMessage();
    t->set_code(TcpCode::UNKNOWN_ERROR);
    std::string test = t->SerializeAsString();
    TcpMessage z;
    z.ParseFromString(test);
    std::cout << z.code();

    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}