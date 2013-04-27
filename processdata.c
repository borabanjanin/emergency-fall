#ifndef PROCESSDATA
#define PROCESSDATA
#include <math.h>
#include "gnuplot_i.h"

#define CHEST 0
#define THIGH 1
#define TRUE 1
#define FALSE 0

double accel_chest[25];
double accel_thigh[25];
int chest_sample = 0;
int thigh_sample = 0;
short chest_samples_fill = FALSE;
short thigh_samples_fill = FALSE;
extern gnuplot_ctrl * plot_handle_chest;
extern gnuplot_ctrl * plot_handle_thigh;

Sample* ProcessData(Sample* point){
	double xaccelsq = (double) point->xaccel;
	double yaccelsq = (double) point->yaccel;
	double zaccelsq = (double) point->zaccel;

	xaccelsq = pow(xaccelsq,2.0);
	yaccelsq = pow(yaccelsq,2.0);
	zaccelsq = pow(zaccelsq,2.0);
	point ->accel = sqrt(xaccelsq + yaccelsq + zaccelsq);
	return point;
}

int GraphData(int sensor_id, Sample* point){

	if(CHEST == sensor_id){
		accel_chest[chest_sample] = (point->accel)/16000;
		gnuplot_resetplot(plot_handle_chest);

		if(FALSE == chest_samples_fill){
			gnuplot_plot_x(plot_handle_chest, accel_chest, chest_sample, "test");
		}else{
			gnuplot_plot_x(plot_handle_chest, accel_chest, 25, "test");
		}

		if(chest_sample < 24){
			chest_sample++;
		}else{
			chest_sample = 0;
		}
	}

/*
	if(THIGH == sensor_id){
		accel_thigh[thigh_sample] = (sequence[point_number]->accel)/1600;
		if(thigh_sample >= 25){
			gnuplot_resetplot(plot_handle_thigh);
			gnuplot_plot_x(plot_handle_thigh, accel_thigh, thigh_sample, "test");
			thigh_sample = 0;
		}else{
			thigh_sample++;
		}
	}
	*/
	return 0;
}










#endif
