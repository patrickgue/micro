CFLAGS=-g

ASMC_SRCS=asmc.c
ASMC_OBJS=$(ASMC_SRCS:.c=.o)
ASMC_PROG=asmc

$(ASMC_PROG):$(ASMC_OBJS)
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -rf $(ASMC_PROG) $(ASMC_OBJS)
