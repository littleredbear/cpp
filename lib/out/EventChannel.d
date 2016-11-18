EventChannel.d: ../base/EventChannel.cc ../base/EventChannel.h \
  ../base/Channel.h ../base/base.h ../base/EventLoop.h ../base/Mutex.h \
  ../base/Poller.h
EventChannel.o: ../base/EventChannel.cc ../base/EventChannel.h \
  ../base/Channel.h ../base/base.h ../base/EventLoop.h ../base/Mutex.h \
  ../base/Poller.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../base/EventChannel.cc
