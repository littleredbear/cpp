lrbTask.d: ../lrbBase/lrbTask.cc ../lrbBase/lrbTask.h
lrbTask.o: ../lrbBase/lrbTask.cc ../lrbBase/lrbTask.h
	cc -c -std=c++11 -I.. -I../lrbBase -I../lrbLog -I../lrbLoop -I../lrbThread -I../out ../lrbBase/lrbTask.cc
