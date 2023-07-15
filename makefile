CC=clang
CF=-c -Wall --optimize -I headers
CFD=-c -Wall -g -fsanitize=address -I headers
OBJECTS=args.o compare.o dir.o files.o getdent.o hashing.o paths.o main.o
DBGOBJ=args-debug.o compare-debug.o dir-debug.o files-debug.o getdent-debug.o hashing-debug.o paths-debug.o main-debug.o
SOURCE=args.c compare.c dir.c files.c getdent.c hashing.c paths.c main.c

all: prog clean

prog: $(OBJECTS)
	$(CC) $(OBJECTS) -lcrypto -o main

debug: prog-debug clean

prog-debug: $(SOURCE)
	foreach src, $(SOURCE), $(echo $(src))
	#$(CC) $(SOURCE) -g -fsanitize=address -o $(SOURCE)-debug.o))
	$(CC) $(DBGOBJ) -g -fsanitize=address -lcrypto -o main

args.o:args.c
	$(CC) $(CF) args.c

compare.o:compare.c
	$(CC) $(CF) compare.c

dir.o:dir.c
	$(CC) $(CF) dir.c

files.o:files.c
	$(CC) $(CF) files.c

getdent.o:getdent.c
	$(CC) $(CF) getdent.c

hashing.o:hashing.c
	$(CC) $(CF) hashing.c

paths.o:paths.c
	$(CC) $(CF) paths.c

main.o:main.c
	$(CC) $(CF) main.c

clean:
	rm -rf *.o
