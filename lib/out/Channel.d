Channel.d: ../base/Channel.cc ../base/Channel.h ../base/base.h \
  ../base/EventLoop.h ../base/Mutex.h ../base/Poller.h \
  ../base/EventChannel.h
Channel.o: ../base/Channel.cc ../base/Channel.h ../base/base.h \
  ../base/EventLoop.h ../base/Mutex.h ../base/Poller.h \
  ../base/EventChannel.h
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../base/Channel.cc
