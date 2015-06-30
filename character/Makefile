out : character.so test.o 
	cc -L. -lcharacter -o out test.o

test.o : test.cc
	cc -c test.cc

character.so : character.cc character.h
	cc -fPIC -shared -o libcharacter.so character.cc

clean:
	rm test.o libcharacter.so out
