Project Status 
==============

Status Update - Ali Awari - 4/9/19 @ 2:43PM
-----------------------------------------------
* Initial file upload.
* Server will create, bind, listen, and connect a client.

Status Update - Ali Awari - 4/9/19 @ 9:12PM
-----------------------------------------------
* Added core socket library.
* Fixed error where quiting program, would leave socket on TIME_WAIT.
* Started attempting to read client input.
* TODO see if we need handleServerClose(), and SIGINIT catch.
* TODO figure out what to do when connection fails and disconnect fails.

Status Update - Ali Awari - 4/10/19 @ 3:25PM
-----------------------------------------------
* Added better readability for console commands.
* Now shows clients ip address on join/leave.
* Temporary client code added.
* Makefile updated to compile client.

Status Update - Ali Awari - 4/10/19 @ 4:06PM
-----------------------------------------------
* Created more generic function names in Socket Lib.
* Added missing return statements to function.
* Cleaned client up a bit.

Status Update - Ali Awari - 4/10/19 @ 6:05PM
-----------------------------------------------
* Added argument framework to client.c.
* Added all arguments for client.c.
* Fixed error in client.c.

Status Update - Ali Awari - 4/11/19 @ 1:43PM
-----------------------------------------------
* Being handling arguments.
* Now send number to server as command for ease of conversion.
* Server now sends back confirmation message.

Status Update - Ali Awari - 4/11/19 @ 5:32PM
-----------------------------------------------
* Added comments to server/socket files.
* Removed redundant line from getIPAddress()

Status Update - Dennis Kager - 4/12/19 @ 4:14AM
------------------------------------------------
* Worked on Client.c to make input handling more consistent with the server's input handling
* Ignore some of the commented-out code in Client.c at the top, I will get rid of it soon!
* Added multi-threading capabilities in TCPServer.c, allowing it to handle many connections at once without blocking
* There is a bug where the port number displayed is not the actual port (9418) but can be connected through as 9418. I tried to fix it but had no luck.
* Updated function declarations so it wasn't bugging out the IDE
* Once we start adding functionality to our commands, I will add mutexes to avoid race conditions and undefined behavior from code being accessed between threads before any one of them can finish the command (i.e. One thread updating a file at the same time as another thread is trying to delete it)

Status Update - Dennis Kager - 4/12/19 @ 12:01AM
------------------------------------------------
* Added some global variables in TCPServer.c for handling SIGINT exit procedures
* Added a signal handler in TCPServer.c that will catch a SIGINT signal (CTRL + C) and properly close things down
* Added an outbound message to all clients connected to the server so when CTRL + C is pressed and all clients get disconnected, they will all recieve a message saying that the server has been disconnected
* Added handling in Client.c so that the client will try to connect to the server every three seconds until it can either connect or is interuppted with CTRL + C
* Modified makefile because now to run TCPServer, you have to supply it with a port number, such as ./WTF 9418

Status Update - Ali Awari - 4/13/19 @ 10:55AM
------------------------------------------------
* Removed handleServerClose()
* Formatting
* Editted printfs
* Changed string memset in client.c

Status Update - Ali Awari - 4/13/19 @ 4:43PM
------------------------------------------------
* Added WTFCommands, configure creates a .wtf directory, and config file.
* Edited makefile to reflect this addition
* General formatting
* Fixed bug where, server would display converted port number instead of orignal.

Status Update - Ali Awari - 4/13/19 @ 6:22PM
------------------------------------------------
* Configure will create a config.txt file inside .wtf/ directory.
* Added colors to warning messages.

Status Update - Ali Awari - 4/13/19 @ 11:32PM
------------------------------------------------
* getServerConfig() now retrieves config port + address, will be used in socketFunc().

Status Update - Ali Awari - 4/15/19 @ 1:58AM
------------------------------------------------
* server_info now has a long type instead of char * for port number.

