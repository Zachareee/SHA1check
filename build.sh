#!/bin/bash
OBJECTS='args compare dir files getdent hashing paths main'
CF='--optimize'
SUFFIX=
OUTPUT=

if [[ $1 = 'debug' ]]; then
    CF='-g -fsanitize=address'
    SUFFIX='-debug'
fi

for file in $OBJECTS; do
    if clang -c -Wall -I headers $CF $file.c -o $file$SUFFIX.o; then
        echo $file.c compiled ;
    else
        echo $file.c did not compile ;
        exit 1 ;
    fi
    OUTPUT+="$file$SUFFIX.o "
    echo
done

if clang $OUTPUT $CF -lcrypto -o SHA1check$SUFFIX; then
    echo SHA1check built
else
    echo
    echo Something went wrong...
fi

rm -rf *.o
