#include "gnuplot_i.h"

typedef struct Sample{
	unsigned short n;
	int xaccel;
	int yaccel;
	int zaccel;
	int xrot;
	int yrot;
	int zrot;

}Sample;

short samplenumber = 0;
short typeinput = 0;
Sample Samples[1000];
double accel_array[1000];
FILE *logfile;
gnuplot_ctrl * plot_handle;

#include "processdata.c"

