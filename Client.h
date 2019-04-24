#ifndef _CLIENT_h
#define _CLIENT_h

#include <stdio.h>
#include <openssl/sha.h>
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

struct project_manifest
{
    char *repoVersion;
    char *fileVersion;
    char *file;
    char *hash;
    struct project_manifest *nextNode;
    struct project_manifest *prevNode;
};

// Forward Declarations

void sendArgument(char *);
void handleArguments(int, char **);

// Client Manifest Functions
void manageManifest(char*); // Coordinates and manages the manifest
struct project_manifest * buildManifest(char *); // Build a manifest with the current files
struct project_manifest * fetchManifest(char *); // Fetch a manifest if one already exists
struct project_manifest * updateManifest(struct project_manifest *, struct project_manifest *); // Merge two manifests together
void outputManifestFile(struct project_manifest *, char *); // Creates a final .manifest file
char *searchOldManifest(char *, struct project_manifest *); // Searches the old manifest for file matches

// For Debugging
void printManifest(struct project_manifest *);

#endif /* _CLIENT_h */
