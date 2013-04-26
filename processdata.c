#ifndef PROCESSDATA
#define PROCESSDATA
#include <math.h>
#include "gnuplot_i.h"

double accel_chest[10];
double accel_thigh[10];
int chest_sample = 0;
int thigh_sample = 0;
extern gnuplot_ctrl * plot_handle;

double ProcessData(){
	double xaccelsq = (double) Samples[samplenumber].xaccel;
	double yaccelsq = (double) Samples[samplenumber].yaccel;
	double zaccelsq = (double) Samples[samplenumber].zaccel;
	double accel;

	xaccelsq = pow(xaccelsq,2.0);
	yaccelsq = pow(yaccelsq,2.0);
	zaccelsq = pow(zaccelsq,2.0);
	accel_array[samplenumber] = sqrt(xaccelsq + yaccelsq + zaccelsq);
	fprintf(logfile,"Acceleration: %f\n",accel_array[samplenumber]);
	return accel;
}


int GraphData(int sensor_id, double accel){
	if(sensor_id == 0){
		accel_chest[chest_sample] = accel;
		gnuplot_plot_x(plot_handle, accel_chest, chest_sample, "test");
		if(chest_sample >= 10){
			chest_sample = 0;
		}else{
			chest_sample++;
		}
	}

	if(sensor_id == 1){
		accel_thigh[thigh_sample] = accel;
		if(thigh_sample >= 10){
			thigh_sample = 0;
		}else{
			thigh_sample++;
		}
	}
	return 0;
}










#endif
