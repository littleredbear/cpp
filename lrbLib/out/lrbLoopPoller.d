lrbLoopPoller.d: ../lrbLoop/lrbLoopPoller.cc ../lrbLoop/lrbLoopPoller.h \
  ../lrbBase/lrbPoller.h
lrbLoopPoller.o: ../lrbLoop/lrbLoopPoller.cc ../lrbLoop/lrbLoopPoller.h \
  ../lrbBase/lrbPoller.h
	cc -c -std=c++11 -I.. -I../lrbBase -I../lrbLog -I../lrbLoop -I../lrbThread -I../out ../lrbLoop/lrbLoopPoller.cc
