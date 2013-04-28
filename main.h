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

typedef struct SensorInfo{
	short type_input;
	short sample_number;
	Sample data_array[1000];
}SensorInfo;

SensorInfo chest_info;
SensorInfo thigh_info;
//Sample chest_samples[1000];
//Sample thigh_samples[1000];
Sample* data_point;
double accel_array[1000];
FILE *logfile;
gnuplot_ctrl * plot_handle_chest;
gnuplot_ctrl * plot_handle_thigh;


