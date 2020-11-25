#################################################
#						#
#	File:		makefile		#
#	Date:		23.11.2020		#
#	Author:		Isa Dzhumabaev		#
#						#
#################################################

CC=gcc
CFLAGS += -Wall -g -Wformat

skiplist_test: skiplist.o skiplist_test.o
	$(CC) -o skiplist_test skiplist.o skiplist_test.o
	rm -rf *.o

skiplist: skiplist.c
	$(CC) -c -o skiplist.o skiplist.c


clean:
	rm -rf *.o *.exe


tdebug: skiplist.o skiplist_test.c
	$(CC) -D TDEBUG -o skiplist_test skiplist.o skiplist_test.c
	rm -rf *.o
