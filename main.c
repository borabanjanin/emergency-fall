/***********************************************
 ***********************************************
 *Emergency Fall Detection Source Code
 *
 *by Bora Banjanin, Chia-Ning Wang, and Lingrui Zhang
 *
 *for EE 449 Controls Capstone
 *
 *Proprietary Information DO NOT USE :)
 ***********************************************
 ***********************************************/

#define DEBUG

#include "rs232.h"
#include "main.h"
#include "processdata.c"
#include <python2.7/Python.h>
#include <stdio.h>

#define CPORTNUMBER 22
#define BDRATE 9600
#define CHEST 0
#define THIGH 1




FILE *logfile;

FILE* OpenFile(){

	struct stat st = {0};
	time_t now = time(NULL);
	struct tm* time = localtime(&now);

	if (stat("logs", &st) == -1) {
	    mkdir("logs", 0700);
	}

	char logfilename[40] = "logs/";
	sprintf(logfilename, "logs/day-%d,hour-%d,min-%d.txt", time->tm_mday, time->tm_hour, time->tm_min);
	logfile = fopen(logfilename, "w");
	return logfile;
}

#ifndef DEBUG
int ConnectSerialPort(){

	if(RS232_OpenComport(CPORTNUMBER, BDRATE)){
		printf("Can not open comport\n");
		return 1;
	}
	printf("Connected serial port successfully\n");
	return 0;
}
#else
	int ConnectSerialPort(){
		return 0;
	}
#endif

int StoreInput(Sample* point, signed int measurement, short type_input){
		switch(type_input){
			case 0:
				point->n = measurement;
				break;
			case 1:
				break;
			case 2:	
				point->xaccel = measurement;
				break;
			case 3:
				break;
			case 4:
				point->yaccel = measurement;
				break;
			case 5:
				break;
			case 6:
				point->zaccel = measurement;
				break;
			case 7:
				break;
			case 8:
				point->xrot = measurement;
				break;
			case 9:
				break;
			case 10:
				point->yrot = measurement;
				break;
			case 11:
				break;
			case 12:
				point->zrot = measurement;
				break;
			case 13:
				/*
					fprintf(logfile,"%i ",point->xaccel);
					fprintf(logfile,"%i ",point->yaccel);
					fprintf(logfile,"%i ",point->zaccel);
					fprintf(logfile,"%i ",point->xrot);
					fprintf(logfile,"%i ",point->yrot);
					fprintf(logfile,"%i \n",point->zrot);
				*/
				break;
		}

		return 0;
}

#ifndef DEBUG
int callNMA(){
     printf("enter\n");
     PyObject *myPyrowl, *myPprint, *myOS, *myTime, *myRes, *arglist, *arglist2, *arglist3, *myP = NULL;
     printf("Starting\n");
     Py_Initialize();
     PyRun_SimpleString("import sys");
     PyRun_SimpleString("sys.path.append(\".\")");
     myPyrowl = PyImport_ImportModule("pynma");
     myPprint = PyImport_ImportModule("pprint");
     myOS = PyImport_ImportModule("os");
     myTime = PyImport_ImportModule("time");
     arglist = Py_BuildValue("(s)","b95a1e3705fed5edcb00652f405a768dfef02001c687c1eb");
     myP = PyEval_CallObject(PyObject_GetAttrString(myPyrowl, "PyNMA"), arglist);
     myRes = PyObject_GetAttrString(myP, "push");
     arglist2 = Py_BuildValue("(s, s, s, s)","My Raspberry Pi", "Fall Warning","Patient 1 Has Fallen", "batch_mode=False");
     PyEval_CallObject(myRes, arglist2);
     //arglist3 = Py_BuildValue("(i)", 60);
     //PyEval_CallObject(PyObject_GetAttrString(myTime, "sleep"), arglist3);
     Py_Finalize();
     printf("exit\n");
     return 0;
}
#endif


