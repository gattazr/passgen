CC=gcc
CFLAGS= -g -Wall
SRCS= passgen.c params.c
LIBRARIES=
OBJS = ${SRCS:.c=.o}

passgen: $(OBJS)
	$(CC) $(CFLAGS) -o passgen $(OBJS) $(LIBRARIES)
clean:
	rm -r passgen $(OBJS)


# dependency list
passgen.o: passgen.h params.h
params.o: params.h
