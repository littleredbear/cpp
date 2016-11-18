EventLoop.d: ../base/EventLoop.cc ../base/EventLoop.h ../base/base.h \
  ../base/Mutex.h ../base/Poller.h ../base/Channel.h \
  ../base/EventChannel.h
EventLoop.o: ../base/EventLoop.cc ../base/EventLoop.h ../base/base.h \
  ../base/Mutex.h ../base/Poller.h ../base/Channel.h \
  ../base/EventChannel.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../base/EventLoop.cc
