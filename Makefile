CC=clang

PROG=micro
SRCS=micro.c ioh.c
OBJS=$(SRCS:.c=.o)

CFLAGS=


$(PROG):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@


clean:
	rm -f $(PROG) $(OBJS)
