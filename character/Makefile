
libcharacter.so : character.o
	cc -fPIC -shared -o libcharacter.so character.o

character.o : character.h

.PHONY : clean
clean :
	-rm character.o libcharacter.so
