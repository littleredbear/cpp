
dir:= $(shell pwd)
dir_H:=$(dir)/
dir_O:=$(dir)/object/

CC:=g++
CPPFLAGS:=-std=c++11

export dir dir_H dir_O CC CPPFLAGS


afile:=liblrb.a
pfiles:=$(shell find $(dir) -name *.cc)
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
