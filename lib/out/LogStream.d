LogStream.d: ../log/LogStream.cc ../log/LogStream.h ../log/LogBuffer.h \
  ../base/base.h ../base/Buffer.h ../base/Thread.h ../log/LogFile.h \
  ../base/Mutex.h ../base/Condition.h
LogStream.o: ../log/LogStream.cc ../log/LogStream.h ../log/LogBuffer.h \
  ../base/base.h ../base/Buffer.h ../base/Thread.h ../log/LogFile.h \
  ../base/Mutex.h ../base/Condition.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../log/LogStream.cc
