#ifndef _CLIENT_h
#define _CLIENT_h

#include <unistd.h>        // For close()
#include "SocketLibrary.h" // socket functions

// Forward Declarations

void sendArgument(char *);
void handleArguments(int, char **);

#endif /* _CLIENT_h */