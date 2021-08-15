.DEFAULT_GOAL := all

MD := mkdir.exe
RM := rm.exe
MV := mv.exe
CP := cp.exe

INC_EXCLUDE := syntax.h cmdflags.h

.PHONY: static test inc clean

static:
	cd src && make all

test: inc
	cd test && make all

inc:
	${MD} -p include/qsh
	${CP} -rf src/*.h include/qsh
	cd include/qsh && ${RM} -rf ${INC_EXCLUDE}

all: static test inc

clean:
	-@${RM} -r build include
	cd test && make clean