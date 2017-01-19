lrbPoller.d: ../lrbBase/lrbPoller.cc ../lrbBase/lrbPoller.h
lrbPoller.o: ../lrbBase/lrbPoller.cc ../lrbBase/lrbPoller.h
	cc -c -std=c++11 -I.. -I../lrbBase -I../lrbLog -I../lrbLoop -I../lrbThread -I../out ../lrbBase/lrbPoller.cc
