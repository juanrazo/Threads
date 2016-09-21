all: bank_simple bank_mutex bank_transfer_bad 

.PHONY: clean all		#declare these targets as phony

clean:
	rm -f *.o *.exe *stackdump bank_simple bank_mutex bank_transfer_bad 

CFLAGS=-g -O2 -fno-ipa-sra -L../lib -I../h

# include files (forces recompile if changed)

bank_simple: bank_simple.o ../lib/tthreads.a
bank_mutex:  bank_mutex.o ../lib/tthreads.a
bank_transfer_bad:  bank_transfer_bad.o ../lib/tthreads.a
