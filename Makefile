CC=gcc
CFLAGS=-Wall -Werror

all: lib

init:
	mkdir -p bin
	mkdir -p build
	cp src/bspline.h bin/bspline.h
	
clean:
	rm -rf bin
	rm -rf build

lib: init
	$(CC) $(CFLAGS) -c src/bspline.c -o build/bspline.static.o
	ar rcs bin/bspline.a build/bspline.static.o
	$(CC) $(CFLAGS) -c -fpic src/bspline.c -o build/bspline.shared.o
	$(CC) --shared -o bin/bspline.so build/bspline.shared.o
