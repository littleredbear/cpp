Vec2.d: ../pathfind/Vec2.cpp ../pathfind/Vec2.h ../pathfind/CCMathBase.h \
  ../pathfind/Vec2.inl
Vec2.o: ../pathfind/Vec2.cpp ../pathfind/Vec2.h ../pathfind/CCMathBase.h \
  ../pathfind/Vec2.inl
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../pathfind/Vec2.cpp
