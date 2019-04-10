CFLAGS= -g

all: compile clean run

compile: TCPServer.c SocketLibrary.o TCPServer.h
	gcc $(CFLAGS) -o WTF TCPServer.c SocketLibrary.o

SocketLibrary.o: SocketLibrary.c SocketLibrary.h
	gcc $(CFLAGS) -c SocketLibrary.c

clean:
	-rm -f *.o
	-rm -f *.h.gch

run:
	./WTF