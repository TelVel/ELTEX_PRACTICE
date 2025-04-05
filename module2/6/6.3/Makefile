all: build

build: menu

menu: calcimproved.o libsum.so libsub.so libdiv.so libmult.so libpower.so
	gcc -o calcimproved calcimproved.o -L../6.3 -l:libdiv.so -l:libmult.so -l:libsub.so -l:libsum.so -l:libpower.so -Wl,-rpath,/home/telvel/projects/ELTEX_PRACTICE/module2/6/6.3/ -lm
	rm *.o

Calc.o: calcimproved.c calc.h
	gcc -c calcimproved.c

list: calcsum.c calcsub.c calcdiv.c calcmult.c calcpower.c calc.h
	gcc -c -fPIC calcsum.c
	gcc -shared -o libsum.so calcsum.o
	gcc -c -fPIC calcsub.c
	gcc -shared -o libsub.so calcsub.o
	gcc -c -fPIC calcdiv.c
	gcc -shared -o libdiv.so calcdiv.o
	gcc -c -fPIC calcmult.c
	gcc -shared -o libmult.so calcmult.o
	gcc -c -fPIC calcpower.c
	gcc -shared -o libpower.so calcpower.o
	rm *.o

clean:
	rm calcimproved
	rm libdiv.so
	rm libsum.so
	rm libsub.so
	rm libmult.so
	rm libpower.so
