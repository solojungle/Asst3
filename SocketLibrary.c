#include "SocketLibrary.h" // general socket functions
#include <libgen.h>

/*
 *  initializeSocket()
 *  @params: server_type *, struct that contains the server file descriptor.
 *  @returns: void.
 *  @comments: function creates and initializes a socket.
 */
void initializeSocket(struct server_type *server)
{
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd == -1) // create socket, then check to see if socket has failed.
    {
        fprintf(stderr, "Socket has %sfailed%s to be created.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        exit(EXIT_FAILURE); // shutdown server correctly.
    }

    printf("Socket has been %ssuccessfully%s created...\n", GREEN, RESET);
    return;
}

/*
 *  bindSocket()
 *  @params:
 *      server_type *, struct that contains the server file descriptor.
 *      sockaddr_in, struct that contains address family, IP address, and port number of socket.
 *  @returns: void.
 *  @comments: function binds a socket to an available IP address and port number.
 */
void bindSocket(struct server_type *server, struct sockaddr_in server_addr)
{
    if (bind(server->socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // bind socket, then check to see if socket has failed.
    {
        fprintf(stderr, "Socket has %sfailed%s to bind to an address.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        exit(EXIT_FAILURE); // shutdown server correctly.
    }

    printf("Socket has %ssuccessfully%s bound to an address...\n", GREEN, RESET);
    return;
}

/*
 *  listenSocket()
 *  @params:
 *      server_type *, struct that contains the server file descriptor.
 *      sockaddr_in, struct that contains address family, IP address, and port number of socket.
 *      int, int that defines the maximum length to which the queue of pending connections for sockfd may grow.
 *  @returns: void.
 *  @comments: function sets socket to start listening for connections.
 */
void listenSocket(struct server_type *server, struct sockaddr_in server_addr, int BACKLOG)
{
    if (listen(server->socket_fd, BACKLOG) == -1) // begin to listen on socket, check for failure.
    {
        fprintf(stderr, "Socket has %sfailed%s to listen on port: %i.\nFILE: %s \nLINE: %d\n", RED, RESET, server_addr.sin_port, __FILE__, __LINE__);
        exit(EXIT_FAILURE); // shutdown server correctly.
    }

    // htons(), changes number based on endianess on machine, must ntohs() to display original port.
    printf("Server is now listening on port: %s%i%s.\n\n", GREEN, ntohs(server_addr.sin_port), RESET);

    return;
}

/*
 *  setSocketOptions()
 *  @params: server_type *, struct that contains the socket file descriptor.
 *  @returns: void.
 *  @comments: function currently only sets socket option REUSEADDR, which allows socket to use same port
 * with different IP addresses.
 */
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
    if (setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) // enables REUSEADDR.
    {
        fprintf(stderr, "SO_REUSEADDR has %sfailed%s to be enabled.\nFILE: %s \nLINE: %d\n", RED, RESET, __FILE__, __LINE__);
        return;
    }

    printf("SO_REUSEADDR has %ssuccessfully%s been enabled...\n", GREEN, RESET);
    return;
}

/*
 *  getIPAddress()
 *  @params:
 *      int, file descriptor of connection.
 *      char *, string to hold IP address.
 *  @returns: void.
 *  @comments: strcpy() the IP address of passed file descriptor into given string.
 */
void getIPAddress(int fd, char *IP)
{
    struct sockaddr_in clientAddress;
    socklen_t clientLength = sizeof(clientAddress);

    if (getpeername(fd, (struct sockaddr *)&clientAddress, &clientLength) == -1) // finds socket IP address.
    {
        fprintf(stderr, "Unable to find a valid IP Address.\n");
        strcpy(IP, "NULL");
        return;
    }

    strcpy(IP, inet_ntoa(clientAddress.sin_addr)); // copy address to passed string.
    return;
}

// creates a fresh node.
struct files_type *initializeFileNode(char *filename, int nameLength, char *file, int fileLength)
{
    struct files_type *temporary = (struct files_type *)malloc(sizeof(struct files_type));

    temporary->file = malloc(strlen(file + 1));
    temporary->filename = malloc(strlen(filename + 1));

    memset(temporary->file, '\0', strlen(file + 1));
    memset(temporary->filename, '\0', strlen(filename + 1));

    strcpy(temporary->file, file);
    strcpy(temporary->filename, filename);

    temporary->nameLength = nameLength;
    temporary->fileLength = fileLength;

    temporary->next = NULL;

    return temporary;
}

struct files_type *append(struct files_type *new, struct files_type *root)
{
    if (root == NULL) // if first node.
    {
        return new;
    }

    struct files_type *cursor = root;
    while (cursor->next != NULL) // find end.
    {
        cursor = cursor->next;
    }

    cursor->next = new; // append to list.

    return root;
}

void createFileList(char **files)
{

    if (files == NULL) // check to see if input is NULL.
    {
        fprintf(stderr, "%sError%s: Files is NULL.\n", RED, RESET);
        return;
    }

    if (files[0] == NULL) // see if files is empty.
    {
        fprintf(stderr, "%sError%s: Files is empty.\n", RED, RESET);
        return;
    }

    int index = 0;  // number of files + 1.
    int fd;         // general fd for files.
    int fileLength; // length of file.
    int nameLength; // file name length.

    struct files_type *root = NULL; // create starting point.

    while (files[index][0] != '\0') // files[index] will almost never be NULL, must compare to it's first char files[index][0].
    {
        fd = open(files[index], O_RDONLY); // open file from array.
        if (fd == -1)
        {
            fprintf(stderr, "%sError%s: File at \"%s\" does not exist.\n", RED, RESET, files[index]);
            close(fd); // close file.
            index += 1;
            continue;
        }

        char *fileName = basename(files[index]); // get filename.
        nameLength = strlen(fileName);           // get length of filename.

        fileLength = lseek(fd, 0, SEEK_END); // find files length with lseek().
        if (fileLength == -1)
        {
            fprintf(stderr, "%sError%s: Lseek failed to find end of file.\n", RED, RESET);
            close(fd); // close file.
            return;
        }

        lseek(fd, 0, SEEK_SET); // reset file offset.

        if (fileLength == 0)
        {
            fprintf(stderr, "%sWarning%s: File at \"%s\" is empty.\n", RED, RESET, files[index]);
        }

        char buffer[fileLength];              // create array (buffer) to hold file.
        memset(buffer, '\0', fileLength + 1); // remove junk memory.
        read(fd, buffer, fileLength);         // place file into buffer.

        root = append(initializeFileNode(fileName, nameLength, buffer, fileLength), root); // create list.

        close(fd);  // close file.
        index += 1; // increment cursor.
    }

    while (root != NULL) // DEBUGGING, print all nodes
    {
        printf("%i:%s:%i:%s\n", root->nameLength, root->filename, root->fileLength, root->file);
        root = root->next;
    }
}