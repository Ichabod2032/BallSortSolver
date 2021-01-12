CFLAGS = -Wall -Werror -pedantic

solver : queue.o
	gcc $(CFLAGS) -o solver queue.o solver.c

debug : queue.o
	gcc $(CFLAGS) -DDBG1 -o dbg queue.o solver.c

queue.o :
	gcc $(CFLAGS) -c queue.c

test : queue.o
	gcc $(CFLAGS) -o test queue.o unittest.c

.PHONY : clean
clean :
	rm -f queue.o test solver dbg
