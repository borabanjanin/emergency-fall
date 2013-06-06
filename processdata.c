#ifndef PROCESSDATA
#define PROCESSDATA
#include <math.h>
#include "gnuplot_i.h"


#define CHEST 0
#define THIGH 1
#define TRUE 1
#define FALSE 0
#define PI 3.14159265
#define LOG
//#define PLOTCHESTANGLE
//#define PLOTTHIGHANGLE
#define PLOTTHIGHACCEl
#define PLOTCHESTACCEl
//#define VISUALIZATION


double accel_chest[25];
double accel_thigh[25];
double angle_chest[25];
double angle_thigh[25];
double accel_avg_thigh[3];
double accel_avg_chest[3];

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
		accel_chest[chest_sample] = (point->accel)/8000;
		gnuplot_resetplot(plot_handle_chest);
		gnuplot_set_xlabel(plot_handle_chest, "Chest Accel g/s");
		gnuplot_cmd(plot_handle_chest, "set yrange [0:4]");
		gnuplot_setstyle(plot_handle_chest, "lines");
		 if(FALSE == chest_samples_fill){
			gnuplot_plot_x(plot_handle_chest, accel_chest, chest_sample, "Chest Acceleration");
		}else{
			gnuplot_plot_x(plot_handle_chest, accel_chest, 25, "test");
		}
#endif

