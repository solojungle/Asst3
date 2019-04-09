#ifndef _TCPSERVER_h
#define _TCPSERVER_h

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"

struct server_type
{
    int socket_fd;
};

/**
 *  Forward Declarations
 **/
void initializeSocket(struct server_type *);
void bindSocket(struct server_type *, struct sockaddr_in);
void listenSocket(struct server_type *, struct sockaddr_in, int, int);
void acceptSocketConnection(struct server_type *);

#endif /* _TCPSERVER_h */