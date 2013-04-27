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

typedef struct PacketInfo{
	short type_input;
	short sample_number;
}PacketInfo;

short samplenumber = 0;
short typeinput = 0;
PacketInfo chest_info;
PacketInfo thigh_info;
Sample chest_samples[1000];
Sample thigh_samples[1000];
Sample* chest;
Sample* thigh;
double accel_array[1000];
FILE *logfile;
gnuplot_ctrl * plot_handle_chest;
gnuplot_ctrl * plot_handle_thigh;

#include "processdata.c"

