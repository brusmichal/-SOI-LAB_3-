CXX = g++
CXXFLAGS = -Wall  -g -pthread

all: main
	
main: main.cpp monitor.h
	${CXX} ${CXXFLAGS} $< -o $@

clean:
	rm -rf *.o