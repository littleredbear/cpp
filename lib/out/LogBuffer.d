LogBuffer.o  LogBuffer.d: ../log/LogBuffer.cc ../log/LogBuffer.h ../base/base.h \
  ../base/Buffer.h ../base/Thread.h ../log/LogFile.h ../base/Mutex.h \
  ../base/Condition.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind  ../log/LogBuffer.cc
