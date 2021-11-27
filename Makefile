CC=gcc -g -Wall

refcnt.o: refcnt.c refcnt.h
	$(CC) -c refcnt.c

test_refcnt: test_refcnt.c refcnt.o
	$(CC) -o test_refcnt test_refcnt.c refcnt.o
clean:
	rm -f test_refcnt refcnt.o 
