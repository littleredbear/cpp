
out: logt.cc
	g++ -I../lib/include -L../lib -llrb logt.cc -o logt.out

.PHONY : clean
clean :
	-rm logt.out
