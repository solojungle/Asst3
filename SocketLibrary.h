#ifndef _SOCKETLIBRARY_h
#define _SOCKETLIBRARY_h

#include <stdio.h>      // stderr
#include <fcntl.h>      // O_RDONLY
#include <stdlib.h>     // exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <signal.h>     // signal()
#include <string.h>     // strcpy(), memset()
#include <unistd.h>     // read(), lseek()
#include <arpa/inet.h>  // inet_ntoa()
#include <netinet/in.h> // htonl(), htons(), sockaddr_in
#include <sys/socket.h> // socket(), bind(), AF_INET

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"

struct server_type
{
    int socket_fd;
};

struct files_type
{
    int file_length;
    int filename_length;
    char *filename;
    char *file;
    struct files_type *next;
};

extern struct files_type *decodeString(int);
extern long findDigit(int);

extern char *createEncodedString(struct files_type *);
extern char *intToStr(long int, char *, int);
extern int digits(int);
extern void initializeSocket(struct server_type *);
extern void bindSocket(struct server_type *, struct sockaddr_in);
extern void listenSocket(struct server_type *, struct sockaddr_in, int);
extern void handleServerClose(int);
extern void setSocketOptions(struct server_type *);
extern void getIPAddress(int, char *);
extern void createFilesFromStream(char *);
extern void sendFiles(struct files_type *, int);
extern void receiveFiles(int fd);
extern struct files_type *createFileList(char **, int);
extern struct files_type *append(struct files_type *, struct files_type *);
extern struct files_type *initializeFileNode(char *, int, char *, int);

#endif /* _SOCKETLIBRARY_h */

