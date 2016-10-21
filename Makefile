all:
	(cd tthreads ; make install)
	(cd bank; make)
	(cd demo-threads; make)
	(cd consumer; make)


.PHONY: clean all 

clean:
	(cd tthreads ; make clean)
	(cd bank; make clean)
	(cd demo-threads; make clean)
	(cd consumer; make clean)
	rm -rf h lib
