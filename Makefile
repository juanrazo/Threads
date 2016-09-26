all:
	(cd tthreads ; make install)
	(cd bank; make)
	(cd demo-threads; make)


.PHONY: clean all 

clean:
	(cd tthreads ; make clean)
	(cd bank; make clean)
	(cd demo-threads; make clean)
	rm -rf h lib
