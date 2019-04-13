CFLAGS= -g

all: compileServer compileClient clean run

compileServer: TCPServer.c SocketLibrary.o TCPServer.h
	gcc $(CFLAGS) -o WTF TCPServer.c SocketLibrary.o

compileClient: Client.c SocketLibrary.o
	gcc $(CFLAGS) -o client Client.c SocketLibrary.o

SocketLibrary.o: SocketLibrary.c SocketLibrary.h
	gcc $(CFLAGS) -c SocketLibrary.c

clean:
	-rm -f *.o
	-rm -f *.h.gch

run:
	./WTF 9418