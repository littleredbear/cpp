Thread.o  Thread.d: ../base/Thread.cc ../base/Thread.h ../base/base.h \
  ../base/Exception.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind  ../base/Thread.cc
