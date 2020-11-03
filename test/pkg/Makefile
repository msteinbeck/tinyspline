CC=gcc
CXX=g++

all: testc testcxx
.PHONY: all

testc:
	${CC} test.c --no-pie `pkg-config --cflags --libs tinyspline` -Wl,-rpath `pkg-config --variable=libdir tinyspline` -o testc

testcxx:
	${CXX} test.cxx --no-pie `pkg-config --cflags --libs tinysplinecxx` -Wl,-rpath `pkg-config --variable=libdir tinysplinecxx` -o testcxx

test: testc testcxx
	./testc
	./testcxx

clean:
	rm -f testc testcxx