int ParseInput(short passed, SensorInfo* point_data, int input_index){
	signed int measurement;
	signed int sign_exten_mask = 0x00008000;
	while(input_index < buffersize){
		char inputbyte =  buf[input_index];
		if(point_data->type_input == 1 || point_data->type_input == 3 || point_data->type_input == 5
				|| point_data->type_input == 7 || point_data->type_input == 9 || point_data->type_input == 11){
			measurement = (inputbyte & 0x000000FF) << 8;
			point_data->type_input = point_data->type_input + 1;
		}else if(point_data->type_input == 2 || point_data->type_input == 4 || point_data->type_input ==  6 || point_data->type_input ==  8
				|| point_data->type_input ==  10 || point_data->type_input ==  12){
			measurement |= inputbyte & 0x000000FF;
			if(sign_exten_mask & measurement){
				measurement |= 0xFFFF0000;
			}

			data_point = &point_data->data_array[point_data->sample_number];
			StoreInput(data_point, measurement, point_data->type_input);
			point_data->type_input = point_data->type_input + 1;
		}
		if(point_data->type_input == 13){

			StoreInput(data_point, measurement, 13);
			ProcessData(data_point);
			AccelAngle(point_data, data_point);
			ComplementaryFilter(point_data, data_point);
			GraphData(passed, point_data, data_point);
			if(point_data->cali_active){
				CalibrationRoutine(data_point);
			}
			point_data->type_input = 0;
			return input_index;
		}
		input_index++;
	}
	return input_index;
}

int ProcessInput(){
	int i;
	for(i=0; i < buffersize; i++){
		char inputbyte =  buf[i];
		if(inputbyte == 85){
			thigh_info.type_input = 1;
			i++;
			i = ParseInput(THIGH, &thigh_info, i);
			if(thigh_info.sample_number < 999){
				thigh_info.sample_number++;
			}else{
				thigh_info.sample_number = 0;
			}
		}
		inputbyte =  buf[i];
		if(inputbyte == 77){
			chest_info.type_input = 1;
			i++;
			inputbyte =  buf[i];
			if('B' == inputbyte){
				chest_info.cali_active = 1;
			}else{
				chest_info.cali_active = 0;
			}
			i++;
			i = ParseInput(CHEST, &chest_info, i);
			if(chest_info.sample_number < 999){
				chest_info.sample_number++;
			}else{
				chest_info.sample_number = 0;
			}
		}
	}
	return 0;
}

void my_handler(int s){
	gnuplot_close(plot_handle_chest);
	gnuplot_close(plot_handle_thigh);
	gnuplot_close(plot_handle_chest_angle);
	gnuplot_close(plot_handle_thigh_angle);
			printf("Closing logfile...\n");
			fclose(logfile);
			printf("Logfile closed\n");
    printf("exiting %d\n",s);
    exit(1);
}

int FakeData(){
	int size = 5, index = 0, i;
	for(i = 0; i < size; i++){
		buf[index++] = 'M';
		buf[index++] = 'B';
		buf[index++] = 36;
		buf[index++] = 21;
		buf[index++] = 36;
		buf[index++] = 21;
		buf[index++] = 36;
		buf[index++] = 21;
		buf[index++] = 0;
		buf[index++] = 255;
		buf[index++] = 0;
		buf[index++] = 255;
		buf[index++] = 0;
		buf[index++] = 255;
		buf[index++] = 'U';
		buf[index++] = 36;
		buf[index++] = 21;
		buf[index++] = 36;
		buf[index++] = 21;
		buf[index++] = 36;
		buf[index++] = 21;
		buf[index++] = 0;
		buf[index++] = 255;
		buf[index++] = 0;
		buf[index++] = 255;
		buf[index++] = 0;
		buf[index++] = 255;
	};
	return size * 27;
}

int Initialize(){
	chest_info.dt = 0.0146;
	thigh_info.dt = 0.0135;
	cali_chest.fill = 0;
	int buffersize = 0;
	plot_handle_thigh = gnuplot_init();
	plot_handle_chest = gnuplot_init();
	plot_handle_thigh_angle = gnuplot_init();
	plot_handle_chest_angle = gnuplot_init();
	char userinput = '!';
	chest_info.cali_active = FALSE;
	chest_info.type_input = 0;
	return 0;
}

int main(){
	ConnectSerialPort();
	Initialize();

	OpenFile();
	printf("Opened log file\n");
	struct sigaction SIGINTHANDLER;
	SIGINTHANDLER.sa_handler = my_handler;
	sigemptyset(&SIGINTHANDLER.sa_mask);
	SIGINTHANDLER.sa_flags = 0;
	sigaction(SIGINT, &SIGINTHANDLER, NULL);

	while(1){

		printf("Polling comport...\n");
#ifndef DEBUG
		buffersize = RS232_PollComport(CPORTNUMBER, buf, 4095);
#else
		buffersize = FakeData();
#endif
		if(buffersize > 0){
			buf[buffersize] = 0;
			ProcessInput();
			//RS232_SendBuf(cportnumber, buf, 4095);
			//printf("received %i bytes: %s\n", buffersize, (char *)buf);
		}
		usleep(100000);  /* sleep for 100 milliSeconds */
	}

	return 0 ;
}

