#ifndef PROCESSDATA
#define PROCESSDATA
#include <math.h>

int ProcessData(){
	double xaccel_squar = (double) Samples[samplenumber].xaccel;
	double yaccel_squar = (double) Samples[samplenumber].yaccel;
	double zaccel_squar = (double) Samples[samplenumber].zaccel;
	double accel;

	xaccel_squar = pow(xaccel_squar,2.0);
	yaccel_squar = pow(yaccel_squar,2.0);
	zaccel_squar = pow(zaccel_squar,2.0);
	accel_array[samplenumber] = sqrt(xaccel_squar + yaccel_squar + zaccel_squar);
	fprintf(logfile,"Acceleration: %f\n",accel_array[samplenumber]);
	return 0;
}










#endif
