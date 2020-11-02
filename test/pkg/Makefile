all: testc testcxx
.PHONY: all

testc:
	cc test.c `pkg-config --cflags --libs tinyspline` -Wl,-rpath=`pkg-config --variable=libdir tinyspline` -o testc

testcxx:
	c++ test.cxx `pkg-config --cflags --libs tinysplinecxx` -Wl,-rpath=`pkg-config --variable=libdir tinysplinecxx` -o testcxx

test: testc testcxx
	./testc
	./testcxx

clean:
	rm -f testc testcxx
