CC=gcc
CFLAGS=-g \
       -Werror -Wall -Wextra -Wfloat-equal -Winit-self -Wno-missing-braces \
       -Wshadow -Wunreachable-code -Wundef -Wnested-externs -Winline \
       -Wcast-align -Wbad-function-cast -Wcast-qual -Wpointer-arith \
       -Wswitch-default -Wswitch-enum

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
	javac -cp libs/jna-4.1.0.jar -d build/ src/*.java
	jar cvf bin/tinyspline.jar -C build com
	cp libs/jna-4.1.0.jar bin/jna-4.1.0.jar

examples: lib
	rm bin/libtinyspline.so # remove shared lib to ensure examples use static lib
	$(CC) $(CFLAGS) -Lbin src/bspline.c -o bin/bspline -lGL -lGLU -lglut -Bstatic -ltinyspline
	$(CC) $(CFLAGS) -Lbin src/nurbs.c -o bin/nurbs -lGL -lGLU -lglut  -Bstatic -ltinyspline
	$(CC) $(CFLAGS) -Lbin src/split.c -o bin/split -lGL -lGLU -lglut  -Bstatic -ltinyspline
