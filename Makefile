CC=clang

PROG=micro
SRCS=micro.c ioi.c instr.c helper.c loader.c
OBJS=$(SRCS:.c=.o)

CFLAGS=


$(PROG):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@


clean:
	rm -f $(PROG) $(ASMS_PROG) $(OBJS)



ASMS_SRCS=asms.c
ASMS_OBJS=$(ASMS_SRCS:.c=.o)
ASMS_PROG=asms

$(ASMS_PROG):$(ASMS_OBJS)
	$(CC) $(CFLAGS) $^ -o $@
