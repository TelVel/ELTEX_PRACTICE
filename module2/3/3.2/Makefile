all:
	gcc iptest.c -o iptest
clean:
	rm -f iptest
runtest1:
	./iptest 192.168.1.1 255.0.0.0 100000
	./iptest 192.168.1.1 255.255.0.0 100000
	./iptest 192.168.1.1 255.255.0.0 10000000