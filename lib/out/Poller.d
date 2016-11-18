Poller.d: ../base/Poller.cc ../base/Poller.h ../base/base.h \
  ../base/Channel.h ../base/EventLoop.h ../base/Mutex.h \
  ../base/EventChannel.h ../log/LogStream.h ../log/LogBuffer.h \
  ../base/Buffer.h ../base/Thread.h ../log/LogFile.h ../base/Condition.h
Poller.o: ../base/Poller.cc ../base/Poller.h ../base/base.h \
  ../base/Channel.h ../base/EventLoop.h ../base/Mutex.h \
  ../base/EventChannel.h ../log/LogStream.h ../log/LogBuffer.h \
  ../base/Buffer.h ../base/Thread.h ../log/LogFile.h ../base/Condition.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../base/Poller.cc
