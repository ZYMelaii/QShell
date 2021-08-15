.DEFAULT_GOAL := all

MD := mkdir.exe
RM := rm.exe
MV := mv.exe

CC  := gcc
CXX := g++
AR  := ar

CWARNINGS := -Wall -Wno-discarded-qualifiers -Wno-comment
CFLAGS    := ${CWARNINGS} -DQSHELL_BUILD
LFLAGS    := -I. -L. -L../build

CORE_OBJS    := shellio.o mmem.o hashmap.o
ENVR_OBJS    := envr.o var.o
PARSER_OBJS  := parser.o lexer.o tokenizer.o scanner.o word.o syntax.o
CMDLINE_OBJS := cmdline.o
EVAL_OBJS    := builtin.o
UIMAN_OBJS   := uiman.o
QSHW_OBJS    := qshw.o

LIB_OBJS := ${CORE_OBJS} ${ENVR_OBJS} ${PARSER_OBJS} \
	${CMDLINE_OBJS} ${EVAL_OBJS} ${UIMAN_OBJS} \
	${QSHW_OBJS}

TARGET_OBJS := core.obj envr.obj parser.obj \
	cmdline.obj eval.obj uiman.obj qshw.obj

.SUFFIXES: .c .cpp

*.c.o:
	${CC} -c $<

*.cpp.o:
	${CXX} -c $<

libqsh.a: ${TARGET_OBJS}
	${AR} rcs $@ ${LIB_OBJS}

qshell: shell.c libqsh.a
	${CC} $< -o $@ ${CFLAGS} ${LFLAGS} -lqsh

all: libqsh.a qshell move

move:
	${MD} -p ../build
	${MV} -f *.o *.exe *.a ../build

.PHONY: ${TARGET_OBJS}

core.obj: ${CORE_OBJS} core.h utf8str.h

envr.obj: ${ENVR_OBJS} envr.h

parser.obj: ${PARSER_OBJS} parser.h syntax.h cmdflags.h

cmdline.obj: ${CMDLINE_OBJS} cmdline.h

eval.obj: ${EVAL_OBJS}  eval.h

uiman.obj: ${UIMAN_OBJS} uiman.h

qshw.obj: ${QSHW_OBJS} qshw.h

.PHONY: clean
clean:
	-@${RM} *.o *.a *.exe
	-@${RM} -r ../build