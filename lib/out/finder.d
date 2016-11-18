finder.d: ../pathfind/finder.cc ../pathfind/finder.h
finder.o: ../pathfind/finder.cc ../pathfind/finder.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../pathfind/finder.cc
