all:
	make -C source
	make -C app -f Makefile.debug 


clean:
	rm -rf image/*
	make -C source clean
	make -C app -f Makefile.debug clean
	rm -f out/* 

