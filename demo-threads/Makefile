all: mutexTest setjmpDemo threadTest1

.PHONY: clean all		#declare these targets as phony

clean:
	rm -f *.o *.exe *stackdump mutexTest setjmpDemo threadTest1

CFLAGS=-g -O2 -fno-ipa-sra -L../lib -I../h

mutexTest: mutexTest.o ../lib/tthreads.a
setjmpDemo: setjmpDemo.o ../lib/tthreads.a
threadTest1: threadTest1.o ../lib/tthreads.a
