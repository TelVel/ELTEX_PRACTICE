all:
	gcc -c *.c
	gcc -o calcimproved *.o -L../6.3 -l:libdiv.so -l:libmult.so -l:libsub.so -l:libsum.so -l:libpower.so -Wl,-rpath,/root/projects/ELTEX_PRACTICE/module2/6/6.3 -lm
clean:
	rm -f calcimproved
	rm *.o
