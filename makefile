#all: rs232.c main.c 
#	gcc main.c rs232.c -o3 -o emergency -lm
emergency: rs232.o main.o
	gcc main.o rs232.o -o3 -o emergency -lm

main.o: main.c
	gcc -c main.c

rs232.o: rs232.c
	gcc -c rs232.c
