
all : subsystem out
.PHONY : all

out : test.o
	cc -L ../character -lcharacter -o out test.o

test.o : test.cc

subsystem:
	cd ../character && $(MAKE) MAKEFLAGS=-w

.PHONY : clean
clean : subclean currentclean

.PHONY : subclean
subclean :
	cd ../character && make clean

.PHONY : currentclean
currentclean:
	-rm test.o out	
