#include "SocketLibrary.h" // socket functions

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        fprintf(stderr, "Not enough arguments.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "configure") == 0)
    {
    }
    else if (strcmp(argv[1], "checkout") == 0)
    {
    }
    else if (strcmp(argv[1], "update") == 0)
    {
    }
    else if (strcmp(argv[1], "upgrade") == 0)
    {
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
    }
    else if (strcmp(argv[1], "push") == 0)
    {
    }
    else if (strcmp(argv[1], "create") == 0)
    {
    }
    else if (strcmp(argv[1], "destroy") == 0)
    {
    }
    else if (strcmp(argv[1], "add") == 0)
    {
    }
    else if (strcmp(argv[1], "remove") == 0)
    {
    }
    else if (strcmp(argv[1], "currentversion") == 0)
    {
    }
    else if (strcmp(argv[1], "history") == 0)
    {
    }
    else if (strcmp(argv[1], "rollback") == 0)
    {
    }
    else
    {
        fprintf(stderr, "Command not found\n");
    }

    socketFunc();
    return 0;
}

#define PORT 9418
void socketFunc()
{
    struct server_type server; // declare struct.
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    initializeSocket(&server);

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    if (connect(server.socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }
    send(server.socket_fd, hello, strlen(hello), 0);
    printf("Message sent\n");
    // valread = read(sock, buffer, 1024);
    // printf("%s\n", buffer);
    return;
}