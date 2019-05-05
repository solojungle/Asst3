#include "WTFCommands.h" // core functions
#include "Client.h"      // client functions
// #include "SocketLibrary.h" // receiveTar

int main(int argc, char *argv[])
{
    handleArguments(argc, argv);

    return 0;
}

/**
 *  handleArguments()
 *  @params:
 *      int, the amount of arguments
 *      char **, the console arguments
 *  @returns: void.
 *  @comments: handles the given arguments to client, will convert them to a number and send to server
 * excluding configure which is a client sided command.
 **/
void handleArguments(int argc, char *argv[])
{
    char string[256];                                 // could segfault. (Need to dynamically allocate if function is needed).
    memset(string, '\0', (sizeof(string)));           // need to memset, or else you get junk characters.
    char *command = (char *)malloc(3 * sizeof(char)); // Separate command for client handling
    if (command == NULL)
    {
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

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

        char path[200];
        memset(path, '\0', 200);
        strcpy(path, "./Projects/");
        strcat(path, argv[2]);
        strcat(path, "\0");

        printf("Path: %s\n", path);
        DIR *fd = opendir(path);

        if (fd != NULL)
        {
            fprintf(stderr, "%sError:%s The project already exists in the Projects/ folder\n", RED, RESET);
            closedir(fd);
            exit(EXIT_FAILURE);
        }

        closedir(fd);

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

        /* char *projectName = (char*)malloc(strlen(argv[2]) * sizeof(char));
         if(projectName == NULL){
         fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
         exit(EXIT_FAILURE);
         }
         strcpy(projectName, argv[2]);
         manageManifest(projectName);*/
    }
    else if (strcmp(argv[1], "upgrade") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: %s upgrade <project name>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(string, "3"); // Convert name to number (easier on server end).

        /* char *projectName = (char*)malloc(strlen(argv[2]) * sizeof(char));
         if(projectName == NULL){
         fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
         exit(EXIT_FAILURE);
         }
         strcpy(projectName, argv[2]);
         manageManifest(projectName);*/
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

        return; // configure is client sided.
        // strcpy(string, "13"); // Convert name to number (easier on server end).
    }
    else
    {
        fprintf(stderr, "Command not found\n");
        manageManifest(argv[2], 1);
        exit(EXIT_FAILURE);
    }

    strcpy(command, string); // Copy command to handle it on client side
    char *repo = argv[2];

    int i = 2;
    while (argv[i] != NULL)
    {
        string[strlen(string)] = ' ';
        string[strlen(string) + 1] = '\0'; // Works without this line, but added just to make sure it's portable.
        strcat(string, argv[i]);
        i += 1;
    }

    sendArgument(string, command, repo, argv);

    return;
}

/**
 *  sendArgument()
 *  @params: char *, the string version of argument.
 *  @returns: void.
 *  @comments: attempts a connection to server, sends argument to server,
 * tries every 3 seconds, on user to cancel.
 **/
