#include "SocketLibrary.h" // general socket functions

void initializeSocket(struct server_type *server)
{
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd == -1) // create socket, then check to see if socket has failed.
    {
        fprintf(stderr, "Socket has %sfailed%s to be created.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        handleServerClose(-1); // shutdown server correctly.
    }

    printf("Socket has been %ssuccessfully%s created...\n", GREEN, RESET);
    return;
}

void bindSocket(struct server_type *server, struct sockaddr_in server_addr)
{
    if (bind(server->socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // bind socket, then check to see if socket has failed.
    {
        fprintf(stderr, "Socket has %sfailed%s to bind to an address.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        handleServerClose(-1); // shutdown server correctly.
    }

    printf("Socket has %ssuccessfully%s binded to an address...\n", GREEN, RESET);
    return;
}

void listenSocket(struct server_type *server, struct sockaddr_in server_addr, int BACKLOG, int PORT)
{
    if (listen(server->socket_fd, BACKLOG) == -1) // begin to listen on socket, check for failure.
    {
        fprintf(stderr, "Socket has %sfailed%s to listen on port: %i.\nFILE: %s \nLINE: %d\n", RED, RESET, server_addr.sin_port, __FILE__, __LINE__);
        handleServerClose(-1); // shutdown server correctly.
    }

    if (PORT == 9418)
    {
        printf("Server is now listening on port: %s%i%s (default).\n\n", GREEN, PORT, RESET);
    }
    else
    {
        printf("Server is now listening on port: %s%i%s.\n\n", GREEN, PORT, RESET);
    }

    return;
}

void handleServerClose(int signal)
{
    printf("%sServer shutting down, closing all threads.%s\n", RED, RESET);
    if (signal == -1)
    {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void setSocketOptions(struct server_type *server)
{
    /**
     *  Why the error exists (socket fails to bind):
     *  A socket that still has data to send will go into a state called TIME_WAIT
     *  when you close it. In that state it will wait until all pending data has been
     *  successfully sent or until a timeout is hit, in which case the socket is closed
     *  forcefully.
     * 
     *  Why SO_REUSEADDR (Socket Option REUSEADDR) works:
     *  The socket can be successfully bound unless there is a conflict with another socket
     *  bound to EXACTLY the same combination of source address and port.
     * 
     *  If SO_REUSEADDR is set for the socket you are trying to bind, another socket bound to
     *  the same address and port in state TIME_WAIT is simply ignored, after all its already
     *  "half dead", and your socket can bind to exactly the same address without any problem.
     **/
    int enable = 1;
    if (setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        fprintf(stderr, "SO_REUSEADDR has %sfailed%s to be enabled.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        return;
    }

    printf("SO_REUSEADDR has %ssuccessfully%s been enabled...\n", GREEN, RESET);
    return;
}

void getIPAddress(int fd, char *IP)
{
    struct sockaddr_in clientAddress;
    socklen_t clientLength = sizeof(clientAddress);
    int result = getpeername(fd, (struct sockaddr *)&clientAddress, &clientLength);

    if (getpeername(fd, (struct sockaddr *)&clientAddress, &clientLength) == -1)
    {
        fprintf(stderr, "Unable to get a valid IP Address.\n");
        strcpy(IP, "NULL");
        return;
    }

    strcpy(IP, inet_ntoa(clientAddress.sin_addr));
    return;
}
