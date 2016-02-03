
dir:= $(shell pwd)
dir_H:=$(dir)/include/
dir_O:=$(dir)/object/
dir_C:=$(dir)/source/

CC:=g++

export dir_H dir_O dir_C CC


afile:=liblrb.a
pfiles:=$(shell ls $(dir_C)*.cc)
ofiles:=$(addprefix $(dir_O),$(notdir $(pfiles:.cc=.o)))
list:=$(filter %.mk,$(shell ls $(dir_O)))
mkfile:=object.mk

.PHONY : $(afile)
$(afile) : 
	$(foreach n,$(pfiles),$(call submake,$(n)))
	$(AR) $(ARFLAGS) $@ $(ofiles)


define submake
	$(MAKE) -C $(dir_O) -f $(mkfile) cfile=$1

endef

.PHONY : clean
clean:
	-rm *.a
	$(MAKE) -C $(dir_O) -f $(mkfile) clean

#.PHONY : test
#test :
#	@for target in $(pfiles);\
	do\
		$(MAKE) -C $(dir_O) -f $(mkfile) hfile=$$target;\
	done

#.PHONY : test1
#test1 :
#	$(foreach n,$(pfiles),$(call submake,$(n)))

#tsh := $(shell ./test.sh)
#.PHONY : testSH
#testSH :
#	@echo $(tsh)
#	@echo $(shell ./cfile.sh $(tsh) ".h")

.PHONY : print
print :
	@echo $(AR)
	@echo $(list)
	@echo $(MAKEFILES)
	@echo $(MAKEFILE_LIST)
