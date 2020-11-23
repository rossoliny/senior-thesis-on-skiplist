CC=gcc
CFLAGS += -Wformat -g

skiplist_test: skiplist.o skiplist_test.o
	$(CC) -o skiplist_test skiplist.o skiplist_test.o
	rm -rf *.o

skiplist: skiplist.c
	$(CC) -c -o skiplist.o skiplist.c


clean:
	rm -rf *.o *.exe
