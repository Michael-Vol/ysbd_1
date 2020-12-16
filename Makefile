CC = gcc
CFLAFS = -g -Wall

prog: main.o util.o heap.o
	$(CC) $(CFLAFS) -o prog main.o util.o heap.o BF/BF_64.a -no-pie
main.o:
	$(CC) $(CFLAFS) -c main.c 
util.o:
	$(CC) $(CFLAFS) -c util.c 
heap.o:
	$(CC) $(CFLAFS) -c HP/heap.c 
.PHONY: clean
clean:
	rm -f prog main.o heap.o util.o