all: tthreads.a

CFLAGS=-g -O1 -fno-ipa-sra

tthreads.a: tthreads.o mutex.o assert2.o
	ar crs $@ $^

install: tthreads.a
	mkdir -p ../h ../lib
	mv $^ ../lib
	cp *.h ../h

clean:
	rm -f tthreads.a *.o

