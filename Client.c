#define PORT 9418
#include <unistd.h>        // For close()
#include "SocketLibrary.h" // socket functions
#include "WTFCommands.h"   // core functions

void sendArgument(char *);
void handleArguments(int, char **);

int main(int argc, char *argv[])
{
    handleArguments(argc, argv);

    return 0;
}

void handleArguments(int argc, char *argv[])
{
    char string[256];                       // could segfault. (Need to dynamically allocate if function is needed).
    memset(string, '\0', (sizeof(string))); // need to memset, or else you get junk characters.

    if (argc < 2) // check to make an argument was passed.
    {
        fprintf(stderr, "Not enough arguments.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "checkout") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s checkout <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        strcpy(string, "1"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "update") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s update <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "2"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "upgrade") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s upgrade <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "3"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s commit <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "4"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "push") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s push <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "5"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "create") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s create <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "6"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "destroy") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s destroy <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "7"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s add <project name> <filename>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "8"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "remove") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s remove <project name> <filename>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "9"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "currentversion") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s currentversion <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "10"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "history") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s history <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "11"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "rollback") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s rollback <project name> <version>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "12"); // Convert name to number (easier on server end).
    }
    else if (strcmp(argv[1], "configure") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s configure <IP> <port>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        makeDirectory();                // create directory if it doesn't already exist.
        createConfig(argv[2], argv[3]); // overwrite config if it already exists.

        strcpy(string, "13"); // Convert name to number (easier on server end).
    }
    else
    {
        fprintf(stderr, "Command not found\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(string, "13") == 0) // Configure is client sided.
    {
        return;
    }

    int i = 2;
    while (argv[i] != NULL)
    {
        string[strlen(string)] = ' ';
        string[strlen(string) + 1] = '\0'; // Works without this line, but added just to make sure it's portable.
        strcat(string, argv[i]);
        i += 1;
    }

    sendArgument(string);

    return;
}

void sendArgument(char *argument)
{
    struct server_info *serverInfo = getServerConfig(); // get IP + Port from config.

    if (serverInfo == NULL) // an error occured reading the config file.
    {
        fprintf(stderr, "Error: Server information is NULL.\n");
        free(serverInfo);
        exit(EXIT_FAILURE);
    }

    // struct sockaddr_in address;
    // int sock = 0, valread;
    int connection_status = 0;
    char buffer[1024] = {0};
    struct server_type server; // declare struct.
    struct sockaddr_in serv_addr;

    initializeSocket(&server);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverInfo->port);

    if (inet_pton(AF_INET, serverInfo->IP, &serv_addr.sin_addr) <= 0) // Convert IPv4 and IPv6 addresses from text to binary form.
    {
        printf("\nInvalid address/Address not supported \n");
        exit(EXIT_FAILURE);
    }

    printf("Attempting connection to %s, port %li...\n", serverInfo->IP, serverInfo->port); // show which port is being connected to.

    while (connection_status == 0)
    {
        if (connect(server.socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            connection_status = 0;
            printf("Connection Failed! Trying again...\n\n");
            sleep(3); // Wait for 3 seconds before attempting to connect
        }
        else
        {
            connection_status = 1;
            printf("Connection to server is %ssuccessful%s.\n", GREEN, RESET);
        }
    }

    send(server.socket_fd, argument, strlen(argument), 0);
    printf("Message sent.\n");

    recv(server.socket_fd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    close(server.socket_fd);

    printf("Disconnected from server.\n\n");

    free(serverInfo->IP); // free IP malloc.
    free(serverInfo);     // free struct afterwards.
    return;
}
