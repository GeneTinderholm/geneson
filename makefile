runtest: test
	./test
test: geneson.o test.o
	gcc -g -o test test.o geneson.o
geneson.o:
	gcc -c -g geneson.c
test.o:
	gcc -c -g test.c
clean:
	rm *.o
	rm test
