#ifndef PROCESSDATA
#define PROCESSDATA
#include <math.h>
#include "gnuplot_i.h"


#define CHEST 0
#define THIGH 1
#define TRUE 1
#define FALSE 0
#define PI 3.14159265
//#define PLOTCHESTANGLE
//#define PLOTTHIGHANGLE
//#define PLOTCHESTACCEL
//#define PLOTCHESTACCEL



double accel_chest[25];
double accel_thigh[25];
double x_angle_chest[25];
double y_angle_chest[25];
double z_angle_chest[25];
double x_angle_thigh[25];
double y_angle_thigh[25];
double z_angle_thigh[25];

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

int GraphData(short sensor_id, SensorInfo* point_data, Sample* point){

	if(CHEST == sensor_id){
#ifdef  PLOTCHESTACCEl
		accel_chest[chest_sample] = (point->accel)/16000;
		gnuplot_resetplot(plot_handle_chest);
		gnuplot_set_xlabel(plot_handle_chest, "Chest Accel g/s");
		gnuplot_cmd(plot_handle_chest, "set yrange [0:2]");
		gnuplot_setstyle(plot_handle_chest, "lines");
		 if(FALSE == chest_samples_fill){
			gnuplot_plot_x(plot_handle_chest, accel_chest, chest_sample, "test");
		}else{
			gnuplot_plot_x(plot_handle_chest, accel_chest, 25, "test");
		}
#endif

#ifdef PLOTCHESTANGLE
		printf("chest angle plotting\n");
		x_angle_chest[chest_sample] = point_data->xangle_comp;
		y_angle_chest[chest_sample] = point_data->yangle_comp;
		z_angle_chest[chest_sample] = point_data->zangle_comp;

		gnuplot_resetplot(plot_handle_chest_angle);
		gnuplot_set_xlabel(plot_handle_chest_angle, "Chest Angle");
		gnuplot_cmd(plot_handle_chest_angle, "set yrange [-180:180]");
		gnuplot_setstyle(plot_handle_chest_angle, "lines");

		if(FALSE == chest_samples_fill){
			gnuplot_plot_x(plot_handle_chest_angle, x_angle_chest, chest_sample, "x angle");
			gnuplot_plot_x(plot_handle_chest_angle, y_angle_chest, chest_sample, "y angle");
			gnuplot_plot_x(plot_handle_chest_angle, z_angle_chest, chest_sample, "z angle");
		}else{
			gnuplot_plot_x(plot_handle_chest_angle, x_angle_chest, 25, "x angle");
			gnuplot_plot_x(plot_handle_chest_angle, y_angle_chest, 25, "y angle");
			gnuplot_plot_x(plot_handle_chest_angle, z_angle_chest, 25, "z angle");
		}
#endif
		if(chest_sample < 24){
			chest_sample++;
		}else{
			chest_sample = 0;
			chest_samples_fill = TRUE;
		}
	}

	if(THIGH == sensor_id){
#ifdef PLOTTHIGHACCEl
		accel_thigh[thigh_sample] = (point->accel)/16000;
		gnuplot_resetplot(plot_handle_thigh);
		gnuplot_set_xlabel(plot_handle_thigh, "Thigh Accel g/s");
		gnuplot_cmd(plot_handle_thigh, "set yrange [0:2]");
		gnuplot_setstyle(plot_handle_thigh, "lines");

		 if(FALSE == thigh_samples_fill){
			gnuplot_plot_x(plot_handle_thigh, accel_thigh, thigh_sample, "test");
		}else{
			gnuplot_plot_x(plot_handle_thigh, accel_thigh, 25, "test");
		}
#endif

#ifdef PLOTTHIGHANGLE
		printf("thigh angle plotting\n");
		x_angle_thigh[thigh_sample] = point_data->xangle_comp;
		y_angle_thigh[thigh_sample] = point_data->yangle_comp;
		z_angle_thigh[thigh_sample] = point_data->zangle_comp;

		gnuplot_resetplot(plot_handle_thigh_angle);
		gnuplot_set_xlabel(plot_handle_thigh_angle, "Thigh Angle");
		gnuplot_cmd(plot_handle_thigh_angle, "set yrange [-180:180]");
		gnuplot_setstyle(plot_handle_thigh_angle, "lines");

		if(FALSE == thigh_samples_fill){
			gnuplot_plot_x(plot_handle_thigh_angle, x_angle_thigh, thigh_sample, "x angle");
			gnuplot_plot_x(plot_handle_thigh_angle, y_angle_thigh, thigh_sample, "y angle");
			gnuplot_plot_x(plot_handle_thigh_angle, z_angle_thigh, thigh_sample, "z angle");
		}else{
			gnuplot_plot_x(plot_handle_thigh_angle, x_angle_thigh, 25, "x angle");
			gnuplot_plot_x(plot_handle_thigh_angle, y_angle_thigh, 25, "y angle");
			gnuplot_plot_x(plot_handle_thigh_angle, z_angle_thigh, 25, "z angle");
		}
#endif

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


int AccelAngle(SensorInfo* sensor, Sample* point){
	double xaccel = (double)point->xaccel;
	double yaccel = (double)point->yaccel;
	double zaccel = (double)point->zaccel;

	sensor->xangle_accel = acos(xaccel/(point->accel))*180.0/PI;
	sensor->yangle_accel = acos(yaccel/(point->accel))*180.0/PI;
	sensor->zangle_accel = acos(zaccel/(point->accel))*180.0/PI;

	return 0;
}

int ComplementaryFilter(SensorInfo* sensor, Sample* point) {
	double time_constant = 0.50;

	printf("Pre comp angle: %f %f %f \n\n",sensor->xangle_comp,sensor->yangle_comp,sensor->zangle_comp);
	double x_ang_vel = (double)sensor->data_array[sensor->sample_number].xrot;
	x_ang_vel = 250 * x_ang_vel/32767.5;
	sensor->xangle_comp = time_constant * (sensor->xangle_comp * x_ang_vel * sensor->dt) + (1 - time_constant) * sensor->xangle_accel;

	double y_ang_vel = (double)sensor->data_array[sensor->sample_number].yrot;
	y_ang_vel = 250 * y_ang_vel/32767.5;
	sensor->yangle_comp = time_constant * (sensor->yangle_comp * y_ang_vel * sensor->dt) + (1 - time_constant) * sensor->yangle_accel;

	double z_ang_vel = (double)sensor->data_array[sensor->sample_number].zrot;
	z_ang_vel = 250 * z_ang_vel/32767.5;
	sensor->zangle_comp = time_constant * (sensor->zangle_comp * z_ang_vel * sensor->dt) + (1 - time_constant) * sensor->zangle_accel;

	printf("accel_vel: %f %f %f \n ", sensor->xangle_accel, sensor->yangle_accel, sensor->zangle_accel);
	printf("ang_vel: %f %f %f \n", x_ang_vel, y_ang_vel, z_ang_vel);
	printf("Post comp angle: %f %f %f \n\n",sensor->xangle_comp,sensor->yangle_comp,sensor->zangle_comp);
	return 0;

}

#endif
