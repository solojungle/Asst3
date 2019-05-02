#include "WTFCommands.h"
#include "SocketLibrary.h"


/**
 *  makeDirectory()
 *  @returns: void.
 *  @comments: creates a .wtf directory with objects subdirectory,
 * lack of 'return' calls in first mkdir() to allow subdirectories to be
 * created if they weren't before.
 **/
void makeDirectory()
{
    if (mkdir(".wtf", S_IRWXU | S_IRWXG | S_IRWXO) == -1) // grant all rights to everyone (mode 0777 = rwxrwxrwx).
    {
        if (errno == EEXIST)
        {
            fprintf(stderr, "%sWarning%s: .Wtf directory already exists.\n", YELLOW, RESET);
        }
        else
        {
            fprintf(stderr, "%sError%s: Mkdir() has failed to create .wtf folder.\n", RED, RESET);
        }
    }
    else
    {
        printf(".Wtf directory has been created.\n");
    }
    
    if (mkdir(".wtf/objects", (S_IRWXU | S_IRWXG | S_IRWXO)) == -1) // grant all rights to everyone (mode 0777 = rwxrwxrwx).
    {
        if (errno == EEXIST)
        {
            fprintf(stderr, "%sWarning%s: Objects directory already exists.\n", YELLOW, RESET);
        }
        else
        {
            fprintf(stderr, "%sError%s: Mkdir() has failed to create objects folder.\n", RED, RESET);
        }
        
        return;
    }
    else
    {
        printf("Objects directory has been created.\n");
    }
    
    return;
}

/**
 *  createConfig()
 *  @params:
 *      char *, IP Address of server to which socket will connect to.
 *      char *, port to which socket will use to connect to server.
 *  @returns: void.
 *  @comments: creates a config.txt file inside .wtf/ and writes address:port.
 **/
void createConfig(char *address, char *port)
{
    int fd; // file descriptor of config file to write to.
    
    /**
     *  will overwrite previous config when called, if previous exists.
     *  will create a new config file if non-exists.
     *  (S_IRWXU | S_IRWXG | S_IRWXO), sets access perms.
     **/
    fd = open(".wtf/config.txt", (O_RDWR | O_CREAT | O_TRUNC), (S_IRWXU | S_IRWXG | S_IRWXO));
    int stringLength = strlen(address) + strlen(port) + 1; // + 1 is for \0 char.
    
    char string[stringLength];          // initalize char array + null character.
    memset(string, '\0', stringLength); // remove junk values.
    
    // creating (address + ':' + port).
    strcpy(string, address);
    strcat(string, ":");
    strcat(string, port);
    
    if (write(fd, string, stringLength) != stringLength)
    {
        fprintf(stderr, "%sError%s: There was an error writing to config.\n", RED, RESET);
    }
    else
    {
        printf("%s%s%s, has been %ssuccessfully%s added to the config file.\n", GREEN, string, RESET, GREEN, RESET);
    }
    
    close(fd); // close file to free up resources.
    return;
}

/**
 *  getServerConfig()
 *  @returns: struct server_info *, holds server IP + Address copied from config.txt.
 *  @comments: retrieves the IP address + port from config file. Must free both the struct and IP char *.
 **/
struct server_info *getServerConfig()
{
    // the only way to return a pointer to a new object that didn't exist before the function was called is to use malloc.
    struct server_info *temporary = malloc(sizeof(struct server_info)); // malloc struct to retain info outside function scope.
    
    if (temporary == NULL)
    {
        fprintf(stderr, "%sError%s: Malloc failed to allocate memory for struct server_info.\n", RED, RESET);
        return NULL;
    }
    
    int fd = open(".wtf/config.txt", O_RDONLY); // get fd for config file.
    if (fd == -1)
    {
        fprintf(stderr, "%sError%s: Open has failed to retrieve the config file.\n", RED, RESET);
        return NULL;
    }
    
    int fileLength = lseek(fd, 0, SEEK_END); // find files length with lseek().
    if (fileLength == -1)
    {
        fprintf(stderr, "%sError%s: Lseek failed to find end of file.\n", RED, RESET);
        return NULL;
    }
    
    lseek(fd, 0, SEEK_SET); // reset file offset
    
    if (fileLength == 0) // config is empty
    {
        fprintf(stderr, "%sError%s: Config file is empty.\n", RED, RESET);
        return NULL;
    }
    
    char buffer[fileLength];              // create array (buffer) to hold file.
    memset(buffer, '\0', fileLength);     // remove junk memory.
    if (read(fd, buffer, fileLength) < 0) // place string into buffer.
    {
        fprintf(stderr, "%sError%s: Read has encountered an error.\n", RED, RESET);
        return NULL;
    }
    
