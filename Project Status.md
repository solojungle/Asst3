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
* Added argument framework to Client.c
* Fixed error in client.c