#include "WTFCommands.h"

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
            fprintf(stderr, "%sWarning%s: .wtf directory already exists.\n", YELLOW, RESET);
        }
        else
        {
            fprintf(stderr, "%sError%s: mkdir() has failed to create .wtf folder.\n", RED, RESET);
        }
    }
    else
    {
        printf(".wtf directory has been created.\n");
    }

    if (mkdir(".wtf/objects", (S_IRWXU | S_IRWXG | S_IRWXO)) == -1) // grant all rights to everyone (mode 0777 = rwxrwxrwx).
    {
        if (errno == EEXIST)
        {
            fprintf(stderr, "%sWarning%s: objects directory already exists.\n", YELLOW, RESET);
        }
        else
        {
            fprintf(stderr, "%sError%s: mkdir() has failed to create objects folder.\n", RED, RESET);
        }

        return;
    }
    else
    {
        printf("objects directory has been created.\n");
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
        fprintf(stderr, "%sError%s: there was an error writing to config.\n", RED, RESET);
    }
    else
    {
        printf("%s%s%s, has been %ssuccessfully%s added to the config file.\n", GREEN, string, RESET, GREEN, RESET);
    }

    close(fd); // close file to free up resources.
    return;
}

// the only way to return a pointer to a new object that didn't exist before the function was called is to use malloc.
struct server_info *getServerConfig()
{
    struct server_info *temporary = malloc(sizeof(struct server_info));

    int fd = open(".wtf/config.txt", O_RDONLY); // get fd for config file.
    if (fd == -1)
    {
        fprintf(stderr, "Error: open has failed to retrieve the config file.\n");
        return NULL;
    }

    int fileLength = lseek(fd, 0, SEEK_END); // find files length with lseek()
    if (fileLength == -1)
    {
        fprintf(stderr, "Error: lseek failed to find end of file.\n");
        return NULL;
    }

    lseek(fd, 0, SEEK_SET); // reset file offset

    char buffer[fileLength];          // create array (buffer) to hold file.
    memset(buffer, '\0', fileLength); // remove junk memory.
    read(fd, buffer, fileLength);     // place string into buffer.

    temporary->IP = strtok(buffer, ":"); // start off tokenizer, get IP.
    temporary->port = strtok(NULL, " "); // get port.

    close(fd); // close file.

    return temporary; // return malloc'd struct that contains address + port.
}