    char *tempIP = strtok(buffer, ":"); // start off tokenizer, get IP.
    
    temporary->IP = malloc(strlen(tempIP)); // when creating pointers inside functions, you must malloc to keep outside of scope.
    
    if (temporary->IP == NULL)
    {
        fprintf(stderr, "%sError%s: Malloc failed to allocate memory for server_info->IP attribute.\n", RED, RESET);
        return NULL;
    }
    
    memset(temporary->IP, '\0', strlen(tempIP));           // remove junk chars.
    strcpy(temporary->IP, tempIP);                         // copy string to malloc.
    char *end;                                             // holds stores the address of the first invalid character.
    temporary->port = strtol(strtok(NULL, " "), &end, 10); // get port.
    
    close(fd); // close file.
    
    return temporary; // return malloc'd struct that contains address + port.
}

//                      CLIENT MANIFEST FUNCTIONS DOWN BELOW
//==================================================================================

void manageManifest(char* projectName){
    struct project_manifest *newManifest = NULL;
    struct project_manifest *oldManifest = NULL;
    struct project_manifest *updatedManifest = NULL;
    char *projectPath = (char*)malloc((strlen(projectName) + 60) * sizeof(char));
    if(projectPath == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    char *dirPath = (char*)malloc((strlen(projectName) + 4) * sizeof(char));
    if(dirPath == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    
    //  strcpy(projectPath, "./");
    strcpy(projectPath, projectName);
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
        free(projectPath);
        return;
    }
    
    free(projectPath);                 // Free dynamically allocated variable
    freeManList(newManifest);         // Free linked list
    freeManList(oldManifest);         // Free linked list
    freeManList(updatedManifest);     // Free linked list
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

void freeManList(struct project_manifest *head){
    struct project_manifest *cursor = head; // Set cursor to the head
    struct project_manifest *delete = malloc(sizeof(struct project_manifest)); // Used as a placeholder to free nodes in the linked list
    
    if(delete == NULL) // If malloc() failed, then throw an Error
        goto Error;
    
    if(cursor != NULL){ // If the token at the head of the list is not NULL, then free the list
        while(cursor -> nextNode != NULL){ // Traverse forward through the list until the end is reached
            cursor = cursor -> nextNode;
        }
        while(cursor != NULL){ // Once the last node is found, then traverse the list backwards and free each node
            delete = cursor; // Set delete to the cursor
            cursor = cursor -> prevNode; // Traverse the cursor backwards
            if(delete != NULL)
                free(delete); // Free delete (frees the node that delete points to)
        }
    }
    
    goto DONE; // FreeList() successfully completed, so go to main()
    
Error:
    fprintf(stderr, "--Error: Malloc() failed!--\n");
    
DONE: ;
}

//                      CLIENT MANIFEST FUNCTIONS UP ABOVE
//==================================================================================
//                             COMMANDS DOWN BELOW

void checkStatus(char *repo){
    _Bool wait = 0;                  // Variable used to tell whether or not to wait
    DIR *dd = opendir(repo);      // Open given repo
    struct dirent *status = NULL; // Status pointer of type struct dirent
    
    do{
        wait = 0; // Reset wait to 0
        
        if(dd != NULL){
            status = readdir(dd); // readdir() retuns pointer to next directory entry
            
            while(status != NULL){ // If directory can be accessed, then keep looping
                status = readdir(dd); // readdir() retuns pointer to next directory entry
                
                if(status == NULL) // If the direcory cannot be accessed, then exit
                    break;
                
                // DO SOMETHING HERE WITH FILES FOUND
                if(strcmp(".mutex", status -> d_name) == 0){
                    printf("File: %s\n", status -> d_name);
                    wait = 1; // Set wait to true
                    sleep(1); // Sleep for 1 second
                    closedir(dd); // Close repo to start again
                    DIR *dd = opendir(repo);  // Open given repo
                }
            }
        }
    }while(wait == 1);
    
    closedir(dd);
}

void createMutex(char *repo){
    char *path = (char*)malloc((strlen(repo) + 8) * sizeof(char));
    if(path == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    strcpy(path, repo);
    strcat(path, "/.mutex");
    strcat(path, "\0");
    
    int wd = open(path, O_CREAT | O_RDONLY, S_IRWXU);
    close(wd);
}

void removeMutex(char *repo){
    char *path = (char*)malloc((strlen(repo) + 8) * sizeof(char));
    if(path == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    strcpy(path, repo);
    strcat(path, "/.mutex");
    strcat(path, "\0");
    
    remove(path);
}

void removeFiles(char *path){
	DIR *dd = opendir(path); // Opens a directory and returns a dirctory pointer
    struct dirent *status = NULL; // Status pointer of type struct dirent

	if(dd != NULL){
		status = readdir(dd); // readdir() retuns pointer to next directory entry
		
		while(status != NULL){ // If directory can be accessed, then keep looping
            status = readdir(dd); // readdir() retuns pointer to next directory entry
            
            if(status == NULL) // If the direcory cannot be accessed, then exit
            	break;
                
            if(status -> d_type != DT_DIR){
            	int inputLength = (int)strlen(path);
                    char* filePath = (char*)malloc((strlen(status -> d_name) + inputLength + 2) * sizeof(char));
                    if(filePath == NULL){
                        fprintf(stderr,"Error: Malloc failed to allocate memory!\n");
                        return;
                    }
                    strcpy(filePath, path);
                    strcat(filePath, "/");
                    strcat(filePath, status -> d_name);
                    
                    remove(filePath);
            }
        }
	}
}

void create(char *repo, int fd){
    DIR *sr = opendir("./.server_repos"); // Open directory for server projects
    DIR *cr = opendir("./Projects"); // Open directory for client projects
    char **files; // Will hold the file path for the server manifest to send to client
    _Bool clientOK = 0;
    _Bool serverOK = 0;
    
    char *serverPath = (char*)malloc((strlen(repo) + 17) * sizeof(char)); // Create path on server side for new repo
    if(serverPath == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    char *clientPath = (char*)malloc((strlen(repo) + 12) * sizeof(char)); // Create path on client side for new repo
    if(clientPath == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    
    strcpy(serverPath, "./.server_repos/"); // Setup path for server
    strcat(serverPath, repo);
    strcat(serverPath, "\0");
    
    strcpy(clientPath, "./Projects/"); // Setup path for client
    strcat(clientPath, repo);
    strcat(clientPath, "\0");
    
    files = malloc((strlen(serverPath) + 11) * sizeof(char));
    if(files == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    files[0] = malloc((strlen(serverPath) + 11) * sizeof(char)); // Setup the path to the server manifest to send send to client
    strcpy(files[0], serverPath);
    strcat(files[0], "/.manifest\0");
    
    if(sr == NULL){ // Check to see if the directory .server_repos exists
        if (mkdir(".server_repos", S_IRWXU | S_IRWXG | S_IRWXO) == -1){ // grant all rights to everyone (mode 0777 = rwxrwxrwx).
            fprintf(stderr, "%sError%s: .server_repos folder could not be created..\n", RED, RESET);
            return;
        }
        else
            printf(".server_repos directory has been created.\n");
    }
    else
        closedir(sr);
    
    checkStatus("./.server_repos"); // If repo is occupied, will make function wait until it is free to use
    createMutex("./.server_repos"); // Lock down repository so no one can modify it
    
    if (mkdir(serverPath, S_IRWXU | S_IRWXG | S_IRWXO) == -1) // grant all rights to everyone (mode 0777 = rwxrwxrwx).
    {
        fprintf(stderr, "%sError%s: %s folder already exists.\n", RED, RESET, repo);
        send(fd, "Error: The project already exists!\n", 35, 0);
        send(fd, "Please use a different project name.\n", 37, 0);
        removeMutex("./.server_repos"); // Remove mutex
        return;
    }
    else
    {
        printf("%s folder has been created on server.\n", repo);
        manageManifest(serverPath); // Creates the deafult manifest for the new server repo
        serverOK = 1;
    }
    
    removeMutex("./.server_repos"); // Remove mutex
    
    if(cr == NULL){ // Check to see if the directory Projects exists
        if (mkdir("Projects", S_IRWXU | S_IRWXG | S_IRWXO) == -1){ // grant all rights to everyone (mode 0777 = rwxrwxrwx).
            fprintf(stderr, "%sError%s: Projects folder could not be created.\n", RED, RESET);
            send(fd, "Error: Projects folder could not be created.\n", 45, 0);
            return;
        }
        else{
            printf("Projects folder has been created.\n");
        }
    }
    else{
        closedir(cr);
    }
    
    if (mkdir(clientPath, S_IRWXU | S_IRWXG | S_IRWXO) == -1) // grant all rights to everyone (mode 0777 = rwxrwxrwx).
    {
        fprintf(stderr, "%sError%s: %s folder already exists on the client's side.\n", RED, RESET, repo);
        send(fd, "Warning: The project already exists locally! Please remove locally.\n", 68, 0);
        destroy(repo, fd);
        return;
    }
    else
    {
        printf("%s folder has been created on client.\n", repo);
        clientOK = 1;
        send(fd, "Repository has been created successfully within Projects folder.\n", 65, 0);
    }
    
    if(clientOK == 1 && serverOK == 1)
    	sendFiles(createFileList(files, 1), fd); // SENDING MANIFEST FILE FROM SERVER TO CLIENT THROUGH THE CLIENT'S FD
}


void destroy(char *repo, int fd){
    DIR *sr = opendir("./.server_repos"); // Open directory for server projects
    DIR *rsd; // Server repo-to-be-deleted
    char *deletedMessage = "Project destroyed successfuly! Please remove the project locally\n";
    char *deleteNotSuccessful = "Error: Project does not exist!\n";
    
    char *serverPath = (char*)malloc((strlen(repo) + 17) * sizeof(char)); // Create path on server side for new repo
    if(serverPath == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    
    strcpy(serverPath, "./.server_repos/"); // Setup path for server
    strcat(serverPath, repo);
    strcat(serverPath, "\0");
    
    if(sr == NULL){ // Check to see if the directory .server_repos exists
        fprintf(stderr, "Error: Server has no projects!\n");
    }
    else
        closedir(sr);
    
    checkStatus("./.server_repos"); // If repo is occupied, will make function wait until it is free to use
    createMutex("./.server_repos"); // Lock down repository so no one can modify it
    
    rsd = opendir(serverPath); // Open the directory-to-be-deleted to see if it exists
    
    if (rsd == NULL) // If the directory does not exist
    {
        fprintf(stderr, "%sError%s: %s project does not exist.\n", RED, RESET, repo);
        send(fd, deleteNotSuccessful, strlen(deleteNotSuccessful), 0);
        removeMutex("./.server_repos"); // Remove mutex
        return;
    }
    else // If the directory exists
    {
    	closedir(rsd); // Close the directory-to-be-deleted
    	removeFiles(serverPath); // Delete all files within the directory
    	remove(serverPath); // Remove the directory
        printf("%s project has been destroyed.\n", repo);
        send(fd, deletedMessage, strlen(deletedMessage), 0);
    }
    
    removeMutex("./.server_repos"); // Remove mutex
}

void checkout(char *repo, int fd){
	DIR *sr = opendir("./.server_repos"); // Open directory for server projects
    DIR *rch; // Server repo-to-be-checked-out
    char *checkoutNotSuccessful = "Error: Project does not exist!\n";
    char zipPath[300];
    memset(zipPath, '\0', sizeof(zipPath)); // memset the path to the tar.gz file
    char cmd[300]; // Holds the system command to tar the directory
    memset(cmd, '\0', sizeof(cmd)); // memset the command
    char **files; // Will hold the file path for the server project tar.gz to send to client
    
    char *serverPath = (char*)malloc((strlen(repo) + 17) * sizeof(char)); // Create path on server side for new repo
    if(serverPath == NULL){
        fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        return;
    }
    
    strcpy(serverPath, "./.server_repos/"); // Setup path for server
    strcat(serverPath, repo);
    strcat(serverPath, "\0");
    
    if(sr == NULL){ // Check to see if the directory .server_repos exists
        fprintf(stderr, "Error: Server has no projects!\n");
    }
    else
        closedir(sr);
    
    rch = opendir(serverPath); // Open the directory-to-be-checked-out to see if it exists
    
    if (rch == NULL) // If the directory does not exist
    {
        fprintf(stderr, "%sError%s: %s project does not exist.\n", RED, RESET, repo);
        send(fd, checkoutNotSuccessful, strlen(checkoutNotSuccessful), 0);
  
        return;
    }
    else // If the directory exists
    {
    	closedir(rch); // Close the directory-to-be-checked-out
    	printf("Checkpoint 1\n");
    	strcpy(cmd, "cd ./.server_repos && tar -czvf "); // Setup the command string to zip the directory
    	strcat(cmd, repo);
    	strcat(cmd, ".tar.gz ");
    	strcat(cmd, repo);
    	printf("CMD: %s\n", cmd);
    	printf("Checkpoint 2\n");
    	
    	system(cmd); // CD to directory and Zip project (which eliminates extra file structure)
    	printf("Checkpoint 3\n");
    	
    	strcpy(zipPath, "./.server_repos/");
    	strcat(zipPath, repo);
    	strcat(zipPath, ".tar.gz");
    	
    	files = malloc((strlen(serverPath) + 11) * sizeof(char));
    	if(files == NULL){
        	fprintf(stderr, "Error: Malloc failed to allocate memory!\n");
        	return;
   		}
   		files[0] = malloc(strlen(zipPath) * sizeof(char)); // Setup the path to the server manifest to send send to client
   		strcpy(files[0], zipPath);
    	
    	printf("Checkpoint 4\n");
    	
    	sendFiles(createFileList(files, 1), fd); // SENDING MANIFEST FILE FROM SERVER TO CLIENT THROUGH THE CLIENT'S FD
    	
    	// remove(zipPath);
    }
}

