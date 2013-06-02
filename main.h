#include "gnuplot_i.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0


typedef struct Sample{
	unsigned short n;
	int xaccel;
	int yaccel;
	int zaccel;
	int xrot;
	int yrot;
	int zrot;
	double accel;
	double ang_accel;

}Sample;

typedef struct SensorInfo{
	short type_input;
	short sample_number;
	short cali_active;
	Sample data_array[1000];
	double dt;
	double xangle_accel;
	double yangle_accel;
	double zangle_accel;
	double xangle_comp;
	double yangle_comp;
	double zangle_comp;
	double comp_ang;
	double moving_accel;
	double moving_accel_array[3];
	double moving_ang;
	double moving_ang_array[3];
	short moving_index;
	short data_fill;

}SensorInfo;

typedef struct CalibrateInfo{
	short fill;
	int xaccel;
	int yaccel;
	int zaccel;
	int xrot;
	int yrot;
	int zrot;
	double accel;
	double one_g;
	double ang_accel;
}CalibrateInfo;

SensorInfo chest_info;
SensorInfo thigh_info;
//Sample chest_samples[1000];
//Sample thigh_samples[1000];
Sample* data_point;
SensorInfo* sensor_ptr;
double accel_array[1000];
FILE *logfile;
gnuplot_ctrl * plot_handle_chest;
gnuplot_ctrl * plot_handle_thigh;
gnuplot_ctrl * plot_handle_chest_angle;
gnuplot_ctrl * plot_handle_thigh_angle;
CalibrateInfo cali_chest;
unsigned char buf[4096];
int buffersize;

