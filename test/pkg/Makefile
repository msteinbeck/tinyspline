CC=cc
CXX=c++

all: testc testcxx
.PHONY: all

testc:
	${CC} test.c `pkg-config --cflags --libs tinyspline` -Wl,-rpath `pkg-config --variable=libdir tinyspline` -o testc

testcxx:
	${CXX} test.cxx `pkg-config --cflags --libs tinysplinecxx` -Wl,-rpath `pkg-config --variable=libdir tinysplinecxx` -o testcxx

test: testc testcxx
	./testc
	./testcxx

clean:
	rm -f testc testcxx
