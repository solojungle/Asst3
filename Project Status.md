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
