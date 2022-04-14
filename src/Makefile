.DEFAULT_GOAL := all

CC := gcc
CXX := g++
AR := ar
CFLAGS := -w -I. -I..\include -L. -L..\build

LIB_OBJS := shellio.o mmem.o \
	envr.o hashmap.o var.o \
	cmdline.o builtin.o parse.o chrmap.o \
	uiman.o \
	qshw.o

.SUFFIXES: .c .cpp

*.c.o:
	@$(CC) -c $<

*.cpp.o:
	$(CXX) -c $<

libqsh.a: core.obj envr.obj cmdline.obj eval.obj uiman.obj qshw.obj
	$(AR) rcs $@ $(LIB_OBJS)

qshell: shell.c libqsh.a
	$(CC) $< -o $@ -lqsh $(CFLAGS)

all: libqsh.a qshell move

move:
	@if [ ! -d "../build" ]; then mkdir ../build; fi
	mv -f *.o *.exe *.a ../build

core.obj: shellio.o mmem.o hashmap.o

envr.obj: envr.o var.o

cmdline.obj: cmdline.o

eval.obj: builtin.o parse.o chrmap.o

uiman.obj: uiman.o

qshw.obj: qshw.o

.PHONY: clean
clean:
	-@rm -r ..\build