cs3421_emul: parse.o cpu.o memory.o clock.o imemory.o cache.o
	g++ -Wall -fno-common -g -o cs3421_emul $^

parse.o: cpu.h memory.h clock.h imemory.h
cpu.o: cpu.h
memory.o: memory.h
clock.o: clock.h
imemory.o: imemory.h
cache.o: cache.h

parse.o: parse.cpp
	g++ -Wall -fno-common -g -c $<

cpu.o: cpu.cpp
	g++ -Wall -fno-common -g -c $<

memory.o: memory.cpp
	g++ -Wall -fno-common -g -c $<

clock.o: clock.cpp
	g++ -Wall -fno-common -g -c $<

imemory.o: imemory.cpp
	g++ -Wall -fno-common -g -c $<

cache.o: cache.cpp
	g++ -Wall -fno-common -g -c $<

clean:
	rm -f *o cs3421_emul 