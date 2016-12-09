#
#	$@: target
#	$<: first independence
#
CFLAGS += -Werror

SRCS1 += main.c client_list.c safe.c debug.c
OGJS1 += main.o client_list.o safe.o debug.o
PROG1  = main

SRCS2 += wd_enhance.c
OGJS2 += wd_enhance.o
PROG2  = wd_enhance

all: ${PROG1} ${PROG2}

${PROG1}: ${OGJS1}
	gcc ${CFLAGS} ${OGJS1} -o $@

${PROG2}: ${OGJS2}
	gcc ${CFLAGS} ${OGJS2} -o $@

%.o: $.c
	gcc ${CFLAGS} -c $<

clean:
	rm -f *.o ${PROG1} ${PROG2}