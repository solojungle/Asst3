#ifndef _TCPSERVER_h
#define _TCPSERVER_h

typedef struct node // struct holding arguments to be used in thread function.
{
    int connection_fd;
    char *clientIP;
} thread_args;

/**
 *  Forward Declarations
 **/
void acceptSocketConnection(struct server_type *);
void handleClientInput(int);
void handleArguments(char *, int);
void *clientThread(void *);
void clean(void);
void sigIntHandler(int);

#endif /* _TCPSERVER_h */
