#include <stdio.h>
#include <stdlib.h>     // exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <sys/socket.h> // socket(), bind()
#include <netinet/in.h> // htonl(), htons()
#include <unistd.h>     // close()
#include "TCPServer.h"

int main(int argc, char *argv[])
{
    struct server_type server;        // declare struct.
    struct sockaddr_in socketAddress; // sockaddr_in, is struct that holds an IP socket address format.
    int PORT = 9418;                  // default port number.
    int BACKLOG = 5;                  // maximum amount of pending connections that can be enqueued for a socket.

    initializeSocket(&server); // create socket, then check to see if socket has failed.

    // initialize values for sockaddr_in.
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY); // binds the socket to all available interfaces (not just "localhost").
    socketAddress.sin_port = htons(PORT);              // set to port number (default 9418).

    bindSocket(&server, socketAddress); // bind socket, then check to see if socket has failed.

    listenSocket(&server, socketAddress, BACKLOG, PORT); // begin to listen on socket, check for failure.

    while (1) // begin accepting server connections.
    {
        acceptSocketConnection(&server); // how to handle a connection request.
    }

    return 0;
}

void initializeSocket(struct server_type *server)
{
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd == -1) // create socket, then check to see if socket has failed.
    {
        fprintf(stderr, "Socket has %sfailed%s to be created.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    printf("Socket has been %ssuccessfully%s created...\n", GREEN, RESET);
    return;
}

void bindSocket(struct server_type *server, struct sockaddr_in server_addr)
{
    if (bind(server->socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // bind socket, then check to see if socket has failed.
    {
        fprintf(stderr, "Socket has %sfailed%s to bind to an address.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    printf("Socket has %ssuccessfully%s binded to an address...\n", GREEN, RESET);
    return;
}

void listenSocket(struct server_type *server, struct sockaddr_in server_addr, int BACKLOG, int PORT)
{
    if (listen(server->socket_fd, BACKLOG) == -1) // begin to listen on socket, check for failure.
    {
        fprintf(stderr, "Socket has %sfailed%s to listen on port: %i.\nFILE: %s \nLINE: %d\n", RED, RESET, server_addr.sin_port, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    if (PORT == 9418)
    {
        printf("Server is now listening on port: %s%i%s (default).\n", GREEN, PORT, RESET);
    }
    else
    {
        printf("Server is now listening on port: %s%i%s.\n", GREEN, PORT, RESET);
    }

    return;
}

void acceptSocketConnection(struct server_type *server)
{
    int connection_fd;
    socklen_t clientLength;
    struct sockaddr_in clientAddress;

    clientLength = sizeof(clientAddress);
    connection_fd = accept(server->socket_fd, (struct sockaddr *)&clientAddress, &clientLength);

    if (connection_fd == -1)
    {
        fprintf(stderr, "Server has %sfailed%s to accept a connection.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    printf("[%s+%s] Client has connected to the server.\n", GREEN, RESET);

    if (close(connection_fd) == -1)
    {
        fprintf(stderr, "Server has %sfailed%s to close a connection.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    printf("[%s-%s] Client has disconnected from the server.\n", RED, RESET);
    return;
}