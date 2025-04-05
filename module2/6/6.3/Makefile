all: build

build: menu

menu: calcimproved.o libsum.so libsub.so libdiv.so libmult.so libpower.so
	gcc -c calcimproved.c
	gcc -rdynamic calcimproved.o -o calcimproved -ldl
	rm *.o

Calc.o: calcimproved.c calc.h
	gcc -c calcimproved.c

list: calcsum.c calcsub.c calcdiv.c calcmult.c calcpower.c calc.h
	gcc -c -fPIC calcsum.c
	gcc -shared -o lib/libsum.so calcsum.o
	gcc -c -fPIC calcsub.c
	gcc -shared -o lib/libsub.so calcsub.o
	gcc -c -fPIC calcdiv.c
	gcc -shared -o lib/libdiv.so calcdiv.o
	gcc -c -fPIC calcmult.c
	gcc -shared -o lib/libmult.so calcmult.o
	gcc -c -fPIC calcpower.c
	gcc -shared -o lib/libpower.so calcpower.o
	rm *.o

clean:
	rm calcimproved
	rm lib/libdiv.so
	rm lib/libsum.so
	rm lib/libsub.so
	rm lib/libmult.so
	rm lib/libpower.so
