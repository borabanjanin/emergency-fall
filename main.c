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

#include "rs232.h"
#include "main.h"

#define CPORTNUMBER 22
#define BDRATE 9600
#define CHEST 0
#define THIGH 1

unsigned char buf[4096];
int buffersize = 0;
short packet_type;

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


int ConnectSerialPort(){

	if(RS232_OpenComport(CPORTNUMBER, BDRATE)){
		printf("Can not open comport\n");
		return 1;
	}
	printf("Connected serial port successfully\n");
	return 0;
}

int StoreInput(Sample* point, signed int measurement){
		switch(typeinput){
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
	signed int measurement;
	signed int sign_exten_mask = 0x00008000;
	for(i=0; i < buffersize; i++){
		char inputbyte =  buf[i];

		if(typeinput == 0 || typeinput == 13){
			if(inputbyte == 85){
				packet_type = THIGH;
				typeinput = 1;
				thigh_info.type_input = 1;
			}
			if(inputbyte == 77){
				packet_type = CHEST;
				typeinput = 1;
				chest_info.type_input = 1;
				i++;
			}

			if(packet_type == CHEST){
				if(chest_info.sample_number < 999){
					chest_info.sample_number++;
				}else{
					chest_info.sample_number = 0;
				}
			}else{
				if(thigh_info.sample_number < 999){
					thigh_info.sample_number++;
				}else{
					thigh_info.sample_number = 0;
				}
			}
		}else if(typeinput == 1 || typeinput == 3 || typeinput == 5 || typeinput == 7 || typeinput == 9 || typeinput == 11){
			measurement = (inputbyte & 0x000000FF) << 8;
			typeinput++;
		}else if(typeinput == 2 || typeinput == 4 || typeinput == 6 || typeinput == 8 || typeinput == 10 || typeinput == 12){
			measurement |= inputbyte & 0x000000FF;
			if(sign_exten_mask & measurement){
				measurement |= 0xFFFF0000;
			}
			if(CHEST == packet_type){
				chest = &chest_samples[samplenumber];
				StoreInput(chest, measurement);
			}
			if(THIGH == packet_type){
				thigh = &thigh_samples[samplenumber];
				StoreInput(thigh, measurement);
			}
			typeinput++;
		}
		if(typeinput == 13){
			printf("\n");
			if(CHEST == packet_type){
				chest = &chest_samples[samplenumber];
				StoreInput(chest, measurement);
				ProcessData(chest);
				GraphData(CHEST, chest);
			}
			if(THIGH == packet_type){
				thigh = &thigh_samples[samplenumber];
				StoreInput(thigh, measurement);
				ProcessData(thigh);
				GraphData(THIGH, thigh);
			}
			printf("\n");
		}

	}

	return 0;
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
		buffersize = RS232_PollComport(CPORTNUMBER, buf, 4095);
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

