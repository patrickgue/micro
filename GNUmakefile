CC=clang

PROG=micro
SRCS=micro.c ioi.c instr.c helper.c loader.c registers.c
OBJS=$(SRCS:.c=.o)

CFLAGS=-g


$(PROG):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	make -C asmc/
	make -C asms/


%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@


install:$(PROG)
	mkdir -p build
	cp micro asm asmc/asmc asms/asms test/calc3.asmc build

run:$(install)
	build/asm test/calc3.asmc test/calc3.bin
	build/micro test/calc3.bin

clean:
	rm -rf $(PROG) $(OBJS) build memory.bin
	make -C asmc clean
	make -C asms clean

