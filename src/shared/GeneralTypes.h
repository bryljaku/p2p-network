#ifndef P2P_NETWORK_GENERALTYPES_H
#define P2P_NETWORK_GENERALTYPES_H
// created by Jakub
#include <string>

#define SOCKET_DEFAULT_TIMEOUT 500	// in seconds TODO: zmienic na wyzszy timeout, bo tak to wywala klientow zaraz
#define SERVER_DEFAULT_PORT 59095
#define CLIENT_DEFAULT_PORT 59096
#define DEFAULTSEGMENTSIZE 8000
typedef uint64_t Id;
typedef uint64_t Hash;
typedef std::string IpV4Address;
typedef std::string IpV6Address;
typedef int Port;
typedef std::string Filename;
#endif