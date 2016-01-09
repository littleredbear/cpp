
dir:= $(shell pwd)
dir_H:=$(dir)/include/
dir_O:=$(dir)/objects/
dir_C:=$(dir)/source/

vpath %.o $(dir_O)
vpath %.cc $(dir_C)
vpath %.h $(dir_H)

CC:=g++

afile:=liblrb.a
ofiles:=lrbCharacter.o lrbLog.o lrbException.o
hfiles:=$(ofiles:%.o=%.h)
cfiles:=$(addprefix $(dir_C),$(ofiles:%.o=%.cc))

$(afile) : $(ofiles)
	$(AR) $(ARFLAGS) $(afile) $(ofiles)

$(ofiles) : $(hfiles)
	$(CC) -c -I$(dir_H) $(cfiles)
#lrbLog.o : lrbLog.h lrb.h lrbLog.cc
#	$(CC) -c $(dir_C)lrbLog.cc

.PHONY : clean
clean:
	-rm *.a
	-rm *.o

.PHONY : print
print :
	@echo $(AR)
