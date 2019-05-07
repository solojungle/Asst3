CFLAGS= -g

all: compileServer compileClient clean

# compiles server includes, socket_lib, and core_commands
compileServer: TCPServer.c TCPServer.h SocketLibrary.o WTFCommands.o
	gcc $(CFLAGS) -o WTFserver TCPServer.c SocketLibrary.o WTFCommands.o -lpthread -lssl -lcrypto

# compiles client includes, socket_lib, and core_commands
compileClient: Client.c Client.h SocketLibrary.o WTFCommands.o
	gcc $(CFLAGS) -o WTF Client.c SocketLibrary.o WTFCommands.o -lssl -lcrypto

# compile socket_lib
SocketLibrary.o: SocketLibrary.c SocketLibrary.h
	gcc $(CFLAGS) -c SocketLibrary.c

# compile core_commands
WTFCommands.o: WTFCommands.c WTFCommands.h
	gcc $(CFLAGS) -c WTFCommands.c

clean:
	-rm -f *.o
	-rm -f *.h.gch
