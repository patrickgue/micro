CC=clang

PROG=micro
SRCS=micro.c ioi.c instr.c helper.c loader.c
OBJS=$(SRCS:.c=.o)

CFLAGS=-g


$(PROG):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	make -C asmc/
	make -C asms/


%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@


clean:
	rm -f $(PROG) $(OBJS)
	make -C asmc clean
	make -C asms clean