void sendArgument(char *argument, char *command, char *repo, char *argv[])
{
    struct server_info *serverInfo = getServerConfig(); // get IP + Port from config.

    if (serverInfo == NULL) // an error occured reading the config file.
    {
        fprintf(stderr, "%sError%s: Server information is NULL.\n", RED, RESET);
        free(serverInfo);
        exit(EXIT_FAILURE);
    }

    // struct sockaddr_in address;
    // int sock = 0, valread;
    int connection_status = 0;
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

    char response_buff[5];
    memset(response_buff, '\0', sizeof(response_buff));
    char commandResponse[500];
    memset(commandResponse, '\0', sizeof(commandResponse));

    printf("\nWaiting for server response... ");
    if (recv(server.socket_fd, response_buff, sizeof(response_buff), 0) == -1)
    {
        fprintf(stderr, "%sError%s: There was an error receiving message from socket.\n", RED, RESET);
        return;
    }
    printf("%s\n", response_buff);

    if (strcmp(command, "1") == 0)
    { // Checkout
        recv(server.socket_fd, commandResponse, 41, 0);

        if (commandResponse[0] == 'O')
            receiveTar(server.socket_fd, repo, 3);
        else if (commandResponse[0] == 'E')
            printf("%s", commandResponse);
    }
    else if (strcmp(command, "2") == 0) // Update
    {
        update(argument, command, repo, server.socket_fd);
    }
    else if (strcmp(command, "3") == 0)
    { // Upgrade
    }
    else if (strcmp(command, "4") == 0)
    { // Commit
    }
    else if (strcmp(command, "5") == 0)
    { // Push
    }
    else if (strcmp(command, "6") == 0)
    { // Create
        recv(server.socket_fd, commandResponse, sizeof(commandResponse), 0);
        printf("%s%s%s", YELLOW, commandResponse, RESET);
        memset(response_buff, '\0', sizeof(response_buff));
        outputFiles(receiveFiles(server.socket_fd), repo, 1); // 1 indicates that the client is receiving
    }
    else if (strcmp(command, "7") == 0)
    { // Destroy
        recv(server.socket_fd, commandResponse, sizeof(commandResponse), 0);
        printf("%s%s%s", YELLOW, commandResponse, RESET);
    }
    else if (strcmp(command, "8") == 0)
    { // Add
    	add(argv[2], argv[3]);
    }
    else if (strcmp(command, "9") == 0)
    { // Remove
    }
    else if (strcmp(command, "10") == 0)
    { // Current Version
        struct files_type *manifest = receiveFiles(server.socket_fd);
        if (manifest == NULL)
        {
            fprintf(stderr, "%sError%s: Unable to grab the current version.\n", RED, RESET);
        }
        else
        {
            char *end;
            char *token = strtok(manifest->file, "\n");
            long version = strtol(token, &end, 10);

            if (end == token)
            {
                fprintf(stderr, "%sError%s: No digits were found\n", RED, RESET);
                return;
            }

            printf("%sCurrent version: %d%s\n", YELLOW, version, RESET);
        }
    }
    else if (strcmp(command, "11") == 0)
    { // History
    }
    else if (strcmp(command, "12") == 0)
    { // Rollback
    }
    else
        fprintf(stderr, "Command not found!\n");

    close(server.socket_fd);

    printf("Disconnected from server.\n\n");

    free(serverInfo->IP); // free IP malloc.
    free(serverInfo);     // free struct afterwards.
    return;
}

