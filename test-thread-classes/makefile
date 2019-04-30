output: main.o lib.o
	g++ main.o lib.o -o output

main.o: main.cpp
	g++ -c `pkg-config --cflags opencv` main.cpp `pkg-config --libs opencv` -pthread

lib.o: lib.cpp lib.h
	#g++ -c lib.cpp
	g++ -c `pkg-config --cflags opencv` lib.cpp `pkg-config --libs opencv` -pthread

target: dependencies
	action

clean:
	rm *.o output
