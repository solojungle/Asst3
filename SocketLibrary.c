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

/*
 *  initializeFileNode()
 *  @params:
 *      char *, file_name.
 *      int, length of file_name.
 *      char *, file contents
 *      int, file contents length.
 *  @returns: struct files_type *, new node.
 *  @comments: creates a new file node.
 */
struct files_type *initializeFileNode(char *filename, int nameLength, char *file, int fileLength)
{
    struct files_type *temporary = (struct files_type *)malloc(sizeof(struct files_type));

    temporary->file = malloc(strlen(file) + 1);
    temporary->filename = malloc(strlen(filename) + 1);

    memset(temporary->file, '\0', strlen(file) + 1);
    memset(temporary->filename, '\0', strlen(filename) + 1);

    if (temporary->file == NULL || temporary->filename == NULL)
    {
        fprintf(stderr, "%sError%s: Malloc failed to allocate memory.\n", RED, RESET);
        return NULL;
    }

    strcpy(temporary->file, file);
    strcpy(temporary->filename, filename);

    temporary->filename_length = nameLength;
    temporary->file_length = fileLength;

    temporary->next = NULL;

    return temporary;
}

/*
 *  append()
 *  @params:
 *      struct files_type *, new node to append to list.
 *      struct files_type *, head node of list.
 *  @returns: struct files_type *, new head of linked list.
 *  @comments: appends node to linked list.
 */
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

/*
 *  createFileList()
 *  @params:
 *      int, number of files.
 *      char **, file paths.
 *  @returns: struct files_type *, head of linked list.
 *  @comments: takes files pathnames and creates a linked list.
 */
struct files_type *createFileList(char **files, int n)
{
    struct files_type *root = NULL; // create starting point.

    if (files == NULL) // check to see if input is NULL.
    {
        fprintf(stderr, "%sError%s: Files is NULL.\n", RED, RESET);
        return root;
    }

    if (files[0] == NULL) // see if files is empty.
    {
        fprintf(stderr, "%sError%s: Files is empty.\n", RED, RESET);
        return root;
    }

    if (n <= 0)
    {
        fprintf(stderr, "%sError%s: Must provide a non-negative number of files.\n", RED, RESET);
        return root;
    }

    int index = 0;       // number of files + 1.
    int fd;              // general fd for files.
    int file_length;     // length of file.
    int filename_length; // file name length.

    while (index < n)
    {
        fd = open(files[index], O_RDONLY); // open file from array.

        if (fd == -1)
        {
            fprintf(stderr, "%sError%s: File at \"%s\" does not exist.\n", RED, RESET, files[index]);
        }
        else
        {
            char *filename = basename(files[index]); // get filename.
            filename_length = strlen(filename);      // get length of filename.

            file_length = lseek(fd, 0, SEEK_END); // find files length with lseek().
            if (file_length == -1)
            {
                fprintf(stderr, "%sError%s: Lseek failed to find end of file.\n", RED, RESET);
                close(fd); // close file.
                return root;
            }

            lseek(fd, 0, SEEK_SET); // reset file offset.

            if (file_length == 0)
            {
                fprintf(stderr, "%sWarning%s: File at \"%s\" is empty.\n", RED, RESET, files[index]);
            }

            char file_contents[file_length + 1];          // create array (buffer) to hold file.
            memset(file_contents, '\0', file_length + 1); // remove junk memory.
            read(fd, file_contents, file_length);         // place file into buffer.

            root = append(initializeFileNode(filename, filename_length, file_contents, file_length), root); // create list.
        }

        if (fd) // don't need to close unopened file.
        {
            close(fd); // close file.
        }

        index += 1;
    }

    return root;
}

/*
 *  receiveFiles()
 *  @params: int, file descriptor of connection.
 *  @returns: void.
 *  @comments: receives encoded string, decodes it, then creates files.
 */
