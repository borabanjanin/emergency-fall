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

typedef struct Sample{
	unsigned short n;
	int xaccel;
	int yaccel;
	int zaccel;
	int xrot;
	int yrot;
	int zrot;

}Sample;

unsigned char buf[4096];
int cportnumber=22;
int bdrate=9600;
int buffersize = 0;
short samplenumber = 0;
short typeinput = 0;
Sample Samples[1000];
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

	if(RS232_OpenComport(cportnumber, bdrate)){
		printf("Can not open comport\n");
		return 1;
	}
	printf("Connected serial port successfully\n");
	return 0;
}

int ProcessInput(){
	int i;

	for(i=0; i < buffersize; i++){
		signed short inputbyte =  buf[i];
		signed int measurement;
//		fprintf(logfile, "Raw data: %x\n", inputbyte);
		printf("%d\n", inputbyte);
		switch(typeinput){
			case 0:
				if(inputbyte == 85){
					typeinput++;
					fprintf(logfile, "Sample Number: %d\n ",samplenumber);
				}
				break;

			case 1:
				measurement = inputbyte << 8;
				typeinput++;
				break;

			case 2:
				measurement |= inputbyte;
				Samples[samplenumber].xaccel = measurement;
				fprintf(logfile, "%x ",Samples[samplenumber].xaccel);
				typeinput++;
				break;

			case 3:
				measurement = inputbyte << 8;
				typeinput++;
				break;

			case 4:
				measurement |= inputbyte;
				Samples[samplenumber].yaccel = measurement;
				fprintf(logfile, "%x ",Samples[samplenumber].yaccel);
				typeinput++;
				break;

			case 5:
				measurement = inputbyte << 8;
				typeinput++;
				break;

			case 6:
				measurement |= inputbyte;
				Samples[samplenumber].zaccel = measurement;
				fprintf(logfile, "%x ",Samples[samplenumber].zaccel);
				typeinput++;
				break;

			case 7:
				measurement = inputbyte << 8;
				typeinput++;
				break;

			case 8:
				measurement |= inputbyte;
				Samples[samplenumber].xrot = measurement;
				fprintf(logfile, "%x ",Samples[samplenumber].xrot);
				typeinput++;
				break;

			case 9:
				measurement = inputbyte << 8;
				typeinput++;
				break;

			case 10:
				measurement |= inputbyte;
				Samples[samplenumber].yrot = measurement;
				fprintf(logfile, "%x ",Samples[samplenumber].yrot);
				typeinput++;
				break;

			case 11:
				measurement = inputbyte << 8;
				typeinput++;
				break;

			case 12:
				measurement |= inputbyte;
				Samples[samplenumber].zrot = measurement;
				fprintf(logfile, "%x \n",Samples[samplenumber].zrot);
				typeinput = 0;
				if(samplenumber < 999){
					samplenumber++;
				}else{
					samplenumber = 0;
				}
				break;
		}
	}

	return 0;
}

int main(){
	ConnectSerialPort();
	OpenFile();
	printf("Opened log file\n");
	char userinput = '!';

	while(1){
	//	if(){
			scanf("%c",&userinput);
	//	}

		if(userinput == 'x' || userinput == 'c'){
			printf("Closing logfile...\n");
			fclose(logfile);
			printf("Logfile closed\n");
			break;
		}
		printf("Polling comport...\n");
		buffersize = RS232_PollComport(cportnumber, buf, 4095);
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

