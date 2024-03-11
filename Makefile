# makefile for thread pool
#

CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

all: client.o threadpool.o queue.o
	$(CC) $(CFLAGS) -o example client.o threadpool.o queue.o $(PTHREADS)

client.o: client.c
	$(CC) $(CFLAGS) -c client.c $(PTHREADS)

threadpool.o: threadpool.c threadpool.h
	$(CC) $(CFLAGS) -c threadpool.c $(PTHREADS)

queue.o: queue.c 
	$(CC) $(CFLAGS) -c queue.c ${PTHREADS}

clean:
	rm -rf *.o
	rm -rf example

