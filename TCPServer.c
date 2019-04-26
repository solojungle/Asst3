#include <stdio.h>         // stderr
#include <unistd.h>        // close()
#include "SocketLibrary.h" // socket functions
#include "TCPServer.h"     // forward declarations
#include <pthread.h>       // thread library
#include <string.h>        // string library
#include <stdlib.h>        // standard library for atexit()
#include <signal.h>        // signal librariy

struct server_type server; // declare struct.
pthread_t threads[3000];   // holds thread identification numbers
int thread_index = 0;      // holds current index of thread
int all_connection_fd[3000];
int fd_index = 0;

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "%sError%s: please supply a port number!\n", RED, RESET);
        return -1;
    }
    
    struct sockaddr_in socketAddress;           // sockaddr_in, is struct that holds an IP socket address format.
    int BACKLOG = 5;                            // maximum amount of pending connections that can be enqueued for a socket.
    const int PORT = strtol(argv[1], NULL, 10); // given port number.
    
    signal(SIGINT, sigIntHandler); // handles when CTRL + C is pressed.
    
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
    
    atexit(clean); // clean upon exiting.
    
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
    thread_args *args = malloc(sizeof(thread_args));
    
    while (1) // loop to keep accepting connections.
    {
        pthread_attr_t tattr;
        pthread_t thread_id;
        clientLength = sizeof(clientAddress);
        connection_fd = accept(server->socket_fd, (struct sockaddr *)&clientAddress, &clientLength); // create new fd for client connection. Wait here for client to connect
        
        if (connection_fd == -1)
        {
            fprintf(stderr, "Server has %sfailed%s to accept a connection.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
            return;
        }
        
        all_connection_fd[fd_index] = connection_fd;
        ++fd_index;
        
        char clientIP[20];                     // holds ip address.
        memset(clientIP, '\0', 20);            // remove junk.
        getIPAddress(connection_fd, clientIP); // client's ip address for better logs.
        
        args->connection_fd = connection_fd;
        args->clientIP = (char *)malloc(strlen(clientIP) * sizeof(char)); // do we have to malloc here? we can use strlen()?
        strcpy(args->clientIP, clientIP);
        
        printf("[%s+%s] %s has connected to the server.\n", GREEN, RESET, clientIP);
        
        threads[thread_index] = thread_id; // Update log of threads
        ++thread_index;                    // Increment thread counter
        pthread_attr_init(&tattr); // Initialize thread attribute
        pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED); // Set thread attribute to detached
        pthread_create(&thread_id, &tattr, clientThread, (void *)args); // Create new detached thread for new client
    }
    
    free(args); // Free thread arguments
    
    return;
}

/**
 *  clientThread()
 *  @params: void*, a struct holding the connection_fd and clientIP.
 *  @returns: NULL.
 *  @comments: function is used to handle each client's requests in a different thread than the main thread.
 **/
void *clientThread(void *args) // thread function where clients get re-directed to.
{
    thread_args *arguments = args;
    int connection_fd = arguments->connection_fd;
    char *clientIP = (char *)malloc(strlen(arguments->clientIP) * sizeof(char));
    strcpy(clientIP, arguments->clientIP);
    
    handleClientInput(connection_fd); // pass client fd to handler.
    
    if (close(connection_fd) == -1)
    {
        fprintf(stderr, "Server has %sfailed%s to close a connection.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        exit(EXIT_FAILURE); // shutdown server correctly.
    }
    
    printf("[%s-%s] %s has disconnected from the server.\n", RED, RESET, clientIP);
    
    return NULL;
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
    char buffer[256];
    memset(buffer, 0, sizeof(buffer) - 1);
    char *success = "OK.\n"; // even if command doesn't exist need to confirm connection was valid.
    
    recv(connection_fd, buffer, sizeof(buffer), 0); // read() and recv() are almost interchangeable.
    
    handleArguments(buffer); // checks to see if argument exists and executes them.
    
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
        case 11: // history
            return "history";
        case 12: // rollback
            return "rollback";
        case 13: // configure
            return "configure";
        default:
            fprintf(stderr, "%sError%s: command not found\n", RED, RESET);
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
            create(tokens[1]);
            break;
        case 7: // destroy
            break;
        case 8: // add
            break;
        case 9: // remove
            break;
        case 10: // currentversion
            break;
        case 11: // history
            break;
        case 12: // rollback
            break;
        case 13: // configure
            break;
        default:
            fprintf(stderr, "%sError%s: command not found\n", RED, RESET);
            return;
    }
    
    printf("%sIssued command: %s%s\n", YELLOW, getCommandName(argument), RESET);
}

//
//         CTRL + C PROCEDURES
//=======================================
void clean(void) // instructions to be run before exiting.
{
    char *clientDisconnectMSG = "\nServer Disconnected.\n";
    int i;
    
    for (i = 0; i < fd_index; i++) // send disconnect message to all clients on server.
    {
        send(all_connection_fd[i], clientDisconnectMSG, strlen(clientDisconnectMSG), 0);
    }
    
    if (close(server.socket_fd) == 0)
        printf("\nSocket closed.\n");
    else
        fprintf(stderr, "\n%sError:%s closing socket has failed.\n", RED, RESET);
    
    printf("%sServer shutting down, closing all threads.%s\n", RED, RESET);
    
    for (i = 0; i < thread_index; i++)
    {
        pthread_kill(threads[i], SIGKILL);
    }
}

void sigIntHandler(int sig) // handler when CTRL + C is pressed.
{
    clean(); // calls cleaning instructions for terminating.
    exit(0); // exit.
}
