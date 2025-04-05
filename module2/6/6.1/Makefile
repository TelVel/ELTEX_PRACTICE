all: build

build: menu

menu:
	gcc -c loglist.c
	gcc loglist.o -o loglist -L. loglistlib.a
	rm *.o

lib:
	gcc -c loglist.c loglistfunc.c
	gcc -c loglist.h -o loglisth.o
	ar r loglistlib.a *.o
	rm *.o

clean:
	rm loglist
	rm loglistlib.a
