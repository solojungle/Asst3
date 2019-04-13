#ifndef _WTFCOMMANDS_h
#define _WTFCOMMANDS_h

#include <fcntl.h>    // open()
#include <errno.h>    // errno
#include <stdio.h>    // stderr
#include <unistd.h>   // close()
#include <sys/stat.h> // mkdir()

struct config_addr // used to hold ip:port from config file.
{
    char *ip_address;
    unsigned int port;
};

/**
 *  Forward Declarations
 **/
extern void makeDirectory();
extern void createConfig();

#endif /* _WTFCOMMANDS_h */