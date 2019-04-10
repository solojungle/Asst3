#include "SocketLibrary.h" // socket functions

#define PORT 9418

int main(int argc, char *argv[])
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
        return -1;
    }

    if (connect(server.socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(server.socket_fd, hello, strlen(hello), 0);
    printf("Message sent\n");
    // valread = read(sock, buffer, 1024);
    // printf("%s\n", buffer);
    return 0;
}