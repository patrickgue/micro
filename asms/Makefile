CFLAGS=

ASMS_SRCS=asms.c
ASMS_OBJS=$(ASMS_SRCS:.c=.o)
ASMS_PROG=asms

$(ASMS_PROG):$(ASMS_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(ASMS_PROG) $(ASMS_OBJS)
