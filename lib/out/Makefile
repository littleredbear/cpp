

-include makeconf

src_files:=$(shell find $(projdir) -name $(src_pattern))
pure_files:=$(notdir $(src_files))
file_names:=$(basename $(pure_files))
src_dirs:=$(shell find $(projdir) -type d)

VPATH:=$(src_dirs)

$(target) : $(addsuffix .o, $(file_names))
	$(cc) $(cflags) -o $(target) $^

%.d : %.c
	@set -e;rm -f $@;\
	$(cc) -MM $< > $@.$$$$;\
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$
#	$(cc) -MM $< > $@


.PHONY : clean
clean :
	-rm -rf *.o *.d $(target)

-include  $(addsuffix .d, $(file_names))






