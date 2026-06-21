CC = gcc

all: server client

server:
	$(CC) server.c file.c udp.c tftp.c -o server

client:
	$(CC) client.c file.c udp.c tftp.c -o client

clean:
	rm -f server client