all: mutexTest setjmpDemo tdemo-simple tdemo-nuanced

.PHONY: clean all		#declare these targets as phony

clean:
	rm -f *.o *.exe *stackdump mutexTest setjmpDemo threadTest1

CFLAGS=-g -O1 -fno-ipa-sra -L../lib -I../h

mutexTest: mutexTest.o ../lib/tthreads.a
setjmpDemo: setjmpDemo.o ../lib/tthreads.a
tdemo-simple: tdemo-simple.o ../lib/tthreads.a
tdemo-nuanced: tdemo-nuanced.o ../lib/tthreads.a
