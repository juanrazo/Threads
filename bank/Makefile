all: dep-wd-race dep-wd-ok transfer-bad transfer-good

.PHONY: clean all		#declare these targets as phony

clean:
	rm -f *.o *.exe *stackdump dep-wd-race dep-wd-ok transfer-bad transfer-good

CFLAGS=-g -O1 -fno-ipa-sra -L../lib -I../h

# include files (forces recompile if changed)

dep-wd-race: dep-wd-race.o ../lib/tthreads.a
dep-wd-ok:  dep-wd-ok.o ../lib/tthreads.a
transfer-bad:  transfer-bad.o ../lib/tthreads.a
transfer-good: transfer-good.o ../lib/tthreads.a
