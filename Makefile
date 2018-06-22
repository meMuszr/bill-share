default: run
folder:
	mkdir -p build
build: folder
	gcc -Wall -lsqlite3 -o build/test src/test.c bin/bill-share.o
debug:
	gcc -g -Wall -c src/bill-share.c -o bin/bill-share.o
	gcc -lsqlite3 -g src/test.c src/bill-share.c 
run: build
