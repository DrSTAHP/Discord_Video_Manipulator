vidmanip: vidmanip.o
	gcc vidmanip.o -o vidmanip
	rm *.o

vidmanip.o: src/vidmanip.c
	gcc -c src/vidmanip.c