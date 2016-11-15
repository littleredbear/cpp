test.o  test.d: ../test.cc ../t2.h ../t1.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind  ../test.cc
