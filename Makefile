CC=gcc
CFLAGS=-Wall -g -o

all: UDP_client UDP_server

UDP_client: UDP_client.c
	$(CC) -Wall $^ -o $@

UDP_server: UDP_server.c
	$(CC) -Wall $^ -o $@

clean:
	rm *.o UDP_client UDP_server
