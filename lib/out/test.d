test.d: ../pathfind/test.cc
test.o: ../pathfind/test.cc
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../pathfind/test.cc
