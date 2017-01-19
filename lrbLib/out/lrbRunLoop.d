lrbRunLoop.d: ../lrbLoop/lrbRunLoop.cc ../lrbLoop/lrbRunLoop.h \
  ../lrbBase/lrbTask.h ../lrbLoop/lrbLoopPoller.h ../lrbBase/lrbPoller.h
lrbRunLoop.o: ../lrbLoop/lrbRunLoop.cc ../lrbLoop/lrbRunLoop.h \
  ../lrbBase/lrbTask.h ../lrbLoop/lrbLoopPoller.h ../lrbBase/lrbPoller.h
	cc -c -std=c++11 -I.. -I../lrbBase -I../lrbLog -I../lrbLoop -I../lrbThread -I../out ../lrbLoop/lrbRunLoop.cc