Status Update - Ali Awari - 4/15/19 @ 2:01PM
------------------------------------------------
* Fixed bug in Client.c, serverInfo->IP wasn't malloc'd so it would lose it's value.
* In Client.c Instead of creating a new socket for each connection attempt, it will just create one, and check multiple times.
* Now message for which IP socket is connecting to is displayed.

Status Update - Ali Awari - 4/16/19 @ 10:59AM
------------------------------------------------
* Added comments to function in Client.c
* Configure will return in ifelse now instead of waiting til the end to return.
* Updated makefile.

Status Update - Ali Awari - 4/16/19 @ 5:03PM
------------------------------------------------
* Began work on sending files.
* Added -lpthread to makefile to remove pthread_create error.
* Editted for-loops to remove -stdc99 error message.
* Fixed iLab bug where args needed to be malloc'd.
* TODO: PLACE FREE() IN CORRECT PLACE FOR ARGS.

Status Update - Dennis Kager - 4/16/19 @ 7:55PM
------------------------------------------------
* Fixed a segfault caused by a loop which kills threads stepping out of bounds

Status Update - Ali Awari - 4/16/19 @ 9:23PM
------------------------------------------------
* Added function to index all files to be sent through socket.
* Created files_type struct to hold files.
* Fixed file_type bug where would put wrong values during creating node.

Status Update - Ali Awari - 4/17/19 @ 4:21PM
------------------------------------------------
* Fixed bug in createFileList(), will also now accept empty files as input.
* Added capitals to error messages.

Status Update - Ali Awari - 4/19/19 @ 3:24AM
------------------------------------------------
* Finished sending file functions.
* TODO: test/comment out functions. Think about free()

Status Update - Dennis Kager - 4/23/19 @ 8:45PM
------------------------------------------------
* Added manifest cabability in Client.h and added function definitions in Client.h
* Updated makefile to work with OpenSSL
* I still have to add some comments
* I have not added free() capability next, but I can do that next...
* TODO: Free() dynamic memory

Status Update - Dennis Kager - 4/25/19 @ 6:10PM
------------------------------------------------
* Moved the manifest functions from Client.c and Client.h to WTFCommands.c and WTFCommands.h to allow for the server to use those functions too
* Updated the makefile to reflect those changes
* Created a function to free the linked lists generated from the manifest functions
* Free()ed the args struct after being done with it in TCPServer.c

Status Update - Dennis Kager - 4/26/19 @ 2:20PM
------------------------------------------------
* Fixed a small error in SocketLibrary.c
* Added functionality to the create function
* WTFCommands.c and WTFCommands.h now has create() which will create a .server_repo folder to hold the server projects, if it does not exist already, then will proceed to add the project that the user requests
* Added createMutex() and removeMutex() as ways to prevent multiple clients from working on the same repository
* Added checkStatus() which will check to see if a repo is protected with a mutex before working on it. If there is a mutex, it will wait for a second and then look again until the mutex is gone (indicating that the repo is free to be worked on now). This prevents multiple clients from working on the same repository at the same time

Status Update - Ali Awari - 4/26/19 @ 2:05PM
------------------------------------------------
* Added functions which encoded files into a string.
* Free some malloc'd parts, will still need to error check everything.
* TODO: Error check, actually send files, free malloc.

Status Update - Ali Awari - 4/27/19 @ 5:15PM
------------------------------------------------
* Added comments to functions.
* Added error messages to functions.

Status Update - Ali Awari - 4/28/19 @ 5:26PM
------------------------------------------------
* Fixed error, in SocketLib.c where I malloc `strlen(string + 1)` instead of correctly `(strlen(string) + 1)`.
* Uncommented receivefiles().

Status Update - Dennis Kager - 4/28/19 @ 9:44PM
------------------------------------------------
* Modified create() to now include the local creation of repos.

Status Update - Ali Awari - 4/28/19 @ 10:27PM
------------------------------------------------
* Finished sending/receiving files.
* Changed 'OK.' and 'ERR.' buffers to reflect smaller messages.

