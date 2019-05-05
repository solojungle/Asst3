#ifndef _CLIENT_h
#define _CLIENT_h

#include <stdio.h>
#include <unistd.h>        // For close()
#include <fcntl.h>         // open()
#include <errno.h>         // errno
#include <stdio.h>         // stderr
#include <stdlib.h>        // free()
#include <string.h>        // memset, strlen()
#include <unistd.h>        // close(), write()
#include <sys/stat.h>      // mkdir()
#include <dirent.h>        // dirent
#include "SocketLibrary.h" // socket functions

// Forward Declarations

void sendArgument(char *, char *, char *);
void handleArguments(int, char **);
void update(char *, char *, char *, int);

#endif /* _CLIENT_h */
