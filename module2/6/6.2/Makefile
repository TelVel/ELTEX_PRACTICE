all:
	gcc -c *.c
	gcc -o logsorted *.o -L../6.2 -l:libdynamo.so -Wl,-rpath,/root/projects/ELTEX_PRACTICE/module2/6/6.2
clean:
	rm -f logsorted
	rm -f *.o