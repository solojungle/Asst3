#ifndef _TCPSERVER_h
#define _TCPSERVER_h

/**
 *  Forward Declarations
 **/

typedef struct node
{ // Struct holding arguments to be used in thread function
    int connection_fd;
    char *clientIP;
} thread_args;

void acceptSocketConnection(struct server_type *);
void handleClientInput(int);
void handleArguments(char *);
void *clientThread(void *);
void clean(void);
void sigIntHandler(int);

#endif /* _TCPSERVER_h */
