CC=clang
CF=$(CFLAGS)
CFLAGS=-c -Wall -I headers
CFLAGSS=-c -Wall -fsanitize=address -I headers
OBJECTS=args.o compare.o files.o hashing.o paths.o main.o
all: prog clean

prog: $(OBJECTS)
	$(CC) $(OBJECTS) -lcrypto -fsanitize=address -o main

args.o:args.c
	$(CC) $(CF) args.c

compare.o:compare.c
	$(CC) $(CF) compare.c

files.o:files.c
	$(CC) $(CF) files.c

hashing.o:hashing.c
	$(CC) $(CF) hashing.c

paths.o:paths.c
	$(CC) $(CF) paths.c

main.o:main.c
	$(CC) $(CF) main.c

clean:
	rm -rf *.o