void receiveFiles(int fd, char *repo, int mode)
{
    char command_buffer[6];          // get first word in string e.i "send".
    memset(command_buffer, '\0', 6); // remove junk.

    printf("Receiving files...");

    if (recv(fd, command_buffer, sizeof(command_buffer) - 1, 0) == -1) // read first word
    {
       // send(fd, "ERR.", 4, 0); // reply to sender saying we received command.
        printf("%sERR.%s\n", RED, RESET); // Error
        fprintf(stderr, "%sError%s: There was an error receiving message from socket.\n", RED, RESET);
        return;
    }

    //send(fd, "OK.", 3, 0); // reply to sender saying we received command. // Wouldn't actually send message to client.

    if (strlen(command_buffer) == 0) // when thread/socket closes it sends an empty string
    {
        //fprintf(stderr, "%sError%s: Returned string was empty, an error occured on server.\n", RED, RESET);
        printf("%sERR.%s\n", RED, RESET); // Error
        fprintf(stderr, "%sError%s: No files received.\n", RED, RESET);
        return;
    }
    else if (strcmp(command_buffer, "send:") == 0) // check to make sure correct string was given.
    {
    	printf("%sOK.%s\n", GREEN, RESET); // Encoded file recieved
        struct files_type *files = decodeString(fd); // decode string.
        outputFiles(files, repo, mode);
    }
    else
    {
    	printf("%sERR.%s\n", RED, RESET); // Error
        fprintf(stderr, "%sError%s: Encoded string was sent incorrectly.\n", RED, RESET);
        return;
    }

    return;
}

