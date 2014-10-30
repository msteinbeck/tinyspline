CC=gcc
CFLAGS=-g -Wall -Werror

all: lib

init:
	mkdir -p bin
	mkdir -p build

clean:
	rm -rf bin
	rm -rf build

lib: init
	cp src/tinyspline.h bin/tinyspline.h
	$(CC) $(CFLAGS) -c src/tinyspline.c -o build/tinyspline.static.o
	ar rcs bin/libtinyspline.a build/tinyspline.static.o
	$(CC) $(CFLAGS) -c -fpic src/tinyspline.c -o build/tinyspline.shared.o
	$(CC) --shared -o bin/libtinyspline.so build/tinyspline.shared.o

java: lib
	javac -cp libs/jna-4.1.0.jar -d build/ src/Binding.java
	jar cvf bin/tinyspline.jar -C build com
	cp libs/jna-4.1.0.jar bin/jna-4.1.0.jar

examples: lib
	$(CC) $(CFLAGS) -lGL -lGLU -lglut src/bspline.c -o bin/bspline bin/libtinyspline.a
	$(CC) $(CFLAGS) -lGL -lGLU -lglut src/nurbs.c -o bin/nurbs bin/libtinyspline.a
