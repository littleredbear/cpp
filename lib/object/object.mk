
vpath %.o $(dir_O)
vpath %.h $(dir_H)

hfile:=$(shell ./pfile.sh $(cfile))
ofile:=$(notdir $(cfile:.cc=.o))
$(ofile) : $(cfile) $(hfile)
	@echo build $@
	$(CC) -c $(CPPFLAGS) -I$(dir_H) $<

.PHONY : print
print :
	@echo build $(MAKEFILE_LIST)

.PHONY : clean
clean :
	-rm *.o
