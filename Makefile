#
#	$@: target
#	$<: first independence
#
CFLAGS += -Werror

SRCS1 += main.c
OGJS1 += main.o
PROG1  = main

all: ${PROG1}

${PROG1}: ${OGJS1}
	gcc ${CFLAGS} ${OGJS1} -o $@

%.o: $.c
	gcc ${CFLAGS} -c $<

clean:
	rm -f *.o ${PROG1}