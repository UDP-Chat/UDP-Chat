all: proj

proj:
	mkdir Debug
	rm -f Debug/a.out
	g++ -std=c++11 -lpthread main.cpp BufferParser.cpp HeartBeat.cpp HoldbackQueue.cpp Members.cpp MessageStore.cpp UDP.cpp -o Debug/a.out

