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
            fprintf(stderr, "Error: .wtf directory already exists\n");
        }
        else
        {
            fprintf(stderr, "Error: mkdir() has failed to create .wtf folder.\n");
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
            fprintf(stderr, "Error: objects directory already exists\n");
        }
        else
        {
            fprintf(stderr, "Error: mkdir() has failed to create objects folder.\n");
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

    char string[stringLength]; // initalize char array + null character.
    memset(string, '\0', stringLength); // remove junk values.

    // creating (address + ':' + port).
    strcpy(string, address);
    strcat(string, ":");
    strcat(string, port);

    if (write(fd, string, stringLength) != stringLength)
    {
        fprintf(stderr, "Error: there was an error writing to config.\n");
    }
    else
    {
        printf("%s, has been successfully added to the config file.\n", string);
    }

    close(fd); // close file to free up resources.
}