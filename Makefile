all:
	(cd tthreads ; make install)
	(cd demo-bank; make)
	(cd demo-threads; make)


.PHONY: clean all tthreads demo-bank demo-threads #declare these targets as phony

clean:
	(cd tthreads ; make clean)
	(cd demo-bank; make clean)
	(cd demo-threads; make clean)
	rm -rf h lib
