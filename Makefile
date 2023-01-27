cs3421_emul: parse.o cpu.o memory.o clock.o
	g++ -Wall -fno-common -g -o cs3421_emul $^

parse.o: cpu.h memory.h clock.h
cpu.o: cpu.h
memory.o: memory.h
clock.o: clock.h

parse.o: parse.cpp
	g++ -Wall -fno-common -g -c $<

cpu.o: cpu.cpp
	g++ -Wall -fno-common -g -c $<

memory.o: memory.cpp
	g++ -Wall -fno-common -g -c $<

clock.o: clock.cpp
	g++ -Wall -fno-common -g -c $<

clean:
	rm -f *o cs3421_emul 