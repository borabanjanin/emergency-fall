#ifndef PROCESSDATA
#define PROCESSDATA
#include <math.h>
#include "gnuplot_i.h"


#define CHEST 0
#define THIGH 1
#define TRUE 1
#define FALSE 0
#define PI 3.14159265

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

int GraphData(short sensor_id, Sample* point){

	if(CHEST == sensor_id){
		accel_chest[chest_sample] = (point->accel)/16000;
		gnuplot_resetplot(plot_handle_chest);
		gnuplot_set_xlabel(plot_handle_chest, "Chest");
		gnuplot_cmd(plot_handle_chest, "set yrange [0:2]");
		gnuplot_setstyle(plot_handle_chest, "lines");
		 if(FALSE == chest_samples_fill){
			gnuplot_plot_x(plot_handle_chest, accel_chest, chest_sample, "test");
		}else{
			gnuplot_plot_x(plot_handle_chest, accel_chest, 25, "test");
		}

		if(chest_sample < 24){
			chest_sample++;
		}else{
			chest_sample = 0;
			chest_samples_fill = TRUE;
		}
	}

	if(THIGH == sensor_id){
		accel_thigh[thigh_sample] = (point->accel)/16000;
		gnuplot_resetplot(plot_handle_thigh);
		gnuplot_set_xlabel(plot_handle_thigh, "Thigh");
		gnuplot_cmd(plot_handle_thigh, "set yrange [0:2]");
		gnuplot_setstyle(plot_handle_thigh, "lines");
		 if(FALSE == thigh_samples_fill){
			gnuplot_plot_x(plot_handle_thigh, accel_thigh, thigh_sample, "test");
		}else{
			gnuplot_plot_x(plot_handle_thigh, accel_thigh, 25, "test");
		}

		if(thigh_sample < 24){
			thigh_sample++;
		}else{
			thigh_sample = 0;
			thigh_samples_fill = TRUE;
		}
	}

	return 0;
}

int CalibrationRoutine(Sample* point){
	if(FALSE == cali_chest.fill){
		cali_chest.fill = TRUE;
		cali_chest.xaccel = point->xaccel;
		cali_chest.yaccel = point->yaccel;
		cali_chest.zaccel = point->zaccel;
		cali_chest.xrot = point->xrot;
		cali_chest.yrot = point->yrot;
		cali_chest.zrot = point->zrot;
		cali_chest.accel = point->accel;
		cali_chest.one_g = point->accel;
	}else{
		cali_chest.one_g = (point->accel-cali_chest.one_g)*0.2+(point->accel);
	}
	return 0;
}


int AccelAngle(SensorInfo sensor){
	Sample* point = &sensor.data_array[sensor.sample_number];
	double xaccel = (double)point->xaccel;
	double yaccel = (double)point->yaccel;
	double zaccel = (double)point->zaccel;
	sensor.xangle = acos(xaccel/(point->accel))*180.0/PI;
	sensor.yangle = acos(yaccel/(point->accel))*180.0/PI;
	sensor.zangle = acos(zaccel/(point->accel))*180.0/PI;
	printf("%f %f %f \n",sensor.xangle,sensor.yangle,sensor.zangle);
	return 0;
}

#endif
