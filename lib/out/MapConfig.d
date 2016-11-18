MapConfig.d: ../pathfind/MapConfig.cc ../pathfind/MapConfig.h \
  ../pathfind/Vec2.h ../pathfind/CCMathBase.h ../pathfind/Vec2.inl
MapConfig.o: ../pathfind/MapConfig.cc ../pathfind/MapConfig.h \
  ../pathfind/Vec2.h ../pathfind/CCMathBase.h ../pathfind/Vec2.inl
	cc -c -I.. -I../base -I../log -I../net -I../object -I../out -I../pathfind ../pathfind/MapConfig.cc
