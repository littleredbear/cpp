

-include makeconf

ifndef src_dirs
src_dirs:=$(shell find $(projdir) -type d)
endif

src_files:=$(shell find $(src_dirs) -maxdepth 1 -name $(src_pattern))
pure_files:=$(notdir $(src_files))
file_names:=$(basename $(pure_files))
#file_names:=$(basename $(src_files))

ifndef include_dirs
include_dirs:=$(addprefix -I, $(src_dirs))
else
include_dirs:=$(addprefix -I, $(include_dirs))
endif

VPATH:=$(src_dirs)

$(target) : $(addsuffix .o, $(file_names))
	$(cc) $(lflags) -o $@ $^


define createdfile
	@set -e;rm -f $1;\
	$(cc) -MM $(include_dirs) $2 > $1.$$$$;\
	sed 's/\($*\).o[:]/\1.d:/g' < $1.$$$$ > $1;\
	cat $1.$$$$ >> $1;\
	echo "\t$(cc) -c $(cflags) $(include_dirs) $<" >> $1;\
	rm -f $1.$$$$

endef

%.d : %.c
	$(call createdfile, $@, $<)

%.d : %.cc
	$(call createdfile, $@, $<)

%.d : %.cpp
	$(call createdfile, $@, $<)


.PHONY : clean
clean :
	-rm -rf *.log *.o *.d.* *.d *.so $(target)

-include  $(addsuffix .d, $(file_names))






