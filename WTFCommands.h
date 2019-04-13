#ifndef _WTFCOMMANDS_h
#define _WTFCOMMANDS_h

#include <fcntl.h>    // open()
#include <errno.h>    // errno
#include <stdio.h>    // stderr
#include <stdlib.h>   // free()
#include <string.h>   // memset, strlen()
#include <unistd.h>   // close(), write()
#include <sys/stat.h> // mkdir()

/**
 *  Forward Declarations
 **/
extern void makeDirectory();
extern void createConfig();

#endif /* _WTFCOMMANDS_h */