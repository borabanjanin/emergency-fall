#include "gnuplot_i.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Sample{
	unsigned short n;
	int xaccel;
	int yaccel;
	int zaccel;
	int xrot;
	int yrot;
	int zrot;
	double accel;

}Sample;

short samplenumber = 0;
short typeinput = 0;
Sample Samples[1000];
Sample* chest;
double accel_array[1000];
FILE *logfile;
gnuplot_ctrl * plot_handle_chest;
gnuplot_ctrl * plot_handle_thigh;

#include "processdata.c"

