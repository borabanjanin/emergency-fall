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

#define CPORTNUMBER 22
#define BDRATE 9600
#define CHEST 0
#define THIGH 1

unsigned char buf[4096];
int buffersize = 0;

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
					printf("%i ",point->xaccel);
					printf("%i ",point->yaccel);
					printf("%i ",point->zaccel);
					printf("%i ",point->xrot);
					printf("%i ",point->yrot);
					printf("%i ",point->zrot);
				break;
		}

		return 0;
}


int ProcessInput(){
	int i;
	for(i=0; i < buffersize; i++){
		char inputbyte =  buf[i];
		if(inputbyte == 85){
			thigh_info.type_input = 1;
			if(thigh_info.sample_number < 999){
				thigh_info.sample_number++;
			}else{
				thigh_info.sample_number = 0;
			}
			i++;
			i = ParseInput(thigh_info, i);
		}
		inputbyte =  buf[i];
		if(inputbyte == 77){
			chest_info.type_input = 1;
			i++;
			if(chest_info.sample_number < 999){
				chest_info.sample_number++;
			}else{
				chest_info.sample_number = 0;
			}
			i++;
			i = ParseInput(chest_info, i);
		}
	}
	return 0;
}

int ParseInput(SensorInfo point_data, int input_index){
	signed int measurement;
	signed int sign_exten_mask = 0x00008000;
	while(input_index < buffersize){
		char inputbyte =  buf[input_index];
		if(point_data.type_input == 1 || point_data.type_input == 3 || point_data.type_input == 5
				|| point_data.type_input == 7 || point_data.type_input == 9 || point_data.type_input == 11){
			measurement = (inputbyte & 0x000000FF) << 8;
			point_data.type_input = point_data.type_input + 1;
		}else if(point_data.type_input == 2 || point_data.type_input == 4 || point_data.type_input ==  6 || point_data.type_input ==  8
				|| point_data.type_input ==  10 || point_data.type_input ==  12){
			measurement |= inputbyte & 0x000000FF;
			if(sign_exten_mask & measurement){
				measurement |= 0xFFFF0000;
			}

			data_point = &point_data.data_array[point_data.sample_number];
			StoreInput(data_point, measurement, point_data.type_input);
			point_data.type_input = point_data.type_input + 1;
		}
		if(point_data.type_input == 13){
			printf("\n");
			data_point = &point_data.data_array[point_data.sample_number];
			StoreInput(data_point, measurement, 13);
			ProcessData(data_point);
			GraphData(CHEST, data_point);
			point_data.type_input = 0;
			printf("\n");
			return input_index;
		}
		input_index++;
	}
	return input_index;
}

void my_handler(int s){
	gnuplot_close(plot_handle_chest);
	gnuplot_close(plot_handle_thigh);
//			printf("Closing logfile...\n");
//			fclose(logfile);
//			printf("Logfile closed\n");
    printf("exiting %d\n",s);
    exit(1);
}

int FakeData(){
	int size = 5, index = 0, i;
	for(i = 0; i < size; i++){
		buf[index++] = 'M';
		buf[index++] = 'G';
		buf[index++] = 255;
		buf[index++] = 248;
		buf[index++] = 255;
		buf[index++] = 247;
		buf[index++] = 255;
		buf[index++] = 246;
		buf[index++] = 255;
		buf[index++] = 245;
		buf[index++] = 255;
		buf[index++] = 245;
		buf[index++] = 255;
		buf[index++] = 244;
		buf[index++] = 'U';
		buf[index++] = 255;
		buf[index++] = 254;
		buf[index++] = 255;
		buf[index++] = 253;
		buf[index++] = 255;
		buf[index++] = 252;
		buf[index++] = 255;
		buf[index++] = 251;
		buf[index++] = 255;
		buf[index++] = 250;
		buf[index++] = 255;
		buf[index++] = 249;
	};
	return size * 27;
}

int main(){
	ConnectSerialPort();
	plot_handle_thigh = gnuplot_init();
	plot_handle_chest = gnuplot_init();
//	OpenFile();
//	printf("Opened log file\n");
	char userinput = '!';
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

