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
	rm -f $(PROG) $(ASMS_PROG) $(ASMC_PROG) $(OBJS)


asm:$(ASMSPROG) $(ASMCPROG)

ASMS_SRCS=asms.c asmhelper.c
ASMS_OBJS=$(ASMS_SRCS:.c=.o)
ASMS_PROG=asms

$(ASMS_PROG):$(ASMS_OBJS)
	$(CC) $(CFLAGS) $^ -o $@


ASMC_SRCS=asms.c asmhelper.c
ASMC_OBJS=$(ASMC_SRCS:.c=.o)
ASMC_PROG=asmc

$(ASMC_PROG):$(ASMC_OBJS)
	$(CC) $(CFLAGS) $^ -o $@
