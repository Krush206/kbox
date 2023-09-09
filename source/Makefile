all:
	for dir in `ls`; do if [ -d $$dir ]; then make -C $$dir -f Makefile.kbox; fi; done;

clean:
	for dir in `ls`; do if [ -d $$dir ]; then make -C $$dir -f Makefile.kbox clean; fi; done;

