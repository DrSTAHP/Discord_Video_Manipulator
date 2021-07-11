vidmanip: vidmanip.o mp4.o webm.o shared.o
	gcc vidmanip.o mp4.o webm.o shared.o -o vidmanip
	rm *.o

vidmanip.o: src/vidmanip.c
	gcc -c src/vidmanip.c

mp4.o: src/mp4.c src/mp4.h
	gcc -c src/mp4.c

webm.o: src/webm.c src/webm.h
	gcc -c src/webm.c

shared.o: src/shared.c src/shared.h
	gcc -c src/shared.c
	