#ifndef _WTFCOMMANDS_h
#define _WTFCOMMANDS_h

#include <fcntl.h>    // open()
#include <errno.h>    // errno
#include <stdio.h>    // stderr
#include <stdlib.h>   // free()
#include <string.h>   // memset, strlen()
#include <unistd.h>   // close(), write()
#include <sys/stat.h> // mkdir()

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"

/**
 *  Forward Declarations
 **/
extern void makeDirectory();
extern void createConfig(char *, char *);

#endif /* _WTFCOMMANDS_h */