void update(char *argument, char *command, char *repo, int fd)
{
    int length = 9 + strlen(repo) + 11; // Projects/ + <project_name> + /.manifest\0
    char project_path[length];
    memset(project_path, '\0', length);

    strcpy(project_path, "Projects/");
    strcat(project_path, repo);
    strcat(project_path, "/.manifest");

    manageManifest(repo, 1);

    struct project_manifest *client_manifest = fetchManifest(project_path);
    struct files_type *server_file = receiveFiles(fd);
    struct project_manifest *server_manifest;

    // printf("file: %s\n", client_manifest->fileVersion);

    if (server_file == NULL || client_manifest == NULL)
    {
        fprintf(stderr, "%sError%s: Unable to grab the current version of .manifest.\n", RED, RESET);
        return;
    }
    else
    {
        if (mkdir(".temp", S_IRWXU | S_IRWXG) == -1)
        {
            fprintf(stderr, "%sError%s: Directory failed to be created.\n", RED, RESET);
            return;
        }

        int temp_length = 6 + strlen(server_file->filename) + 1;
        char temp_manifest_path[temp_length];
        memset(temp_manifest_path, '\0', temp_length);

        strcpy(temp_manifest_path, ".temp/"); // 6
        strcat(temp_manifest_path, server_file->filename);

        int wd = open(temp_manifest_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (wd == -1)
        {
            fprintf(stderr, "%sError%s: Could not open file.\n", RED, RESET);
            rmdir(".temp");
            return;
        }

        if (write(wd, server_file->file, server_file->file_length) == -1)
        {
            fprintf(stderr, "%sError%s: Could not write file.\n", RED, RESET);
            remove(temp_manifest_path);
            rmdir(".temp");
            return;
        }
        close(wd);

        if ((server_manifest = fetchManifest(temp_manifest_path)) == NULL) // fetch.
        {
            fprintf(stderr, "%sError%s: server_manifest is NULL\n", RED, RESET);
            remove(temp_manifest_path);
            rmdir(".temp");
            return;
        }

        if (remove(temp_manifest_path) != 0) // remove file.
        {
            fprintf(stderr, "%sError%s: temp_manifest_path failed to be removed.\n", RED, RESET);
            return;
        }

        if (rmdir(".temp") == -1) // remove dir.
        {
            fprintf(stderr, "%sError%s: Directory failed to be deleted.\n", RED, RESET);
            return;
        }

        // ====================================================================================================

        struct project_manifest *client_cursor = client_manifest;
        int numberOfFilesInClient = 0;

        while (client_cursor != NULL)
        {
            numberOfFilesInClient += 1;
            client_cursor = client_cursor->nextNode;
        }

        client_cursor = client_manifest;

        if (numberOfFilesInClient == 1 && client_cursor->hash == NULL)
        {
            fprintf(stderr, "%sWarning%s: Client .manifest is empty.\n", RED, RESET);
            return;
        }

        // ====================================================================================================

        struct project_manifest *server_cursor = server_manifest;
        int numberOfFilesInServer = 0;

        while (server_manifest != NULL)
        {
            numberOfFilesInServer += 1;
            server_manifest = server_manifest->nextNode;
        }

        server_manifest = client_manifest;

        if (numberOfFilesInServer == 1 && server_manifest->hash == NULL)
        {
            fprintf(stderr, "%sWarning%s: Server .manifest is empty.\n", RED, RESET);
            return;
        }

        // ====================================================================================================

        struct project_manifest *cursor = client_manifest;
        struct project_manifest *inner_cursor = server_manifest;

        int index = 0;
        int existsIn;
        int differentHash;

        struct project_manifest *notInServerManifest[numberOfFilesInClient + 1];                 // sizeof client manifest
        struct project_manifest *notInClientManifest[numberOfFilesInServer + 1];                 // sizeof server manifest
        struct project_manifest *hashIsDifferent[numberOfFilesInClient + numberOfFilesInServer]; // more efficient to do max(a, b), but this will never fail either.

        memset(notInServerManifest, '\0', numberOfFilesInClient + 1);
        memset(notInClientManifest, '\0', numberOfFilesInServer + 1);
        memset(hashIsDifferent, '\0', numberOfFilesInClient + numberOfFilesInServer + 1);

        while (cursor != NULL)
        {
            existsIn = 0;
            differentHash = 0;

            while (inner_cursor != NULL)
            {
                if (strcmp(inner_cursor->file, cursor->file) == 0) // same name
                {
                    existsIn = 1;
                    if (strcmp(inner_cursor->hash, cursor->hash) != 0)
                    {
                        differentHash = 1;
                    }
                    break;
                }

                inner_cursor = inner_cursor->nextNode;
            }

            if (!existsIn)
            {
                notInServerManifest[index] = cursor;
            }

            if (differentHash)
            {
                hashIsDifferent[index] = cursor;
            }

            index += 1;
            inner_cursor = server_manifest;
            cursor = cursor->nextNode;
        }

        // ====================================================================================================

        cursor = server_manifest;
        inner_cursor = client_manifest;

        index = 0;
        while (cursor != NULL)
        {
            existsIn = 0;

            while (inner_cursor != NULL)
            {
                if (strcmp(inner_cursor->file, cursor->file) == 0) // same name
                {
                    existsIn = 1;
                    break;
                }

                inner_cursor = inner_cursor->nextNode;
            }

            if (!existsIn)
            {
                notInClientManifest[index] = cursor;
            }

            index += 1;
            inner_cursor = client_manifest;
            cursor = cursor->nextNode;
        }

        // ====================================================================================================

        if (strcmp(server_manifest->repoVersion, client_manifest->repoVersion) == 0) // same version
        {
            /* File not in server .manifest */
            if (notInServerManifest[0] != NULL)
            {
                int i = 0;
                while (notInServerManifest[i] != NULL)
                {
                    printf("U %s\n", notInServerManifest[i]->file);
                    i += 1;
                }
            }

            if (hashIsDifferent[0] != NULL)
            {
                int i = 0;
                while (hashIsDifferent[i] != NULL)
                {
                    printf("U %s\n", hashIsDifferent[i]->file);
                    i += 1;
                }
            }
        }
        else // different version
        {
            int path_length = 9 + strlen(repo) + 9;
            char path[path_length];
            memset(path, '\0', path_length);

            strcpy(path, "Projects/");
            strcat(path, repo);
            strcat(path, "/.Update");

            int wd = open(path, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
            // Case 2 (modify):
            // 	File in client .manifest
            // 	File in server .manifest
            // 	.manifests are different versions
            // 	Files hash in client is same in client .manifest

            if (notInClientManifest[0] != NULL) // Case 3 (added)
            {
                int i = 0;
                while (notInClientManifest[i] != NULL)
                {
                    printf("A %s\n", notInClientManifest[i]->file);
                    i += 1;
                }
            }

            if (notInServerManifest[0] != NULL) // Case 4 (deleted)
            {
                int i = 0;
                while (notInServerManifest[i] != NULL)
                {
                    printf("D %s\n", notInServerManifest[i]->file);
                }
            }
        }
    }
}
