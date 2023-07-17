CC=clang
CF=-c -Wall --optimize -I headers
CFD=-c -Wall -g -fsanitize=address -I headers
NAMES=args compare dir files getdent hashing paths main
SRC=$(foreach name,$(NAMES),$(name).c)
OBJECTS=$(foreach name,$(NAMES),$(name).o)
DBGOBJ=$(foreach name,$(NAMES),$(name)-debug.o)

all: prog clean

prog: $(OBJECTS)
	$(CC) $(OBJECTS) -lcrypto -o main

debug: prog-debug clean

prog-debug: $(DBGOBJ)
	$(CC) $(DBGOBJ) -g -fsanitize=address -lcrypto -o main-debug

$(DBGOBJ): $(SRC)
	@for file in $(SRC); do \
		$(CC) $(CFD) $$file -o $${file%.c}-debug.o; \
		echo $${file%.c} compiled; \
	done

$(OBJECTS): $(SRC)
	@for file in $(SRC); do \
		$(CC) $(CF) $$file -o $${file%.c}.o; \
		echo $${file} compiled; \
	done

#args.o:args.c
#	$(CC) $(CF) args.c
#
#compare.o:compare.c
#	$(CC) $(CF) compare.c
#
#dir.o:dir.c
#	$(CC) $(CF) dir.c
#
#files.o:files.c
#	$(CC) $(CF) files.c
#
#getdent.o:getdent.c
#	$(CC) $(CF) getdent.c
#
#hashing.o:hashing.c
#	$(CC) $(CF) hashing.c
#
#paths.o:paths.c
#	$(CC) $(CF) paths.c
#
#main.o:main.c
#	$(CC) $(CF) main.c

clean:
	rm -rf *.o
