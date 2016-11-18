EPoller.d: ../net/EPoller.cc
EPoller.o: ../net/EPoller.cc
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../net/EPoller.cc
