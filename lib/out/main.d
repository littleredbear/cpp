main.o  main.d: ../pathfind/main.cc ../pathfind/Vec2.h ../pathfind/CCMathBase.h \
  ../pathfind/Vec2.inl
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind  ../pathfind/main.cc