#ifdef PLOTCHESTANGLE
		printf("chest angle plotting\n");
		angle_chest[chest_sample] = point_data->moving_ang;

		gnuplot_resetplot(plot_handle_chest_angle);
		gnuplot_set_xlabel(plot_handle_chest_angle, "Chest Angular Velocity");
		gnuplot_cmd(plot_handle_chest_angle, "set yrange [0:250]");
		gnuplot_setstyle(plot_handle_chest_angle, "lines");

		if(FALSE == chest_samples_fill){
			gnuplot_plot_x(plot_handle_chest_angle, angle_chest, chest_sample, "Degrees/Seconds");
		}else{
			gnuplot_plot_x(plot_handle_chest_angle, angle_chest, 25, "Angular Velocity");
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
		accel_thigh[thigh_sample] = (point->accel)/8000;
		gnuplot_resetplot(plot_handle_thigh);
		gnuplot_set_xlabel(plot_handle_thigh, "Thigh Accel g/s");
		gnuplot_cmd(plot_handle_thigh, "set yrange [0:4]");
		gnuplot_setstyle(plot_handle_thigh, "lines");

		 if(FALSE == thigh_samples_fill){
			gnuplot_plot_x(plot_handle_thigh, accel_thigh, thigh_sample, "test");
		}else{
			gnuplot_plot_x(plot_handle_thigh, accel_thigh, 25, "test");
		}
#endif

#ifdef PLOTTHIGHANGLE
			printf("thigh angle plotting\n");
			angle_thigh[thigh_sample] = point_data->moving_ang;

			gnuplot_resetplot(plot_handle_thigh_angle);
			gnuplot_set_xlabel(plot_handle_thigh_angle, "Thigh Angular Velocity");
			gnuplot_cmd(plot_handle_thigh_angle, "set yrange [0:250]");
			gnuplot_setstyle(plot_handle_thigh_angle, "lines");

			if(FALSE == thigh_samples_fill){
				gnuplot_plot_x(plot_handle_thigh_angle, angle_thigh, thigh_sample, "Degrees/Seconds");
			}else{
				gnuplot_plot_x(plot_handle_thigh_angle, angle_thigh, 25, "Angular Velocity");
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

int CalibrationRoutine(short sensor_id, Sample* point){
	if(sensor_id == CHEST){
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
			cali_chest.ang_accel = point->ang_accel;
		}else{
			if(point->accel > cali_chest.one_g){
				cali_chest.one_g = point->accel;
			}
			if(point->ang_accel > cali_chest.ang_accel){
				cali_chest.ang_accel = point->ang_accel;
			}
		}
	}else if(sensor_id == THIGH){
		if(FALSE == cali_thigh.fill){
			cali_thigh.fill = TRUE;
			cali_thigh.xaccel = point->xaccel;
			cali_thigh.yaccel = point->yaccel;
			cali_thigh.zaccel = point->zaccel;
			cali_thigh.xrot = point->xrot;
			cali_thigh.yrot = point->yrot;
			cali_thigh.zrot = point->zrot;
			cali_thigh.accel = point->accel;
			cali_thigh.one_g = point->accel;
			cali_thigh.ang_accel = point->ang_accel;
		}else{
			if(point->accel > cali_chest.one_g){
				cali_thigh.one_g = point->accel;
			}
			if(point->ang_accel > cali_chest.ang_accel){
				cali_thigh.ang_accel = point->ang_accel;
			}
		}
	}
	return 0;
}

int SittingCalibration(short sensor_id, SensorInfo* sensor){
	if(sensor_id == CHEST){
		cali_chest.x_sitting_angle = sensor->xangle_comp;
		cali_chest.y_sitting_angle = sensor->yangle_comp;
		cali_chest.z_sitting_angle = sensor->zangle_comp;
		cali_chest.sitting_fill = TRUE;
	}else if(sensor_id == THIGH){
		cali_thigh.x_sitting_angle = sensor->xangle_comp;
		cali_thigh.y_sitting_angle = sensor->yangle_comp;
		cali_thigh.z_sitting_angle = sensor->zangle_comp;
		cali_thigh.sitting_fill = TRUE;
	}
	return 0;
}

int StandingCalibration(short sensor_id, SensorInfo* sensor){
	if(sensor_id == CHEST){
		cali_chest.x_standing_angle = sensor->xangle_comp;
		cali_chest.y_standing_angle = sensor->yangle_comp;
		cali_chest.z_standing_angle = sensor->zangle_comp;
		cali_chest.standing_fill = TRUE;
	}else if(sensor_id == THIGH){
		cali_thigh.x_standing_angle = sensor->xangle_comp;
		cali_thigh.y_standing_angle = sensor->yangle_comp;
		cali_thigh.z_standing_angle = sensor->zangle_comp;
		cali_thigh.standing_fill = TRUE;
	}
	return 0;
}


int AccelAngle(SensorInfo* sensor, Sample* point){
	double xaccel = (double)point->xaccel;
	double yaccel = (double)point->yaccel;
	double zaccel = (double)point->zaccel;
	//printf("accel %f %f %f \n", xaccel, yaccel, zaccel);
	sensor->xangle_accel = acos(xaccel/(point->accel))*180.0/PI;
	sensor->yangle_accel = acos(yaccel/(point->accel))*180.0/PI;
	sensor->zangle_accel = acos(zaccel/(point->accel))*180.0/PI;
	//printf("xyzrot %d %d %d \n", point->xrot,point->yrot,point->zrot);
	double xrot_sq = 250*(point->xrot)/32767.5;
	double yrot_sq = 250*(point->yrot)/32767.5;
	double zrot_sq = 250*(point->zrot)/32767.5;
	xrot_sq = pow(xrot_sq,2.0);
	yrot_sq = pow(yrot_sq,2.0);
	zrot_sq = pow(zrot_sq,2.0);
	point->ang_accel = sqrt(xrot_sq + yrot_sq + zrot_sq);
	//printf("%f \n",point->ang_accel);
	return 0;
}

int ComplementaryFilter(SensorInfo* sensor, Sample* point) {
	double time_constant = 0.50;

//	printf("Pre comp angle: %f %f %f \n\n",sensor->xangle_comp,sensor->yangle_comp,sensor->zangle_comp);

	double x_ang_vel = (double)sensor->data_array[sensor->sample_number].xrot;
	x_ang_vel = 250 * x_ang_vel/32767.5;
	sensor->xangle_comp = time_constant * (sensor->xangle_comp+x_ang_vel * sensor->dt) + (1 - time_constant) * sensor->xangle_accel;

	double y_ang_vel = (double)sensor->data_array[sensor->sample_number].yrot;
	y_ang_vel = 250 * y_ang_vel/32767.5;
	sensor->yangle_comp = time_constant * (sensor->yangle_comp+y_ang_vel * sensor->dt) + (1 - time_constant) * sensor->yangle_accel;

	double z_ang_vel = (double)sensor->data_array[sensor->sample_number].zrot;
	z_ang_vel = 250 * z_ang_vel/32767.5;
	sensor->zangle_comp = time_constant * (sensor->zangle_comp+z_ang_vel * sensor->dt) + (1 - time_constant) * sensor->zangle_accel;

//	printf("accel_vel: %f %f %f \n ", sensor->xangle_accel, sensor->yangle_accel, sensor->zangle_accel);	
//	printf("ang_vel: %f %f %f \n", x_ang_vel, y_ang_vel, z_ang_vel);
	//printf("Post comp angle: %f %f %f \n\n",sensor->xangle_comp,sensor->yangle_comp,sensor->zangle_comp);

	return 0;
}


//double moving_accel;
//double moving_accel_array[3];
//short moving_accel_index;
//short data_fill;
//double moving_ang;
//double moving_ang_array[3];


int MovingAverage(SensorInfo* sensor, Sample* point){
	if(sensor->sample_number < 3 &&  sensor->data_fill == FALSE){
		if(sensor->sample_number == 0){
			sensor->moving_accel = point-> accel;
			sensor->moving_accel_array[0] = point-> accel;
			sensor->moving_ang = point-> ang_accel;
			sensor->moving_ang_array[0] = point-> ang_accel;
		}else if(sensor->sample_number == 1){
			sensor->moving_accel_array[1] = point-> accel;
			sensor->moving_accel = (sensor->moving_accel_array[1] + sensor->moving_accel_array[0])/2;
			sensor->moving_ang_array[1] = point-> ang_accel;
			sensor->moving_ang = (sensor->moving_ang_array[1] + sensor->moving_ang_array[0])/2;
		}else if(sensor->sample_number == 2){
			sensor->moving_accel_array[2] = point-> accel;
			sensor->moving_accel = (sensor->moving_accel_array[2] +sensor->moving_accel_array[1] + sensor->moving_accel_array[0])/3;
			sensor->moving_ang_array[2] = point-> ang_accel;
			sensor->moving_ang = (sensor->moving_ang_array[2] + sensor->moving_ang_array[1] + sensor->moving_ang_array[0])/3;
			sensor->moving_index = 0;
			sensor->data_fill = TRUE;
			//printf("hit");
		}
	}else{
		sensor->moving_accel_array[sensor->moving_index] = point-> accel;
		sensor->moving_accel = (sensor->moving_accel_array[0] + sensor->moving_accel_array[1] + sensor->moving_accel_array[2])/3;
		sensor->moving_ang_array[sensor->moving_index] = point-> ang_accel;
		sensor->moving_ang = (sensor->moving_ang_array[0] + sensor->moving_ang_array[1] + sensor->moving_ang_array[2])/3;
		sensor->moving_index = ++(sensor->moving_index)%3;
	}
	//printf("Data Fill: %d\n",sensor->data_fill);
	//printf("moving angular: %f\n",sensor->moving_ang);
	//printf("moving accel: %f\n",sensor->moving_accel);
	//printf("sample number: %d",sensorb->sample_number);
	return 0;
}

short fall_detected = FALSE;
short fall_counter = 0;
int FallDetection(short sensor_id, SensorInfo* sensor, Sample* point){	
	static int last_sample_chest;
	static int last_sample_thigh;

#ifdef LOG
	if(sensor_id == CHEST){
		fprintf(logfile, "c ");
	}else{
		fprintf(logfile, "t ");
	}
	fprintf(logfile, "%i ",point->xaccel);
	fprintf(logfile,"%i ",point->yaccel);
	fprintf(logfile,"%i ",point->zaccel);
	fprintf(logfile,"%i ",point->xrot);
	fprintf(logfile,"%i ",point->yrot);
	fprintf(logfile,"%i \n",point->zrot);
#endif
		if(sensor_id == CHEST && cali_chest.fill == TRUE && sensor->cali_active == FALSE){
			if(sensor->sample_number == (++last_sample_chest%1000)){
				if(sensor->moving_accel > cali_chest.one_g * 1.4){
					fall_detected = 3;
					fall_counter = 16;
				}
				if((sensor->moving_ang)>cali_chest.ang_accel*2 && (sensor->moving_ang) > 90){
					fall_detected = 4;
					fall_counter = 16;
				}
			}
			last_sample_chest = sensor->sample_number;
		}

		if(sensor_id == THIGH && cali_thigh.fill == TRUE && sensor->cali_active == FALSE){
			if(sensor->sample_number == (++last_sample_thigh%1000)){
				if(sensor->moving_accel > cali_thigh.one_g * 1.4){
					fall_detected = 2;
					fall_counter = 16;
				}
				if((sensor->moving_ang)>cali_thigh.ang_accel*2 && sensor->moving_ang > 90){
					fall_detected = 1;
					fall_counter = 16;
				}
			}

			last_sample_thigh =sensor->sample_number;
		}

	fall_counter--;
	if(cali_chest.standing_fill == TRUE && cali_thigh.standing_fill == TRUE){
		if(chest_info.xangle_comp  < cali_chest.x_standing_angle + 30 && chest_info.xangle_comp >  cali_chest.x_standing_angle - 30){
			if(chest_info.yangle_comp  < cali_chest.y_standing_angle + 30 && chest_info.yangle_comp >  cali_chest.y_standing_angle - 30){
				if(chest_info.zangle_comp  < cali_chest.z_standing_angle + 30 && chest_info.zangle_comp >  cali_chest.z_standing_angle - 30){
					if(thigh_info.xangle_comp  < cali_thigh.x_standing_angle + 30 && thigh_info.xangle_comp >  cali_thigh.x_standing_angle - 30){
						if(thigh_info.yangle_comp  < cali_thigh.y_standing_angle + 30 && thigh_info.yangle_comp >  cali_thigh.y_standing_angle - 30){
							if(thigh_info.zangle_comp  < cali_thigh.z_standing_angle + 30 && thigh_info.zangle_comp >  cali_thigh.z_standing_angle - 30){
								fall_detected = 0;
								fall_counter = 0;
							}
						}
					}
				}
			}
		}
	}else{
//		printf("Not Standing Fill");
	}

	if(fall_detected == 4 && fall_counter == 0){
		printf("FALL DETECTED ACCELERATION CHEST\n");
	}
	if(fall_detected == 3 && fall_counter == 0){
		printf("FALL DETECTED GYROSCOPE CHEST\n");
	}


	if(fall_detected == 2 && fall_counter == 0){
		printf("FALL DETECTED ACCELERATION THIGH\n");
	}
	if(fall_detected == 1 && fall_counter == 0){
		printf("FALL DETECTED GYROSCOPE THIGH\n");
	}
	//printf("Moving Aceel: %f versus Cali Moving Accel: %f \n", sensor->moving_accel, cali_chest.one_g);
	//printf("Moving Ang: %f verus Cali moving Moving Ang %f \n", sensor->moving_ang, cali_chest.ang_accel);
#ifdef VISUALIZATION
	if(sensor_id == CHEST){
		printf("c %f \%f %f \n", sensor->xangle_accel,sensor->yangle_accel,sensor->zangle_accel);

	}else if(sensor_id == THIGH){
		printf("t %f \%f %f \n", sensor->xangle_accel,sensor->yangle_accel,sensor->zangle_accel);
	}
#endif

	return 0;
}



#endif
