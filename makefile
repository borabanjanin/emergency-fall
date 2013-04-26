emergency: rs232.o main.o gnuplot_i.o
	gcc main.o rs232.o gnuplot_i.o -o3 -o emergency -lm

main.o: main.c main.h
	gcc -c main.c

rs232.o: rs232.c
	gcc -c rs232.c

clean: rs232.c main.c 
	gcc main.c rs232.c -o3 -o emergency -lm

gnuplot_i.o: gnuplot_i.c
	gcc -c gnuplot_i.c

clean: 
	rm -rf *.o
