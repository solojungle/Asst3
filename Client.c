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
        strcpy(string, "2"); // Convert name to number (easier on server end)
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
        
        return; // configure is client sided.
        // strcpy(string, "13"); // Convert name to number (easier on server end).
    }
    else
    {
        fprintf(stderr, "Command not found\n");
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
        update(repo, server.socket_fd);
    }
    else if (strcmp(command, "3") == 0)
    { // Upgrade
        upgrade(repo, server.socket_fd);
    }
    else if (strcmp(command, "4") == 0)
    { // Commit
        commit(argv[2], server.socket_fd);
    }
    else if (strcmp(command, "5") == 0)
    { // Push
        push(argv[2], server.socket_fd);
    }
    else if (strcmp(command, "6") == 0)
    { // Create
        _Bool clientOK = 0;
        DIR *cr = opendir("./Projects"); // Open directory for client projects
        
        char *clientPath = (char *)malloc((strlen(repo) + 12) * sizeof(char)); // Create path on client side for new repo
        if (clientPath == NULL)
        {
            fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
            exit(EXIT_FAILURE);
        }
        
        strcpy(clientPath, "./Projects/"); // Setup path for client
        strcat(clientPath, repo);
        strcat(clientPath, "\0");
        
        if (cr == NULL)
        { // Check to see if the directory Projects exists
            if (mkdir("Projects", S_IRWXU | S_IRWXG | S_IRWXO) == -1)
            { // grant all rights to everyone (mode 0777 = rwxrwxrwx).
                fprintf(stderr, "%sError%s: Projects folder could not be created.\n", RED, RESET);
                send(server.socket_fd, "E", 1, 0); // Send error to server
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("Projects folder has been created.\n");
            }
        }
        else
        {
            closedir(cr);
        }
        
        if (mkdir(clientPath, S_IRWXU | S_IRWXG | S_IRWXO) == -1) // grant all rights to everyone (mode 0777 = rwxrwxrwx).
        {
            fprintf(stderr, "%sError%s: %s folder already exists locally.\n", RED, RESET, repo);
            send(server.socket_fd, "E", 1, 0);
        }
        else
        {
            printf("%s folder has been successfully created locally.\n", repo);
            clientOK = 1;
            send(server.socket_fd, "O", 1, 0); // Send OK to server
        }
        
        recv(server.socket_fd, commandResponse, sizeof(commandResponse), 0);
        printf("%s%s%s", YELLOW, commandResponse, RESET);
        
        if (clientOK == 1 && commandResponse[0] == 'P')
        {
            outputFiles(receiveFiles(server.socket_fd), repo, 1); // 1 indicates that the client is receiving
        }
        else
        {
            recv(server.socket_fd, commandResponse, sizeof(commandResponse), 0);
            remove(clientPath);
        }
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
        removeFile(argv[2], argv[3]);
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
        recv(server.socket_fd, commandResponse, sizeof(commandResponse), 0);
        printf("%s%s%s", YELLOW, commandResponse, RESET);
        outputFiles(receiveFiles(server.socket_fd), repo, 1); // 1 indicates that the client is receiving
        
        char *historyPath = (char *)malloc((strlen(repo) + 21) * sizeof(char));
        strcpy(historyPath, "./Projects/");
        strcat(historyPath, repo);
        strcat(historyPath, "/.history\0");
        
        char *newPath = (char *)malloc((strlen(repo) + 23) * sizeof(char));
        strcpy(newPath, "./Projects/");
        strcat(newPath, repo);
        strcat(newPath, "_History.txt");
        
        int a = rename(historyPath, newPath);
        if (a == 0)
        {
            printf(".history file %ssuccessfully%s renamed and moved to %s\n", GREEN, RESET, newPath);
        }
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

void update(char *repo, int fd)
{
    if (!existsOnServerRecv(fd))
    {
        fprintf(stderr, "Error: Project does not exist on server.\n");
        return;
    }
    
    int length = 9 + strlen(repo) + 11; // Projects/ + <project_name> + /.manifest\0
    char client_manifest_path[length];
    memset(client_manifest_path, '\0', length);
    
    strcpy(client_manifest_path, "Projects/");
    strcat(client_manifest_path, repo);
    strcat(client_manifest_path, "/.manifest");
    
    struct project_manifest *client_manifest = fetchManifest(client_manifest_path);
    struct files_type *server_file = receiveFiles(fd);
    struct project_manifest *server_manifest;
    
    if (server_file == NULL || client_manifest == NULL)
    {
        fprintf(stderr, "%sError%s: Unable to grab the current version of .manifest.\n", RED, RESET);
        return;
    }
    
    // ====================================================================================================
    
    DIR *dir;                             // in case somehow the folder was left over.
    if ((dir = opendir(".temp")) != NULL) // check to see if folder was left over.
    {
        closedir(dir);
        remove(".temp/.manifest");
        rmdir(".temp");
    }
    
    // ====================================================================================================
    
    if (mkdir(".temp", S_IRWXU | S_IRWXG) == -1)
    {
        fprintf(stderr, "%sError%s: Directory failed to be created.\n", RED, RESET);
        return;
    }
    
    int temp_length = 6 + strlen(server_file->filename) + 1; // .temp/ + file_name + \0
    char temp_manifest_path[temp_length];
    memset(temp_manifest_path, '\0', temp_length);
    
    strcpy(temp_manifest_path, ".temp/"); // 6
    strcat(temp_manifest_path, server_file->filename);
    
    int wd;
    if ((wd = open(temp_manifest_path, O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
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
    
    if ((server_manifest = fetchManifest(".temp/.manifest")) == NULL) // fetch.
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
    
    struct project_manifest *outer_cursor = client_manifest;
    int numberOfFilesInClient = -1;
    
    while (outer_cursor != NULL)
    {
        numberOfFilesInClient += 1;
        outer_cursor = outer_cursor->nextNode;
    }
    
    outer_cursor = client_manifest->nextNode; // setup for loop
    
    // ====================================================================================================
    
    struct project_manifest *inner_cursor = server_manifest;
    int numberOfFilesInServer = -1;
    
    while (inner_cursor != NULL)
    {
        numberOfFilesInServer += 1;
        inner_cursor = inner_cursor->nextNode;
    }
    
    inner_cursor = server_manifest->nextNode; // setup for loop
    
    // ====================================================================================================
    
    struct project_manifest *fileMissingFromServer[numberOfFilesInClient];
    struct project_manifest *fileMissingFromClient[numberOfFilesInServer];
    struct project_manifest *fileInBothC[numberOfFilesInServer + numberOfFilesInClient];
    struct project_manifest *fileInBothS[numberOfFilesInServer + numberOfFilesInClient];
    
    int FMFS = 0; // File missing from server.
    int FMFC = 0; // File missing from client.
    int FIB = 0;  // File in both.
    
    int index = 0;
    int exists = 0;
    while (outer_cursor != NULL) // client files
    {
        while (inner_cursor != NULL) // server files
        {
            if (strcmp(inner_cursor->file, outer_cursor->file) == 0) // exists in both
            {
                exists = 1;
                break;
            }
            inner_cursor = inner_cursor->nextNode;
        }
        
        if (exists)
        {
            FIB += 1;
            fileInBothC[index] = outer_cursor;
            fileInBothS[index] = inner_cursor;
        }
        else
        {
            FMFS += 1;
            fileMissingFromServer[index] = outer_cursor;
        }
        
        exists = 0;                               // reset
        index += 1;                               // increment
        inner_cursor = server_manifest->nextNode; // reset.
        outer_cursor = outer_cursor->nextNode;    // increment.
    }
    
    index = 0;
    exists = 0;
    inner_cursor = client_manifest->nextNode;
    outer_cursor = server_manifest->nextNode;
    while (outer_cursor != NULL) // server files
    {
        while (inner_cursor != NULL) // client files
        {
            if (strcmp(inner_cursor->file, outer_cursor->file) == 0) // exists in both
            {
                exists = 1;
                break;
            }
            inner_cursor = inner_cursor->nextNode;
        }
        
        if (!exists)
        {
            FMFC += 1;
            fileMissingFromClient[index] = outer_cursor;
        }
        
        exists = 0;                               // reset
        index += 1;                               // increment
        inner_cursor = client_manifest->nextNode; // reset.
        outer_cursor = outer_cursor->nextNode;    // increment.
    }
    
    // ====================================================================================================
    
    struct project_manifest *uploadA[FMFS];
    struct project_manifest *uploadB[FIB];
    struct project_manifest *modify[FIB];
    struct project_manifest *added[FMFC];
    struct project_manifest *deleted[FMFS];
    struct project_manifest *conflict[FIB];
    
    int UA = 0; // upload A count.
    int UB = 0; // upload B count.
    int M = 0;  // modify count.
    int A = 0;  // added count.
    int D = 0;  // deleted count.
    int C = 0;  // conflict count.
    
    if (strcmp(client_manifest->repoVersion, server_manifest->repoVersion) == 0)
    {
        if (FIB != 0) // upload 1b: File in both server & client.
        {
            int i = 0;
            while (i < FIB)
            {
                char *hash = fileLiveHash(fileInBothC[i]->file);
                if (strcmp(fileInBothS[i]->hash, hash) != 0)
                {
                    uploadB[UB] = fileInBothC[i];
                    UB += 1;
                }
                free(hash);
                i += 1;
            }
        }
        if (FMFS != 0) // upload 1a: File not in server.
        {
            int i = 0;
            while (i < FMFS)
            {
                uploadA[UA] = fileMissingFromServer[i];
                UA += 1;
                i += 1;
            }
        }
    }
    else
    {
        if (FIB == 0) // modify
        {
            int i = 0;
            while (i < FIB)
            {
                if (strcmp(fileInBothC[i]->fileVersion, fileInBothS[i]->fileVersion) != 0) // If file versions are different.
                {
                    char *hash = fileLiveHash(fileInBothC[i]->file); // Grab live hash.
                    if (strcmp(fileInBothC[i]->hash, hash) == 0)     // If client manifest and live hashes are the same.
                    {
                        modify[M] = fileInBothS[i];
                        M += 1;
                    }
                    else
                    {
                        conflict[C] = fileInBothC[i];
                        C += 1;
                    }
                    free(hash);
                }
                i += 1;
            }
        }
        if (FMFS == 0) // deleted
        {
            int i = 0;
            while (i < FMFS)
            {
                deleted[D] = fileMissingFromServer[i];
                D += 1;
                i += 1;
            }
        }
        if (FMFC == 0) // added
        {
            int i = 0;
            while (i < FMFC)
            {
                added[A] = fileMissingFromClient[i];
                A += 1;
                i += 1;
            }
        }
    }
    
    // ====================================================================================================
    
    int path_length = 9 + strlen(repo) + 9;
    char path[path_length];
    memset(path, '\0', path_length);
    
    strcpy(path, "Projects/");
    strcat(path, repo);
    strcat(path, "/.Update");
    
    if ((wd = open(path, O_RDONLY)) != -1) // remove old .Update if one exists.
    {
        close(wd);
        remove(path);
    }
    
    if (C > 0) //  If conflicts are found, the client should not write a .Update.
    {
        int i = 0;
        while (i < C)
        {
            printf("Conflict: %s\n", conflict[i]->file);
        }
        return;
    }
    
    wd = open(path, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); // create .Update.
    
    if (UA > 0)
    {
        int i = 0;
        while (i < UA)
        {
            printf("U %s\n", uploadA[i]->file);
            write(wd, "U ", 2);
            write(wd, uploadA[i]->fileVersion, strlen(uploadA[i]->fileVersion));
            write(wd, " ", 1);
            write(wd, uploadA[i]->file, strlen(uploadA[i]->file));
            write(wd, " ", 1);
            write(wd, uploadA[i]->hash, strlen(uploadA[i]->hash));
            write(wd, "\n", 1);
            i += 1;
        }
    }
    
    if (UB > 0)
    {
        int i = 0;
        while (i < UB)
        {
            printf("U %s\n", uploadB[i]->file);
            write(wd, "U ", 2);
            write(wd, uploadB[i]->fileVersion, strlen(uploadB[i]->fileVersion));
            write(wd, " ", 1);
            write(wd, uploadB[i]->file, strlen(uploadB[i]->file));
            write(wd, " ", 1);
            write(wd, uploadB[i]->hash, strlen(uploadB[i]->hash));
            write(wd, "\n", 1);
            i += 1;
        }
    }
    
    if (M > 0)
    {
        int i = 0;
        while (i < M)
        {
            printf("M %s\n", modify[i]->file);
            write(wd, "M ", 2);
            write(wd, modify[i]->fileVersion, strlen(modify[i]->fileVersion));
            write(wd, " ", 1);
            write(wd, modify[i]->file, strlen(modify[i]->file));
            write(wd, " ", 1);
            write(wd, modify[i]->hash, strlen(modify[i]->hash));
            write(wd, "\n", 1);
            i += 1;
        }
    }
    
    if (A > 0)
    {
        int i = 0;
        while (i < A)
        {
            printf("A %s\n", added[i]->file);
            write(wd, "A ", 2);
            write(wd, added[i]->fileVersion, strlen(added[i]->fileVersion));
            write(wd, " ", 1);
            write(wd, added[i]->file, strlen(added[i]->file));
            write(wd, " ", 1);
            write(wd, added[i]->hash, strlen(added[i]->hash));
            write(wd, "\n", 1);
            i += 1;
        }
    }
    
    if (D > 0)
    {
        int i = 0;
        while (i < D)
        {
            printf("D %s\n", deleted[i]->file);
            write(wd, "D ", 2);
            write(wd, deleted[i]->fileVersion, strlen(deleted[i]->fileVersion));
            write(wd, " ", 1);
            write(wd, deleted[i]->file, strlen(deleted[i]->file));
            write(wd, " ", 1);
            write(wd, deleted[i]->hash, strlen(deleted[i]->hash));
            write(wd, "\n", 1);
            i += 1;
        }
    }
    write(wd, "\n", 1); // Ends the last line with a new line (leaves empty line at the end of the file which is helpful for tokenizing)
    close(wd);
    // updateHistory(path, repo, fd);
    return;
}

char *fileLiveHash(char *path)
{
    int fd;
    int file_length;
    char *hash_char;
    char *hash_string;
    
    if ((fd = open(path, O_RDONLY)) == -1)
    {
        fprintf(stderr, "Error: File could not be opened!\n");
        return NULL;
    }
    
    if ((file_length = lseek(fd, 0, SEEK_END)) == -1)
    {
        fprintf(stderr, "Error: lseek failed to find end of file.\n");
        return NULL;
    }
    
    lseek(fd, 0, SEEK_SET); // reset file offset
    
    if (file_length == 0)
    {
        fprintf(stderr, "Error: File is empty.\n");
        return NULL;
    }
    
    char file_buffer[file_length + 1];
    memset(file_buffer, '\0', file_length + 1); // remove garbage chars.
    
    if (read(fd, file_buffer, file_length) == -1)
    {
        fprintf(stderr, "Error: Failed to read file to hash.\n");
        return NULL;
    }
    
    const char *string = file_buffer;                        // Point to the contents of the buffer with a const char *
    unsigned char *hash = SHA256(string, strlen(string), 0); // Get the hash of the string (in hexadecimal)
    
    if ((hash_char = malloc(1)) == NULL)
    {
        fprintf(stderr, "Error: Malloc failed to allocate memory for hash string.\n");
        return NULL;
    }
    
    char temp_buffer[33];          // String that holds converted hash
    memset(temp_buffer, '\0', 33); // remove garbage chars.
    
    int i; // Iterate through the hash
    for (i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        sprintf(hash_char, "%02x", hash[i]);
        temp_buffer[i] = *hash_char;
    }
    
    free(hash_char); // free after using.
    
    if ((hash_string = (char *)malloc(33)) == NULL)
    {
        fprintf(stderr, "Error: Malloc failed to allocate memory for hash string.\n");
        return NULL;
    }
    
    strcpy(hash_string, temp_buffer); // Copy the converted hash into a char*
    
    return hash_string;
}

void upgrade(char *repo, int fd)
{
    if (!existsOnServerRecv(fd))
    {
        fprintf(stderr, "Error: Project does not exist on server.\n");
        return;
    }
    
    int project_path_length = 9 + strlen(repo) + 2; // Projects/ + <project_name> + / + \0
    char project_path[project_path_length];
    memset(project_path, '\0', project_path_length);
    
    strcpy(project_path, "Projects/");
    strcat(project_path, repo);
    strcat(project_path, "/");
    
    int manifest_path_length = project_path_length + 9;
    char manifest_path[manifest_path_length];
    memset(manifest_path, '\0', manifest_path_length);
    
    strcpy(manifest_path, project_path);
    strcat(manifest_path, ".manifest");
    
    struct project_manifest *client_manifest = fetchManifest(manifest_path); // local manifest
    
    if (client_manifest == NULL) // project name doesn't exist on client.
    {
        fprintf(stderr, "%sError%s: Unable to grab the client .manifest.\n", RED, RESET);
        return;
    }
    
    // ====================================================================================================
    
    int upgrade_length = project_path_length + 7;
    char upgrade_file_path[upgrade_length];
    memset(upgrade_file_path, '\0', upgrade_length);
    
    strcpy(upgrade_file_path, project_path);
    strcat(upgrade_file_path, ".Update");
    
    int update_fd;
    if ((update_fd = open(upgrade_file_path, O_RDONLY)) == -1)
    {
        fprintf(stderr, "%sPlease run './WTF update <project name>' before running upgrade.%s\n", YELLOW, RESET);
        return;
    }
    
    int update_contents_length;
    if ((update_contents_length = lseek(update_fd, 0, SEEK_END)) == -1)
    {
        fprintf(stderr, "Error: lseek failed to find end of file.\n");
        return;
    }
    
    lseek(update_fd, 0, SEEK_SET); // reset file offset
    
    char update_contents[update_contents_length + 1];
    memset(update_contents, '\0', update_contents_length + 1);
    read(update_fd, update_contents, update_contents_length);
    close(update_fd);
    
    // ====================================================================================================
    
    char *token = strtok(update_contents, " \n");
    if (token == NULL)
    {
        printf("%sAlready up to date.%s\n", GREEN, RESET);
        if (remove(upgrade_file_path) == -1)
        {
            fprintf(stderr, "Error: Failed to remove the .Update file.\n");
            return;
        }
        
        return;
    }
    
    // ====================================================================================================
    
    char *files[1];
    files[0] = upgrade_file_path;
    sendFiles(createFileList(files, 1), fd); // send .Update file to server.
    
    struct files_type *updated_files = receiveFiles(fd); // get files that need to be changed back.
    
    while (token != NULL)
    {
        if (strcmp(token, "D") == 0) // delete
        {
            token = strtok(NULL, " \n"); // file version
            token = strtok(NULL, " \n"); // filepath
            removeFile(repo, token);     // remove file (hopefully from .manifest)
            token = strtok(NULL, " \n"); // hash
        }
        else
        {
            token = strtok(NULL, " \n"); // file version
            token = strtok(NULL, " \n"); // filepath
            token = strtok(NULL, " \n"); // hash
        }
        
        token = strtok(NULL, " \n");
    }
    
    // ====================================================================================================
    
    struct files_type *cursor = updated_files;
    while (cursor != NULL)
    {
        char file_path[9 + strlen(repo) + cursor->filename_length + 2];
        memset(file_path, '\0', 9 + strlen(repo) + cursor->filename_length + 2);
        strcpy(file_path, "Projects/");
        strcat(file_path, repo);
        strcat(file_path, "/");
        strcat(file_path, cursor->filename);
        
        int wd;
        if ((wd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
        {
            fprintf(stderr, "Error: Upgrade open failed.\n");
        }
        else
        {
            write(wd, cursor->file, cursor->file_length);
        }
        
        close(wd);
        cursor = cursor->next;
    }
    
    // ====================================================================================================
    
    if (remove(upgrade_file_path) == -1)
    {
        fprintf(stderr, "Error: Failed to remove the .Update file.\n");
        return;
    }
    
    return;
}

void commit(char *repo, int fd)
{
    if (!existsOnServerRecv(fd)) //  if the project name doesn’t exist on the server
    {
        fprintf(stderr, "Error: Project does not exist on server.\n");
        return;
    }
    
    struct files_type *client_update;
    if ((client_update = grabClientUpdate(repo)) != NULL) // client has a .Update file that isn't empty (no .Update is fine).
    {
        char *token = strtok(client_update->file, " \n");
        if (token != NULL)
        {
            fprintf(stderr, "Error: .Update file is not empty, please run './WTF update <project name>'.\n");
            return;
        }
    }
    
    struct project_manifest *server_manifest;
    if ((server_manifest = grabServerManifest(repo, fd)) == NULL) // if cannot grab server .manifest.
    {
        fprintf(stderr, "%sError%s: Unable to fetch the server's .Manifest file.\n", RED, RESET);
        return;
    }
    
    struct project_manifest *client_manifest;
    if ((client_manifest = grabClientManifest(repo)) == NULL) // the client should run through its own .Manifest.
    {
        fprintf(stderr, "%sError%s: Unable to fetch the client's .Manifest file.\n", RED, RESET);
        return;
    }
    
    // the client should should first check to make sure that the .Manifest versions match.
    if (strcmp(client_manifest->repoVersion, server_manifest->repoVersion) != 0)
    {
        fprintf(stderr, "%sError%s: .Manifest versions do not match, please UPDATE local project.\n", RED, RESET);
        return;
    }
    
    // If the client's commit fails, it should delete its own .Commit
    int path_length = 9 + strlen(repo) + 8 + 1; // Projects/:9 + <repo> + /.Commit:8 +\0:1
    char path[path_length];
    memset(path, '\0', path_length);
    
    strcpy(path, "Projects/");
    strcat(path, repo);
    strcat(path, "/.Client");
    
    int wd;
    if ((wd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
    {
        fprintf(stderr, "Error: Creating .Commit has failed.\n");
        return;
    }
    
    // run through its own .Manifest and recompute a hashcode for each file listed in it.
    struct project_manifest *cursor = client_manifest->nextNode;
    while (cursor != NULL)
    {
        char *hash = fileLiveHash(cursor->file);
        if (strcmp(cursor->hash, hash) != 0)
        {
            char *end;
            long version = strtol(cursor->fileVersion, &end, 10);
            version += 1;
            
            char *new_file_version;
            intToStr(version, new_file_version, 10);
            write(wd, new_file_version, strlen(new_file_version));
            write(wd, " ", 1);
            write(wd, cursor->file, strlen(cursor->file));
            write(wd, " ", 1);
            write(wd, hash, strlen(hash));
            write(wd, "\n", 1);
        }
        free(hash);
        
        cursor = cursor->nextNode;
    }
    
    // if the only differences between the server's .Manifest and the client's are
    //  1. files that are in the server's .Manifest that are not in the client's
    //  2. files that are in the client's .Manifest that are not in the server's
    //  3. files that are in both .Manifests, but also have an entry in the client's newly-written .Commit with a higher version number
    
    return;
}

struct project_manifest *grabClientManifest(char *repo) // wrapper for fetchManifest.
{
    int path_length = 9 + strlen(repo) + 10 + 1; // Projects/:9 + <project_name> + /.manifest:10 + \0:1
    char client_manifest_path[path_length];
    memset(client_manifest_path, '\0', path_length);
    
    strcpy(client_manifest_path, "Projects/");
    strcat(client_manifest_path, repo);
    strcat(client_manifest_path, "/.manifest");
    
    struct project_manifest *client_manifest = fetchManifest(client_manifest_path); // is already malloc'd.
    
    if (client_manifest == NULL)
    {
        fprintf(stderr, "%sError%s: Unable to grab the current version of .manifest.\n", RED, RESET);
        return NULL;
    }
    
    return client_manifest;
}

struct files_type *grabClientUpdate(char *repo)
{
    int update_path_length = 9 + strlen(repo) + 8 + 1; // Projects/:9 + <project_name> + /.Update:8 + \0:1
    char update_path[update_path_length];
    memset(update_path, '\0', update_path_length);
    
    strcpy(update_path, "Projects/");
    strcat(update_path, repo);
    strcat(update_path, "/.Update");
    
    int fd;
    if ((fd = open(update_path, O_RDONLY)) == -1)
    {
        fprintf(stderr, "Warning: .Update file could not be found.\n");
        return NULL;
    }
    
    int file_length;
    if ((file_length = lseek(fd, 0, SEEK_END)) == -1)
    {
        fprintf(stderr, "Error: lseek failed to find end of file.\n");
        return NULL;
    }
    
    lseek(fd, 0, SEEK_SET); // reset file offset
    
    char file_buffer[file_length + 1];
    memset(file_buffer, '\0', file_length + 1); // remove garbage chars.
    
    if (read(fd, file_buffer, file_length) == -1)
    {
        fprintf(stderr, "Error: Failed to read .Update file.\n");
        return NULL;
    }
    
    close(fd);
    
    return initializeFileNode(update_path, strlen(update_path), file_buffer, file_length);
}

struct project_manifest *grabServerManifest(char *repo, int fd)
{
    DIR *dir;                                             // in case somehow the folder was left over.
    if ((dir = opendir(".temporary_conversion")) != NULL) // check to see if folder was left over.
    {
        closedir(dir);
        remove(".temporary_conversion/.manifest");
        rmdir(".temporary_conversion");
    }
    
    struct files_type *server_file = receiveFiles(fd);
    if (server_file == NULL) // If cannot grab server .manifest.
    {
        fprintf(stderr, "%sError%s: Unable to fetch the server's .manifest file.\n", RED, RESET);
        return NULL;
    }
    
    if (mkdir(".temporary_conversion", S_IRWXU | S_IRWXG) == -1)
    {
        fprintf(stderr, "%sError%s: Directory 'temporary_conversion' failed to be created.\n", RED, RESET);
        return NULL;
    }
    
    int path_length = 22 + strlen(server_file->filename) + 1; // .temporary_conversion/:22 + <manifest> + \0
    char path[path_length];
    memset(path, '\0', path_length);
    
    strcpy(path, ".temporary_conversion/");
    strcat(path, server_file->filename);
    
    int wd;
    if ((wd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
    {
        fprintf(stderr, "%sError%s: Could not open file.\n", RED, RESET);
        rmdir(".temporary_conversion");
        return NULL;
    }
    
    if (write(wd, server_file->file, server_file->file_length) == -1)
    {
        fprintf(stderr, "%sError%s: Could not write file.\n", RED, RESET);
        remove(path);
        rmdir(".temporary_conversion");
        return NULL;
    }
    
    close(wd);
    
    struct project_manifest *server_manifest;
    if ((server_manifest = fetchManifest(".temporary_conversion/.manifest")) == NULL) // fetch.
    {
        fprintf(stderr, "%sError%s: server_manifest is NULL\n", RED, RESET);
        remove(path);
        rmdir(".temporary_conversion");
        return NULL;
    }
    
    if (remove(path) != 0) // remove file.
    {
        fprintf(stderr, "%sError%s: path failed to be removed.\n", RED, RESET);
        return NULL;
    }
    
    if (rmdir(".temporary_conversion") == -1) // remove dir.
    {
        fprintf(stderr, "%sError%s: Directory failed to be deleted.\n", RED, RESET);
        return NULL;
    }
    
    return server_manifest;
}

void push(char *repo, int fd){
    int path_length = 9 + strlen(repo) + 8 + 1;
	char commit_path[path_length]; // Projects/:9 + <repo> + /.commit:8 + \0:1
    memset(commit_path, '\0', path_length);

	strcpy(commit_path, "Projects/");
	strcat(commit_path, repo);
	strcat(commit_path, "/.commit");

	/*if (!existsOnServerRecv(fd)) //  if the project name doesn't exist on the server
    {
        fprintf(stderr, "Error: Project does not exist on server.\n");
        return;
    }*/
    
    int rd;
    if((rd = open(commit_path, O_RDONLY)) == -1){
    	fprintf(stderr, "Error: .commit file does not exist! please run the commit function.\n");
    	return;
    }
    
    close(rd);

    send(fd, repo, strlen(repo), 0);
    
    char *files[1];
    files[0] = commit_path;
    sendFiles(createFileList(files, 1), fd);
}











