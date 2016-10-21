all: consumer

.PHONY: clean all		#declare these targets as phony

clean:
	rm -f *.o *.exe *stackdump consumer

CFLAGS=-g -O1 -fno-ipa-sra -L../lib -I../h

# include files (forces recompile if changed)

consumer: consumer.o ../lib/tthreads.a