void outputFiles(struct files_type *files, char *repo, int mode){
	struct files_type *cursor = files;
	char path[200];
	int wd;
	
	if(mode == 1){ // Outputting relative to client
		while(cursor != NULL){
			memset(path, '\0', sizeof(path));
			strcpy(path, "./Projects/");
			strcat(path, repo);
			strcat(path, "/");
			strcat(path, cursor -> filename);
		
			wd = open(path, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			if(wd != -1){
				write(wd, cursor -> file, cursor -> file_length);
				printf("%s file %sadded%s to %s\n", cursor -> filename, GREEN, RESET, path);
			}
			else
				fprintf(stderr, "Error: Could not create file!\n");
		
		
			close(wd);
			cursor = cursor -> next;
		}	
	}
	
	if(mode == 2){ // Outputting relative to server
		while(cursor != NULL){
			memset(path, '\0', sizeof(path));
			strcpy(path, "./.server_repos/");
			strcat(path, repo);
			strcat(path, "/");
			strcat(path, cursor -> filename);
		
			wd = open(path, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			if(wd != -1){
				write(wd, cursor -> file, cursor -> file_length);
				printf("%s file %sadded%s to %s\n", cursor -> filename, GREEN, RESET, path);
			}
			else
				fprintf(stderr, "Error: Could not create file!\n");
		
		
			close(wd);
			cursor = cursor -> next;
		}	
	}
}

/*
 *  decodeString()
 *  @params: int, file descriptor of connection.
 *  @returns: struct files_type *, a list of files.
 *  @comments: receives encoded string, and decodes it.
 */
struct files_type *decodeString(int fd)
{
    if (fd == -1) // check for invalid file descriptor
    {
        fprintf(stderr, "%sError%s: File descriptor is invalid.\n", RED, RESET);
        return;
    }

    int numberOfFiles; // number of files
    if ((numberOfFiles = findDigit(fd)) == -1)
    {
        fprintf(stderr, "%sError%s: Couldn't find beginning number in encoded string.\n", RED, RESET);
        return;
    }

    struct files_type *root = NULL; // create starting point.
    while (numberOfFiles != 0)      // while there exists files
    {
        long filename_length = findDigit(fd); // filename length.

        char filename[filename_length + 1];          // holds filename.
        memset(filename, '\0', filename_length + 1); // remove junk.

        if (recv(fd, filename, filename_length, 0) == -1) // read entire filename.
        {
            fprintf(stderr, "%sError%s: There was an error receiving message from socket.\n", RED, RESET);
            return;
        }

        long file_length = findDigit(fd); // find file contents length.

        // wait until all filenames are recorded before getting file contents.
        root = append(initializeFileNode(filename, filename_length, "null", file_length), root);

        numberOfFiles -= 1; // decrement counter.
    }

    struct files_type *cursor = root;
    while (cursor != NULL) // add file contents to files.
    {
        char temp[cursor->file_length + 1];
        memset(temp, '\0', cursor->file_length + 1);

        if (recv(fd, temp, cursor->file_length, 0) == -1) // read entire file
        {
            fprintf(stderr, "%sError%s: There was an error receiving message from socket.\n", RED, RESET);
            return;
        }

        cursor->file = realloc(cursor->file, cursor->file_length + 1); // realloc to file content size.
        memset(cursor->file, '\0', cursor->file_length + 1);           // remove previous contents.
        strcpy(cursor->file, temp);                                    // place correct content.

        cursor = cursor->next;
    }

    return root;
}

/*
 *  findDigit()
 *  @params: int, file descriptor of connection.
 *  @returns: long, 
 *  @comments: finds digit in encoded string and returns it as a long.
 */
long findDigit(int fd)
{
    if (fd == -1)
    {
        fprintf(stderr, "%sError%s: File descriptor is invalid.\n", RED, RESET);
        return;
    }

    char *digit_buffer = malloc(1); // unknown number of digits.

    if (digit_buffer == NULL)
    {
        fprintf(stderr, "%sError%s: Malloc failed to allocate memory.\n", RED, RESET);
        return -1;
    }

    memset(digit_buffer, '\0', 1); // remove junk

    char c[2];          // current char + null.
    memset(c, '\0', 2); // remove junk.

    int i = 1;                  // string + '\0'.
    while (strcmp(c, ":") != 0) // while char is  not delim.
    {
        i += 1;

        if (recv(fd, c, 1, 0) == -1) // read string.
        {
            fprintf(stderr, "%sError%s: There was an error receiving message from socket.\n", RED, RESET);
            return;
        }

        digit_buffer = realloc(digit_buffer, i); // digit is now 1 larger.
        strcat(digit_buffer, c);                 // add digit to string.
    }

    char *end;                                    // used for strtol.
    long number = strtol(digit_buffer, &end, 10); // convert string to long.
    free(digit_buffer);                           // free malloc

    return number;
}

/*
 *  sendFiles()
 *  @params:
 *      struct files_type *, linked list of files.
 *      int, file descriptor to send files too.
 *  @returns: void.
 *  @comments: takes a linked list of files, and sends an encoded string to client.
 */
void sendFiles(struct files_type *files, int fd)
{
    if (fd == -1)
    {
        fprintf(stderr, "%sError%s: File descriptor is invalid.\n", RED, RESET);
        return;
    }

    char *string = createEncodedString(files);
    if (string == NULL)
    {
        fprintf(stderr, "%sError%s: Encoded string return NULL.\n", RED, RESET);
        return;
    }

    printf("Files have been sent.\n");
    send(fd, string, strlen(string), 0);
    free(string);

    char response_buff[5]; // buff for err. or ok.
    memset(response_buff, '\0', 5);

    printf("Waiting for sender response... ");
    if (recv(fd, response_buff, sizeof(response_buff), 0) == -1)
    {
        fprintf(stderr, "%sError%s: There was an error receiving message from socket.\n", RED, RESET);
        return;
    }
    printf("%s\n", response_buff);

    return;
}

/*
 *  createEncodedString()
 *  @params: struct files_type *, linked list of files.
 *  @returns: char *, returns encoded string.
 *  @comments: iterates thorugh linked list of files, creates two seperate strings simultaneously
 * and then combines them to form an encoded string.
 */
char *createEncodedString(struct files_type *files)
{
    if (files == NULL) // check to see if LL is empty.
    {
        fprintf(stderr, "%sError%s: Files is NULL.\n", RED, RESET);
        return NULL;
    }

    char *all_filenames = malloc(1); // hold all the filenames.
    char *all_files = malloc(1);     // hold all the actual files.

    if (all_filenames == NULL || all_files == NULL)
    {
        fprintf(stderr, "%sError%s: Malloc failed to allocate memory.\n", RED, RESET);
        return NULL;
    }

    memset(all_filenames, '\0', 1); // remove junk.
    memset(all_files, '\0', 1);     // remove junk.

    int file_count = 0; // holds number of files in linked list.

    struct files_type *cursor = files; // begin looping through list.
    while (cursor != NULL)
    {
        file_count += 1; // increment # of files.

        // allnames.txt + newname.txt + null.
        int new_allfiles_length = strlen(all_files) + cursor->file_length + 1; // find how long string needs to be to accommodate new file.

        char file_contents[new_allfiles_length];          // buffer that will hold file contents.
        memset(file_contents, '\0', new_allfiles_length); // remove junk.
        strcpy(file_contents, all_files);                 // copy old files to buffer.
        strcat(file_contents, cursor->file);              // concat new file to buffer.

        if (cursor->file_length <= 0) // give warning for empty files.
        {
            printf("%sWarning%s: An empty file was encoded: \"%s\".\n", RED, RESET, cursor->filename);
        }
        else
        {
            all_files = realloc(all_files, new_allfiles_length); // increase length of file string.
            strcpy(all_files, file_contents);                    // overwrite old string with new.
        }

        // number of digits of the filename length e.i. 777 -> 3 digits + : + filename + digits of file_length.
        int encoded_filename_length = digits(cursor->filename_length) + 1 + cursor->filename_length + digits(cursor->file_length) + 1 + 1; // (:, :, '\0')

        char encoded_filename[encoded_filename_length];          // create string buffer.
        memset(encoded_filename, '\0', encoded_filename_length); // remove junk.

        char number[65];                               // hold filename_length.
        intToStr(cursor->filename_length, number, 10); // convert char* to str.

        strcpy(encoded_filename, number);           // add number.
        strcat(encoded_filename, ":");              // add ':'.
        strcat(encoded_filename, cursor->filename); // add filename.

        intToStr(cursor->file_length, number, 10); // convert file_length.
        strcat(encoded_filename, number);          // add file_length.
        strcat(encoded_filename, ":");             // add ':'.

        int new_allfilenames_length = strlen(all_filenames) + strlen(encoded_filename) + 1; // length of new allfilesnames
        char filenames_buffer[new_allfilenames_length];                                     // buffer that will hold filenames.
        memset(filenames_buffer, '\0', new_allfilenames_length);                            // remove junk.

        strcpy(filenames_buffer, all_filenames);    // copy old filenames into buffer.
        strcat(filenames_buffer, encoded_filename); // add new encoded filename to old.

        all_filenames = realloc(all_filenames, new_allfilenames_length); // realloc all_filenames fit size.

        strcpy(all_filenames, filenames_buffer); // add filenames_buffer to all_filenames.

        struct files_type *next = cursor->next;

        free(cursor->file);     // free linkedlist.
        free(cursor->filename); // free linkedlist.
        free(cursor);           // free linkedlist.

        cursor = next; // iterate through list.
    }

    // now begin to combine 'all_filenames' + 'all_files', to create final string.
    char numberBuff[65];
    intToStr(file_count, numberBuff, 10);
    int encoded_length = 5 + digits(file_count) + 1 + strlen(all_filenames) + strlen(all_files) + 1;
    char *encoded = malloc(encoded_length);
    memset(encoded, '\0', encoded_length);

    if (encoded == NULL)
    {
        fprintf(stderr, "%sError%s: Malloc failed to allocate memory.\n", RED, RESET);
        return NULL;
    }

    strcpy(encoded, "send:");
    strcat(encoded, numberBuff);
    strcat(encoded, ":");
    strcat(encoded, all_filenames); // add first half of encoded string.
    strcat(encoded, all_files);     // add second half of encoded string.

    free(all_filenames); // free temp malloc.
    free(all_files);     // free temp malloc.

    return encoded;
}

/*
 *  digits()
 *  @params: int, value to be counted.
 *  @returns: int, number of digits in number.
 *  @comments: Takes any number in base 10, and counts the digits in the number.
 */
int digits(int n)
{
    int count = 0;
    while (n != 0)
    {
        n /= 10;
        ++count;
    }

    return count;
}

/*
 *  intToStr()
 *  @params:
 *      int, value to be converted.
 *      char *, destination for conversion.
 *      int, base of number i.e 10.
 *  @returns: char *, converted integer string.
 *  @comments: converts a number into a string.
 */
char *intToStr(long int val, char *dst, int radix)
{
    char buffer[65];
    char *p;
    long int new_val;
    unsigned long int uval = (unsigned long int)val;

    if (radix < 0) /* -10 */
    {
        if (val < 0)
        {
            *dst++ = '-';
            /* Avoid integer overflow in (-val) for LLONG_MIN (BUG#31799). */
            uval = (unsigned long int)0 - uval;
        }
    }

    p = &buffer[sizeof(buffer) - 1];
    *p = '\0';
    new_val = (long)(uval / 10);
    *--p = '0' + (char)(uval - (unsigned long)new_val * 10);
    val = new_val;

    while (val != 0)
    {
        new_val = val / 10;
        *--p = '0' + (char)(val - new_val * 10);
        val = new_val;
    }
    while ((*dst++ = *p++) != 0)
        ;
    return dst - 1;
}
