#ifndef _WTFCOMMANDS_h
#define _WTFCOMMANDS_h

#include <fcntl.h>    // open()
#include <errno.h>    // errno
#include <stdio.h>    // stderr
#include <stdlib.h>   // free()
#include <string.h>   // memset, strlen()
#include <unistd.h>   // close(), write()
#include <sys/stat.h> // mkdir()
#include <openssl/sha.h>
#include <unistd.h>        // For close()
#include <errno.h>         // errno
#include <dirent.h>        // dirent
#include "SocketLibrary.h" // socket functions

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"

struct server_info
{
    char *IP;
    long port;
};

struct project_manifest
{
    char *repoVersion;
    char *fileVersion;
    char *file;
    char *hash;
    struct project_manifest *nextNode;
    struct project_manifest *prevNode;
};


// Forward Declarations:

extern void makeDirectory();
extern void createConfig(char *, char *);
extern struct server_info *getServerConfig();

// Client Manifest Functions
void manageManifest(char*); // Coordinates and manages the manifest
struct project_manifest * buildManifest(char *); // Build a manifest with the current files
struct project_manifest * fetchManifest(char *); // Fetch a manifest if one already exists
struct project_manifest * updateManifest(struct project_manifest *, struct project_manifest *); // Merge two manifests together
void outputManifestFile(struct project_manifest *, char *); // Creates a final .manifest file
char *searchOldManifest(char *, struct project_manifest *); // Searches the old manifest for file matches
void freeManList(struct project_manifest *); // Frees manifest linked list
void printManifest(struct project_manifest *); // For Debugging

// Commands
void checkStatus(char *);        // Checks to see if the
void createMutex(char *);        // Creates a project mutex
void removeMutex(char *);        // Removes a file mutex
void create(char *);            // Creates repository for both server and client

#endif /* _WTFCOMMANDS_h */
