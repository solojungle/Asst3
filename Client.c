#include "SocketLibrary.h" // socket functions

void socketFunc();
void handleArguments();

int main(int argc, char *argv[])
{
    handleArguments(argc, argv);
    return 0;
}

void handleArguments(int argc, char *argv[])
{

    char string[100];                         // could segfault. (Need to dynamically allocate if function is needed).
    memset(string, '\0', sizeof(char) * 100); // need to memset, or else you get junk characters.

    if (argc < 2)
    {
        fprintf(stderr, "Not enough arguments.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "configure") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s configure <IP> <port>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "checkout") == 0)
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
    }
    else if (strcmp(argv[1], "upgrade") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s upgrade <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s commit <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "push") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s push <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "create") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s create <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "destroy") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s destroy <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s add <project name> <filename>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "remove") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s remove <project name> <filename>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "currentversion") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s currentversion <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "history") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s history <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "rollback") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Usage: %s rollback <project name> <version>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "Command not found\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "configure") == 0) // JUST FOR NOW TO PREVENT NULL.
    {
        printf("THIS COMMAND IS ONLY FOR CLIENT.\n");
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

    socketFunc(string);

    return;
}

#define PORT 9418
void socketFunc(char *argument)
{
    struct server_type server; // declare struct.
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

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

    send(server.socket_fd, argument, strlen(argument), 0);
    printf("Message sent\n");

    recv(server.socket_fd, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);
    return;
}