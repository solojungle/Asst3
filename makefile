CFLAGS= -g

all: compileServer compileClient clean run

# compiles server includes, socket_lib, and core_commands
compileServer: TCPServer.c TCPServer.h SocketLibrary.o WTFCommands.o
	gcc $(CFLAGS) -o WTF TCPServer.c SocketLibrary.o WTFCommands.o

# compiles client includes, socket_lib, and core_commands
compileClient: Client.c SocketLibrary.o WTFCommands.o
	gcc $(CFLAGS) -o client Client.c SocketLibrary.o WTFCommands.o

# compile socket_lib
SocketLibrary.o: SocketLibrary.c SocketLibrary.h
	gcc $(CFLAGS) -c SocketLibrary.c

# compile core_commands
WTFCommands.o: WTFCommands.c WTFCommands.h
	gcc $(CFLAGS) -c WTFCommands.c

clean:
	-rm -f *.o
	-rm -f *.h.gch

run:
	./WTF 9418