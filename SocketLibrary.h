#ifndef _SOCKETLIBRARY_h
#define _SOCKETLIBRARY_h

#include <stdio.h>      // stderr
#include <stdlib.h>     // exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <signal.h>     // signal()
#include <string.h>     // strcpy(), memset()
#include <arpa/inet.h>  // inet_ntoa()
#include <netinet/in.h> // htonl(), htons(), sockaddr_in
#include <sys/socket.h> // socket(), bind(), AF_INET

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"

struct server_type
{
    int socket_fd;
};

extern void initializeSocket();
extern void bindSocket();
extern void listenSocket();
extern void handleServerClose();
extern void setSocketOptions();
extern void getIPAddress();

#endif /* _SOCKETLIBRARY_h */