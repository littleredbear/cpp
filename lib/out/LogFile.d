LogFile.d: ../log/LogFile.cc ../log/LogFile.h
LogFile.o: ../log/LogFile.cc ../log/LogFile.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../log/LogFile.cc
