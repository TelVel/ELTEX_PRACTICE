all:
	gcc chmod.c -o chmod
clean:
	rm -f chmod
runtest1:
		./chmod r-x-wx-rw
		./chmod 431
		./chmod u+rw test1
		./chmod g+xr test1
		./chmod a+rwx test1
		./chmod o+x test1
		./chmod a-rwx chmod