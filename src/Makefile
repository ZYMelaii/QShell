.DEFAULT_GOAL := all

CC := gcc
CXX := g++
AR := ar
CWARNINGS := -Wall -Wno-discarded-qualifiers -Wno-comment
CFLAGS := ${CWARNINGS} -I. -L. -L../build

LIB_OBJS := shellio.o mmem.o \
	envr.o hashmap.o var.o \
	cmdline.o builtin.o parse.o chrmap.o \
	uiman.o \
	qshw.o

.SUFFIXES: .c .cpp

*.c.o:
	@${CC} -c $<

*.cpp.o:
	${CXX} -c $<

libqsh.a: core.obj envr.obj cmdline.obj eval.obj uiman.obj qshw.obj
	${AR} rcs $@ ${LIB_OBJS}

qshell: shell.c libqsh.a
	${CC} $< -o $@ -lqsh ${CFLAGS}

all: libqsh.a qshell move

move:
	@if not exist ..\build (mkdir ..\build)
	mv -f *.o *.exe *.a ..\build

core.obj: shellio.o mmem.o hashmap.o core.h

envr.obj: envr.o var.o envr.h

cmdline.obj: cmdline.o cmdline.h

eval.obj: builtin.o parse.o chrmap.o eval.h

uiman.obj: uiman.o uiman.h

qshw.obj: qshw.o qshw.h

.PHONY: clean
clean:
	-@rm *.o *.a *.exe
	-@rm -r ..\build