Status Update - Ali Awari - 4/28/19 @ 1:38AM
------------------------------------------------
* Added comments to SocketLib.c.

Status Update - Dennis Kager - 4/30/19 @ 1:48AM
------------------------------------------------
* Sent .manifest file from server during create() but having a hard time figuring out how to recieve it in the client to add the file locally.
* Integrated manifest functions to be more universal with server and client.

Status Update - Ali Awari - 4/30/19 @ 1:49AM
------------------------------------------------
* Added second error message to SocketLib.c recvFiles().

Status Update - Dennis Kager - 4/30/19 @ 10:42PM
------------------------------------------------
* Slight overhaul on input handling in Client.c to separate what the client should expect (messages, files, etc.)
* Added many messages for the client to recieve from server for create() and destroy(), which brings me to....
* NEW COMMAND: Destroy() will wipe the contents of a directory on the server-side, then remove() the directory (hence the name, destroy)
* Added some more comments here and there
* Probably more I forgot to mention. Consider them "easter eggs" if you come across them before I can remember what else

Status Update - Dennis Kager - 5/01/19 @ 7:45PM
------------------------------------------------
* Added outputFiles() on socketLibrary.c, which now creates the files from the file struct
* Modifed some function definitions. For example, in recieveFiles(), I added a new parameter (called mode) to distinguish between the server and the client calling it

Status Update - Dennis Kager - 5/02/19 @ 12:02AM
------------------------------------------------
* Added checkout() to WTFCommands.c
* There is a bug somewhere with sending/receiving files. I have no problem grabbing the .manifest from a newly created project, however, when doing checkout I cannot open the project.tar.gz file that comes from the server. I do not know where the issue is with it. I intentionally left the project.tar.gz on the server when doing the checkout to compare the client and server copies. Both copies are the same size (in terms of bytes) and should theoretically have the same contents. The tar file I create on the server can be opened, but the client's (sent from server) cannot since it says it is corrupted. When doing checkout, there will be a message on the client-side saying it is decompressing, but it is not actually just yet. Just putting that out there to eliminate confusion.

Status Update - Ali Awari - 5/2/19 @ 2:31AM
------------------------------------------------
* Hard reset my repo.
* Formatting.
* Changed receiveFiles to return a LinkedList.
* Edited above change in Client.c.

Status Update - Ali Awari - 5/2/19 @ 2:31AM
------------------------------------------------
* Implemented currentversion.
* Fixed a bug in sendFiles where I didn't allocated a NULL term, for file contents.

Status Update - Dennis Kager - 5/04/19 @ 4:56AM
------------------------------------------------
* Finished checkout()
* Created three new functions in SocketLibrary.c for sending/receiving specifically tar.gz files

Status Update - Ali Awari - 5/4/19 @ 3:25PM
------------------------------------------------
* Fixed sendManifest()
* Added update()

Status Update - Ali Awari - 5/4/19 @ 9:42PM
------------------------------------------------
* Attempted to modify manageManifest().

Status Update - Dennis Kager - 5/04/19 @ 11:22PM
------------------------------------------------
* Fixed manageManifest()
* Removed extraneous code I left in SocketLibrary.c
* Added add() function. Note: I designed add() to only work when both a) the project repo exists and b) the file being added to the .manifest file is in the project repo

Status Update - Ali Awari - 5/5/19 @ 3:42AM
------------------------------------------------
* Update() will print out the differences in files. (Upload case a. was only one currently tested).
* TODO: write to .Update file.

Status Update - Dennis Kager - 5/05/19 @ 6:10PM
------------------------------------------------
* Renamed command for destroy to be removeProj()
* Added removeFile() for the remove command to take an entry out of the manifest
* Attempting to localize create() so that it is relative to the client side

Status Update - Ali Awari - 5/6/19 @ 12:30AM
------------------------------------------------
* Added fileLiveHash().
* Rewrote update().
* TODO: Test is all.