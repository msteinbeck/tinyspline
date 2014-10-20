CC=gcc
CFLAGS=-g -Wall -Werror

all: lib

init:
	mkdir -p bin
	mkdir -p build
	cp src/tinyspline.h bin/tinyspline.h
	
clean:
	rm -rf bin
	rm -rf build

lib: init
	$(CC) $(CFLAGS) -c src/tinyspline.c -o build/tinyspline.static.o
	ar rcs bin/libtinyspline.a build/tinyspline.static.o
	$(CC) $(CFLAGS) -c -fpic src/tinyspline.c -o build/tinyspline.shared.o
	$(CC) --shared -o bin/libtinyspline.so build/tinyspline.shared.o
