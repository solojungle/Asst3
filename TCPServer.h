#ifndef _TCPSERVER_h
#define _TCPSERVER_h

/**
 *  Forward Declarations
 **/

typedef struct node{
    int connection_fd;
    char *clientIP;
}thread_args;

void acceptSocketConnection(struct server_type *);
void handleClientInput(int);
void handleArguments(char *);
void* clientThread(void *);

#endif /* _TCPSERVER_h */
