CC = gcc

all: build

build: menu

menu: loglist.o liblinked.so
	$(CC) loglist.o -o loglist -L../6.2 -llinked -Wl,-rpath,/home/telvel/projects/ELTEX_PRACTICE/module2/6/6.2/
	rm *.o

phonebook.o: loglist.c loglist.h
	$(CC) -c loglist.c

list: loglistfunc.c loglist.h
	$(CC) -c -fPIC loglistfunc.c
	$(CC) -shared -o liblinked.so loglistfunc.o
	rm *.o

clean:
	rm loglist
	rm liblinked.so