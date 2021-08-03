.DEFAULT_GOAL := all

CFLAGS := -I../../include -L../../build -lqsh

.PHONY: hashmap.c qshw.c sample.0

hashmap.c:
	cd $@ && gcc *.c -o test ${CFLAGS}

qshw.c:
	cd $@ && gcc *.c -o test ${CFLAGS}

sample.0:
	cd sample && gcc 0.c -o 0.exe ${CFLAGS}

all: hashmap.c qshw.c