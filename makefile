CFLAGS= -g

all: compile clean run

compile: TCPServer.c TCPServer.h
	gcc $(CFLAGS) -o WTF TCPServer.c

clean:
	-rm -f *.o
	-rm -f *.h.gch

run:
	./WTF