#include "WTFCommands.h" // core functions
#include "Client.h"      // client functions

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
        exit(EXIT_FAILURE);
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

/**
 *  sendArgument()
 *  @params: char *, the string version of argument.
 *  @returns: void.
 *  @comments: attempts a connection to server, sends argument to server,
 * tries every 3 seconds, on user to cancel.
 **/
void sendArgument(char *argument)
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
    printf("\nWaiting for server response... ");
    recv(server.socket_fd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    
    close(server.socket_fd);
    
    printf("Disconnected from server.\n\n");
    
    free(serverInfo->IP); // free IP malloc.
    free(serverInfo);     // free struct afterwards.
    return;
}

//                      CLIENT MANIFEST FUNCTIONS DOWN BELOW
//==================================================================================

void manageManifest(char* projectName){
    struct project_manifest *newManifest = NULL;
    struct project_manifest *oldManifest = NULL;
    struct project_manifest *updatedManifest = NULL;
    char *projectPath = (char*)malloc((strlen(projectName) + 13) * sizeof(char));
    if(projectPath == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    char *dirPath = (char*)malloc((strlen(projectName) + 4) * sizeof(char));
    if(dirPath == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    
    strcpy(projectPath, "./");
    strcat(projectPath, projectName);
    strcat(projectPath, "/");
    strcpy(dirPath, projectPath);
    strcat(dirPath, "\0");
    strcat(projectPath,".manifest");
    strcat(projectPath, "\0");
    
    int fd = open(projectPath, O_RDONLY);
    
    
    if(fd != -1){ // A pre-existing .manifest file exists
        close(fd);
        oldManifest = fetchManifest(projectPath); // Store contents of old manifest file into the oldManifest struct
    }
    newManifest = buildManifest(dirPath); // Store the new manifest data into the newManifest struct -- By default, versions are set to zero (0)
    
    if(newManifest != NULL && oldManifest != NULL){ // If two manifest structs now exists, then merge them together and create a new struct
        updatedManifest = updateManifest(newManifest, oldManifest);
        outputManifestFile(updatedManifest, projectPath); // Output the merged manifest structs to a new manifest file
    }
    else if(newManifest != NULL && oldManifest == NULL){ // If there is no pre-existing manifest file
        outputManifestFile(newManifest, projectPath); // Output the contents of the manifest struct to a new manifest file
    }
    else{ // If anything else is the case
        fprintf(stderr, "Error: Project directory not found!\n");
        return;
    }
}

struct project_manifest *buildManifest(char *dirPath){
    struct project_manifest *head = NULL;
    struct project_manifest *current = NULL;
    
    char *path = dirPath;
    
    if(path == NULL)
        goto ERROR;
    DIR *dd = opendir(path); // Opens a directory and returns a dirctory pointer
    struct dirent *status = NULL; // Status pointer of type struct dirent
    
    if(dd != NULL){
        status = readdir(dd); // readdir() retuns pointer to next directory entry
        
        head = malloc(sizeof(struct project_manifest));
        if(head == NULL)
            goto ERROR;
        head -> repoVersion = (char*)malloc(2*sizeof(char));
        if(head -> repoVersion == NULL)
            goto ERROR;
        strcpy(head -> repoVersion, "0");
        head -> fileVersion = NULL;
        head -> file = NULL;
        head -> hash = NULL;
        head -> nextNode = NULL;
        head -> prevNode = NULL;
        
        while(status != NULL){ // If directory can be accessed, then keep looping
            int fd = 0;
            status = readdir(dd); // readdir() retuns pointer to next directory entry
            int inputLength = 0;
            char* filePath = NULL;
            int fileLength = 0;
            
            if(status == NULL) // If the direcory cannot be accessed, then exit
                break;
            
            if(status -> d_type != DT_DIR && strcmp(".manifest", status -> d_name) != 0){
                inputLength = (int)strlen(path);
                filePath = (char*)malloc((strlen(status -> d_name) + inputLength + 1) * sizeof(char));
                if(filePath == NULL)
                    goto ERROR;
                strcpy(filePath, path);
                strcat(filePath, status -> d_name);
                
                fd = open(filePath, O_RDONLY);
                
                if(fd == -1){
                    fprintf(stderr, "Error: File could not be opened!\n");
                    return NULL;
                }
                
                fileLength = (int)lseek(fd, 0, SEEK_END); // find files length with lseek()
                if (fileLength == -1)
                {
                    fprintf(stderr, "Error: lseek failed to find end of file.\n");
                    return NULL;
                }
                
                lseek(fd, 0, SEEK_SET); // reset file offset
                
                char buffer[fileLength + 1];          // create array (buffer) to hold file contents.
                memset(buffer, '\0', fileLength + 1); // remove junk memory.
                read(fd, buffer, fileLength);         // place string into buffer.
                buffer[fileLength + 1] = '\0';        // null-terminate the buffer
                
                const char *string = buffer;          // Point to the contents of the buffer with a const char *
                unsigned char *hash = SHA256(string, strlen(string), 0); // Get the hash of the string (in hexadecimal)
                char tempString[33];                  // String that holds converted hash
                char *hashBuff = malloc(1 * sizeof(char));
                if(hashBuff == NULL)
                    goto ERROR;
                
                int i; // Iterate through the hash
                for(i = 0; i < SHA256_DIGEST_LENGTH; ++i){
                    sprintf(hashBuff, "%02x", hash[i]); // Convert each hash character to hexidecimal
                    tempString[i] = *hashBuff; // Populate the tempString array with converted hash characters
                }
                tempString[i] = '\0'; // End the tempString with a delimiter
                char* hashString = (char*)malloc(33 * sizeof(char));
                if(hashString == NULL)
                    goto ERROR;
                strcpy(hashString, tempString); // Copy the converted hash into a char*
                
                if(head != NULL){
                    current = head;
                    while(current -> nextNode != NULL){
                        current = current -> nextNode;
                    }
                    current -> nextNode = malloc(sizeof(struct project_manifest));
                    current -> nextNode -> repoVersion = NULL;
                    current -> nextNode -> fileVersion = (char*)malloc(2*sizeof(char));
                    if(current -> nextNode -> fileVersion == NULL)
                        goto ERROR;
                    strcpy(current -> nextNode -> fileVersion, "0");
                    current -> nextNode -> file = (char*)malloc(strlen(filePath) * sizeof(char));
                    if(current -> nextNode -> file == NULL)
                        goto ERROR;
                    memcpy(current -> nextNode -> file, filePath, strlen(filePath));
                    current -> nextNode -> hash = (char*)malloc(strlen(hash) * sizeof(char));
                    if(current -> nextNode -> hash == NULL)
                        goto ERROR;
                    memcpy(current -> nextNode -> hash, hashString, strlen(hashString));
                    current -> nextNode -> nextNode = NULL;
                    current -> nextNode -> prevNode = current;
                }
                
            }
        }
    }
    else{
        fprintf(stderr, "Error: Directory could not be opened!\n");
        return NULL;
    }
    
    goto DONE;
    
ERROR:
    fprintf(stderr, "Error: Malloc() returned NULL!\n");
    return NULL;
DONE:
    return head;
}

struct project_manifest * fetchManifest(char *path){ // Fetch a manifest if one already exists
    struct project_manifest *head = NULL;
    struct project_manifest *cursor = NULL;
    int rd = open(path, O_RDONLY);
    int status = 0;
    char buffer = ' '; // Holds each character found in file
    char string[2000][2000]; // Holds strings of tokens found
    int i = 0; // String index
    int j = 0; // Character index of string
    int cont = 1; // Value to decide whether or not to loop through list to start new node
    _Bool startPath = 0; // Used to indicate when to start taking file path token
    _Bool startHash = 0; // Used to indicate when to start taking hash token
    
    if(rd != -1){
        do{
            status = (int)read(rd, &buffer, 1);
            
            if(status > 0){
                // IF HEAD DOES NOT EXIST YET
                if(head == NULL){
                    while(status > 0){
                        if(buffer != '\n' && isdigit(buffer)){
                            string[i][j] = buffer;
                            ++j;
                        }
                        else if(buffer == '\n'){
                            string[i][j] = '\0';
                            head = malloc(sizeof(struct project_manifest));
                            if(head == NULL)
                                goto ERROR;
                            head -> repoVersion = (char*)malloc(strlen(string[i]) * sizeof(char));
                            strcpy(head -> repoVersion, string[i]);
                            ++i;
                            j = 0;
                            break;
                        }
                        status = (int)read(rd, &buffer, 1); // Get Character
                    }
                    head -> nextNode = NULL;
                    head -> prevNode = NULL;
                }
                // IF HEAD EXISTS
                else if(head != NULL){
                    if(cont == 1){ // Only loop once new token(s) have been added to the linked list
                        cursor = head;
                        while(cursor -> nextNode != NULL) // Loop until at the end of the list
                            cursor = cursor -> nextNode;
                        
                        cursor -> nextNode = malloc(sizeof(struct project_manifest));
                        if(cursor -> nextNode == NULL)
                            goto ERROR;
                        cursor -> nextNode -> repoVersion = NULL;
                        cont = 0;
                    }
                    while(buffer != '\n' && status > 0){
                        if(isdigit(buffer) && startPath == 0 && startHash == 0){
                            string[i][j] = buffer;
                            ++j;
                        }
                        else if(buffer == ' ' && startPath == 0 && startHash == 0){
                            startPath = 1;
                            string[i][j] = '\0';
                            cursor -> nextNode -> fileVersion = (char*)malloc(strlen(string[i]) * sizeof(char));
                            if(cursor -> nextNode -> fileVersion == NULL)
                                goto ERROR;
                            strcpy(cursor -> nextNode -> fileVersion, string[i]);
                            ++i;
                            j = 0;
                        }
                        else if(buffer != ' ' && startPath == 1 && startHash == 0){
                            string[i][j] = buffer;
                            ++j;
                        }
                        else if(buffer == ' ' && startPath == 1 && startHash == 0){
                            startPath = 0;
                            startHash = 1;
                            string[i][j] = '\0';
                            cursor -> nextNode -> file = (char*)malloc(strlen(string[i]) * sizeof(char));
                            if(cursor -> nextNode -> file == NULL)
                                goto ERROR;
                            strcpy(cursor -> nextNode -> file, string[i]);
                            ++i;
                            j = 0;
                        }
                        else if(startHash == 1 && startPath == 0){
                            string[i][j] = buffer;
                            ++j;
                        }
                        
                        status = (int)read(rd, &buffer, 1); // Get Character
                    }
                    if(buffer == '\n' && startHash == 1){
                        startHash = 0;
                        string[i][j] = '\0';
                        cursor -> nextNode -> hash = (char*)malloc(strlen(string[i]) * sizeof(char));
                        if(cursor -> nextNode -> hash == NULL)
                            goto ERROR;
                        memcpy(cursor -> nextNode -> hash, string[i], strlen(string[i]));
                        ++i;
                        j = 0;
                    }
                    cursor -> nextNode -> nextNode = NULL;
                    cursor -> nextNode -> prevNode = cursor;
                    cont = 1;
                }
            }
        }while(status > 0);
    }
    else{
        fprintf(stderr, "Error: Manifest file could not be opened\n");
        return NULL;
    }
    
    cursor -> nextNode = NULL; // Pinch off linked list
    
    goto DONE;
    
ERROR:
    close(rd);
    fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
    return NULL;
DONE:
    close(rd); // Close .manifest file
    return head;
}

struct project_manifest * updateManifest(struct project_manifest *newMan, struct project_manifest *oldMan){ // Merge two manifests together
    struct project_manifest *updated = NULL; // Will hold the merged manifests
    struct project_manifest *upCursor = NULL;
    struct project_manifest *newCursor = newMan;
    int iterator = 0; // Index for loop
    char *searchReturn; // Holds value of file version, returned by searchOldManifest()
    
    while(newCursor != NULL){
        if(updated == NULL){
            updated = malloc(sizeof(struct project_manifest));
            if(updated == NULL)
                goto ERROR;
            updated -> repoVersion = (char*)malloc(SHA256_DIGEST_LENGTH * sizeof(char));
            if(updated -> repoVersion == NULL)
                goto ERROR;
            strcpy(updated -> repoVersion, oldMan -> repoVersion);
            
            updated -> nextNode = NULL;
            updated -> prevNode = NULL;
            newCursor = newCursor -> nextNode;
        }
        else if(updated != NULL){
            upCursor = updated;
            while(upCursor -> nextNode != NULL){
                upCursor = upCursor -> nextNode;
            }
            
            searchReturn = searchOldManifest(newCursor -> file, oldMan); // Get version number of file from old manifest. If the file doesn't exist, then NULL is returned
            
            // MERGE THE OLD AND NEW MANIFEST STRUCTS INTO THE UPDATED MANIFEST STRUCT
            if(searchReturn != NULL && strcmp(searchReturn, "\0") != 0){
                upCursor -> nextNode = malloc(sizeof(struct project_manifest));
                upCursor -> nextNode -> fileVersion = (char*)malloc(strlen(searchReturn) * sizeof(char));
                if(upCursor -> nextNode -> fileVersion == NULL)
                    goto ERROR;
                strcpy(upCursor -> nextNode -> fileVersion, searchReturn);
                upCursor -> nextNode -> file = (char*)malloc((strlen(newCursor -> file)) * sizeof(char));
                if(upCursor -> nextNode -> file == NULL)
                    goto ERROR;
                strcpy(upCursor -> nextNode -> file, newCursor -> file);
                upCursor -> nextNode -> hash = (char*)malloc((strlen(newCursor -> hash)) * sizeof(char));
                if(upCursor -> nextNode -> hash == NULL)
                    goto ERROR;
                strcpy(upCursor -> nextNode -> hash, newCursor -> hash);
                upCursor -> nextNode -> nextNode = NULL;
                upCursor -> nextNode -> prevNode = upCursor;
            }
            else if(searchReturn == NULL){ // Since NULL is returned, then the file version defaults to 0
                upCursor -> nextNode = malloc(sizeof(struct project_manifest));
                upCursor -> nextNode -> fileVersion = (char*)malloc(2 * sizeof(char));
                if(upCursor -> nextNode -> fileVersion == NULL)
                    goto ERROR;
                strcpy(upCursor -> nextNode -> fileVersion, "0");
                upCursor -> nextNode -> file = (char*)malloc((strlen(newCursor -> file)) * sizeof(char));
                if(upCursor -> nextNode -> file == NULL)
                    goto ERROR;
                strcpy(upCursor -> nextNode -> file, newCursor -> file);
                upCursor -> nextNode -> hash = (char*)malloc((strlen(newCursor -> hash)) * sizeof(char));
                if(upCursor -> nextNode -> hash == NULL)
                    goto ERROR;
                strcpy(upCursor -> nextNode -> hash, newCursor -> hash);
                upCursor -> nextNode -> nextNode = NULL;
                upCursor -> nextNode -> prevNode = upCursor;
            }
            
            ++iterator;
            newCursor = newCursor -> nextNode;
        }
    }
    
    goto DONE;
    
ERROR:
    fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
    return NULL;
DONE:
    // Following print statements are for debugging
    /*printManifest(oldMan);
     printf("\n");
     printManifest(newMan);
     printf("\n");
     printManifest(updated);*/
    
    return updated;
}

char *searchOldManifest(char *file, struct project_manifest *oldMan){ // Search contents of pre-existing manifest file
    struct project_manifest *cursor = oldMan;
    
    if(cursor != NULL){
        cursor = cursor -> nextNode;
        
        if(cursor != NULL){
            if(strcmp(cursor -> file, file) == 0){
                return cursor -> fileVersion;
            }
            while(cursor -> nextNode != NULL){
                cursor = cursor -> nextNode;
                if(strcmp(cursor -> file, file) == 0){
                    return cursor -> fileVersion;
                }
            }
        }
    }
    
    return NULL; // Returns NULL if the file being searched does not exist in the old manifest
}

void outputManifestFile(struct project_manifest *manifest, char* path){ // Creates a final .manifest file
    struct project_manifest *cursor = manifest;
    int wd = open(path, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    char* string[SHA256_DIGEST_LENGTH];
    
    if(wd != -1){
        while(cursor != NULL){
            if(cursor -> repoVersion != NULL){
                write(wd, cursor -> repoVersion, strlen(cursor -> repoVersion));
                write(wd, "\n", 1);
            }
            else{
                write(wd, cursor -> fileVersion, strlen(cursor -> fileVersion));
                write(wd, " ", 1);
                write(wd, cursor -> file, strlen(cursor -> file));
                write(wd, " ", 1);
                write(wd, cursor -> hash, strlen(cursor -> hash));
                write(wd, "\n", 1);
            }
            
            cursor = cursor -> nextNode;
        }
        write(wd, "\n", 1); // Ends the last line with a new line (leaves empty line at the end of the file which is helpful for tokenizing)
    }
    else{
        fprintf(stderr,"Error: File could not be opened!\n");
    }
    
    close(wd);
}

// For debugging -- Outputs contents of manifest structs
void printManifest(struct project_manifest *head){
    struct project_manifest *cursor = head;
    
    if(cursor != NULL && cursor -> repoVersion != NULL){
        printf("Repo Version: %s\n", cursor -> repoVersion);
        while(cursor -> nextNode != NULL){
            cursor = cursor -> nextNode;
            
            printf("Version: %s\n", cursor -> fileVersion);
            printf("File: %s\n", cursor -> file);
            printf("Hash: ");
            int i;
            for(i = 0; i < SHA256_DIGEST_LENGTH; ++i)
                printf("%02x", cursor -> hash[i]);
            printf("\n");
        }
    }
}

//                      CLIENT MANIFEST FUNCTIONS UP ABOVE
//==================================================================================
