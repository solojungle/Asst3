#include <stdio.h>         // stderr
#include <unistd.h>        // close()
#include "SocketLibrary.h" // socket functions
#include "TCPServer.h"     // forward declarations

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

    setSocketOptions(&server); // prevent socket from failing to bind.

    bindSocket(&server, socketAddress); // bind socket to address, then check to see if socket has failed.

    listenSocket(&server, socketAddress, BACKLOG, PORT); // begin to listen on socket, check for failure.

    while (1) // begin accepting server connections.
    {
        acceptSocketConnection(&server); // handling a connection request.
    }

    return 0;
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
        return;
    }

    char clientIP[20];
    getClientIPAddress(connection_fd, clientIP); // client's ip address for better logs.

    printf("[%s+%s] %s has connected to the server.\n", GREEN, RESET, clientIP);

    while (1)
    {
        handleClientInput(connection_fd);
        break;
    }

    if (close(connection_fd) == -1)
    {
        fprintf(stderr, "Server has %sfailed%s to close a connection.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        handleServerClose(-1); // shutdown server correctly.
    }

    printf("[%s-%s] %s has disconnected from the server.\n", RED, RESET, clientIP);
    return;
}

void handleClientInput(int connection_fd)
{
    char buffer[80];
    memset(buffer, 0, sizeof(buffer));
    read(connection_fd, buffer, sizeof(buffer));
    printf("%sIssued command: %s%s\n", YELLOW, buffer, RESET);
    return;
}