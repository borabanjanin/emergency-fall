emergency: rs232.o main.o gnuplot_i.o
	gcc -L/usr/lib/python2.7/config -lpthread -ldl -lutil -lm -lpython2.7 -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions -g main.o rs232.o gnuplot_i.o  -o3 -o emergency -lm 

main.o: main.c main.h
	gcc -g -c main.c

rs232.o: rs232.c processdata.c
	gcc -g -c rs232.c

gnuplot_i.o: gnuplot_i.c gnuplot_i.h
	gcc -g -c gnuplot_i.c

clean: 
	rm -rf *.o
