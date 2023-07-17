CC=clang
CF=-c -Wall --optimize -I headers
CFD=-c -Wall -g -fsanitize=address -I headers
NUM=1 2
NAMES=args compare dir files getdent hashing paths main
SRC=$(foreach name,$(NAMES),$(name).c)
OBJECTS=$(foreach name,$(NAMES),$(name).o)
DBGOBJ=$(foreach name,$(NAMES),$(name)-debug.o)

prod: prog clean

prog: $(OBJECTS)
	$(CC) $(OBJECTS) -lcrypto -o main

debug: prog-debug clean

prog-debug: $(DBGOBJ)
	$(CC) $(DBGOBJ) -g -fsanitize=address -lcrypto -o main-debug

$(DBGOBJ): $(SRC)
	@echo Building debug
	@i=0
	@for i in $(NUM) ; do \
		echo;\
	done
	@for file in $(SRC); do \
		$(CC) $(CFD) $$file -o $${file%.c}-debug.o; \
		if [ $$? = 0 ]; then \
			echo $${file} compiled ;\
		else \
			echo $${file} did not compile ;\
			exit 1 ;\
		fi \
	done

$(OBJECTS): $(SRC)
	@echo Building main
	@i=0
	@for i in $(NUM) ; do \
		echo;\
	done
	@for file in $(SRC); do \
		$(CC) $(CF) $$file -o $${file%.c}.o; \
		if [ $$? = 0 ]; then \
			echo $${file} compiled ;\
		else \
			echo $${file} did not compile ;\
			exit 1 ;\
		fi \
	done

clean:
	rm -rf *.o
