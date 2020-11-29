#################################################
#						#
#	File:		makefile		#
#	Date:		23.11.2020		#
#	Author:		Isa Dzhumabaev		#
#						#
#################################################

CC=cc
CFLAGS += -Wall -g -Wformat -pedantic -std=c99

skiplist_test: skiplist.c skiplist_test.c
	$(CC) $(CFLAGS) -o skiplist_test.out skiplist.c skiplist_test.c

clean:
	rm -rf *.o *.exe *.out *.dSYM

tdebug: skiplist.c skiplist_test.c
	$(CC) -D TDEBUG $(CFLAGS) -g -o debug_skiplist_test.out skiplist.c skiplist_test.c
	rm -rf *.o *.dSYM
