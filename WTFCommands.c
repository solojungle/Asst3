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
 *  createConfigure()
 *  @params:
 *  @returns:
 *  @comments:
 **/
void createConfig()
{
    int fd;

    /**
     *  will overwrite previous config when called, if previous exists.
     *  will create a new config file if non-exists.
     *  (S_IRWXU | S_IRWXG | S_IRWXO), sets access perms.
     **/
    fd = open(".wtf/config.txt", (O_RDWR | O_CREAT | O_TRUNC), (S_IRWXU | S_IRWXG | S_IRWXO));
    close(fd);
}