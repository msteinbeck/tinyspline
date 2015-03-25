CC=gcc
CCX=g++
CFLAGS=-g \
       -Werror -Wall -Wextra -Wfloat-equal -Winit-self -Wno-missing-braces \
       -Wunreachable-code -Wundef -Wswitch-default -Wswitch-enum

all: c

init:
	mkdir -p bin
	mkdir -p build
	mkdir -p gen

clean:
	rm -rf bin
	rm -rf build
	rm -rf gen

c: init
	cp src/tinyspline.h bin/tinyspline.h
	$(CC) $(CFLAGS) -c src/tinyspline.c -o build/tinyspline.static.o
	ar rcs bin/libtinyspline.a build/tinyspline.static.o
	$(CC) $(CFLAGS) -c -fpic src/tinyspline.c -o build/tinyspline.shared.o
	$(CC) --shared -o bin/libtinyspline.so build/tinyspline.shared.o

cpp: init
	cp src/tinyspline.h bin/tinyspline.h
	cp src/tinysplinecpp.h bin/tinysplinecpp.h
	$(CC) $(CFLAGS) -c src/tinyspline.c -o build/tinyspline.static.o -lstdc++
	$(CC) $(CFLAGS) -c src/tinysplinecpp.cpp -o build/tinysplinecpp.static.o -lstdc++
	ar rcs bin/libtinysplinecpp.a build/tinyspline.static.o build/tinysplinecpp.static.o
	$(CC) $(CFLAGS) -c -fpic src/tinyspline.c -o build/tinyspline.shared.o -lstdc++
	$(CC) $(CFLAGS) -c -fpic src/tinysplinecpp.cpp -o build/tinysplinecpp.shared.o -lstdc++
	$(CXX) -shared -o bin/libtinysplinecpp.so build/tinyspline.shared.o build/tinysplinecpp.shared.o

java: lib
	javac -cp libs/jna-4.1.0.jar -d build/ src/*.java
	jar cvf bin/tinyspline.jar -C build com
	cp libs/jna-4.1.0.jar bin/jna-4.1.0.jar

examples: c
	rm bin/libtinyspline.so # remove shared lib to ensure examples use static lib
	$(CC) $(CFLAGS) -Lbin src/bspline.c -o bin/bspline -lGL -lGLU -lglut -Bstatic -ltinyspline
	$(CC) $(CFLAGS) -Lbin src/nurbs.c -o bin/nurbs -lGL -lGLU -lglut  -Bstatic -ltinyspline
	$(CC) $(CFLAGS) -Lbin src/split.c -o bin/split -lGL -lGLU -lglut  -Bstatic -ltinyspline
