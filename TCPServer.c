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
    socketAddress.sin_family = AF_INET;                // AF_INET is an address family that is used to designate the type of addresses that your socket can communicate with.
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY binds the socket to all available interfaces (not just "localhost").
    socketAddress.sin_port = htons(PORT);              // set to port number (default 9418).

    setSocketOptions(&server); // prevent socket from failing to bind.

    bindSocket(&server, socketAddress); // bind socket to address, then check to see if socket has failed.

    listenSocket(&server, socketAddress, BACKLOG); // begin to listen on socket, check for failure.

    while (1) // begin accepting server connections.
    {
        acceptSocketConnection(&server); // handling a connection request.
    }

    return 0;
}

/**
 *  acceptSocketConnection()
 *  @params: server_type *, struct that contains the server file descriptor.
 *  @returns: void.
 *  @comments: function is passed the server file descriptor which is currently listening, accepts a client
 * connection, and passes a new file descriptor to client input handler, when handler returns, closes client connection.
 **/
void acceptSocketConnection(struct server_type *server)
{
    int connection_fd;
    socklen_t clientLength;
    struct sockaddr_in clientAddress;

    clientLength = sizeof(clientAddress);
    connection_fd = accept(server->socket_fd, (struct sockaddr *)&clientAddress, &clientLength); // create new fd for client connection.

    if (connection_fd == -1)
    {
        fprintf(stderr, "Server has %sfailed%s to accept a connection.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        return;
    }

    char clientIP[20];                     // holds ip address.
    getIPAddress(connection_fd, clientIP); // client's ip address for better logs.

    printf("[%s+%s] %s has connected to the server.\n", GREEN, RESET, clientIP);

    handleClientInput(connection_fd); // pass client fd to handler.

    if (close(connection_fd) == -1)
    {
        fprintf(stderr, "Server has %sfailed%s to close a connection.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        handleServerClose(-1); // shutdown server correctly.
    }

    printf("[%s-%s] %s has disconnected from the server.\n", RED, RESET, clientIP);
    return;
}

/**
 *  handleClientInput()
 *  @params: int, a file descriptor to a client connection.
 *  @returns: void.
 *  @comments: function is passed a file descriptor to a connection, reads data (commands) sent from a client,
 * passes data to argument handler, sends a message back to client to confirm reception of data.
 **/
void handleClientInput(int connection_fd)
{
    char buffer[80];
    memset(buffer, 0, sizeof(buffer));

    recv(connection_fd, buffer, sizeof(buffer), 0); // read() and recv() are almost interchangeable.

    handleArguments(buffer); // checks to see if argument exists and executes them.

    char *success = "SERVER: Data was received.\n";   // even if command doesn't exist need to confirm connection was valid.
    send(connection_fd, success, strlen(success), 0); // write() and send() are almost interchangeable.
    return;
}

// @TODO: ADD getCommandName() TO LIBRARY FILE. (.WTF library), CASE 12, IS ONLY ADDED TO BE USED W/ CLIENT.C
// WHEN PORTED TO LIB.

/**
 *  getCommandName()
 *  @params: int, the argument number given from client connection.
 *  @returns: char *, converted name of argument.
 *  @comments: function is passed argument number, returns argument name.
 **/
char *getCommandName(int n)
{
    switch (n)
    {
    case 1: // checkout
        return "checkout";
    case 2: // update
        return "update";
    case 3: // upgrade
        return "upgrade";
    case 4: // commit
        return "commit";
    case 5: // push
        return "push";
    case 6: // create
        return "create";
    case 7: // destroy
        return "destroy";
    case 8: // add
        return "add";
    case 9: // remove
        return "remove";
    case 10: // currentversion
        return "currentversion";
    case 11: // rollback
        return "rollback";
    case 12: // configure
        return "configure";
    default:
        fprintf(stderr, "Command not found\n");
        return NULL;
    }
}

/**
 *  handleArguments()
 *  @params: char *, data (string) recieved from client.
 *  @returns: void.
 *  @comments: function tokenizes string, checks to see if argument exists and executes them.
 **/
void handleArguments(char *arguments)
{
    char *tokens[4];
    char *end;
    int i = 0;

    tokens[i] = strtok(arguments, " "); // start off tokenizer.
    while (tokens[i] != NULL)
    {
        i += 1;
        tokens[i] = strtok(NULL, " "); // loop through tokens and append them to array.
    }

    long argument = strtol(tokens[0], &end, 10);

    switch (argument)
    {
    case 1: // checkout
        break;
    case 2: // update
        break;
    case 3: // upgrade
        break;
    case 4: // commit
        break;
    case 5: // push
        break;
    case 6: // create
        break;
    case 7: // destroy
        break;
    case 8: // add
        break;
    case 9: // remove
        break;
    case 10: // currentversion
        break;
    case 11: // rollback
        break;
    default:
        fprintf(stderr, "Command not found!\n");
        return;
    }

    printf("%sIssued command: %s%s\n", YELLOW, getCommandName(argument), RESET);
}