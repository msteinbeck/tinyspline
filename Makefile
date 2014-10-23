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

examples: lib
	$(CC) $(CFLAGS) -lGL -lGLU -lglut src/bspline.c -o bin/bspline bin/libtinyspline.a
	$(CC) $(CFLAGS) -lGL -lGLU -lglut src/nurbs.c -o bin/nurbs bin/libtinyspline.a
