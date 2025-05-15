all:
	gcc client.c -o client
	gcc server.c -o server
	mkdir -p servdir
	touch bin.txt
	date +%c > bin.txt
clean:
	rm -f server
	rm -f client
	rm -rf servdir
	rm -f bin.txt