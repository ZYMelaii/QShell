.DEFAULT_GOAL := all

CFLAGS := -I../../include -L../../build -lqsh

.PHONY: hashmap.c qshw.c

hashmap.c:
	cd $@ && gcc *.c -o test ${CFLAGS}

qshw.c:
	cd $@ && gcc *.c -o test ${CFLAGS}

all: hashmap.c qshw.c