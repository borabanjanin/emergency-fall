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
Sample Samples[1000];
FILE *logfile;

FILE* OpenFile(){

	struct stat st = {0};
	time_t now = time(NULL);
	struct tm* time = localtime(&now);

	if (stat("logs", &st) == -1) {
	    mkdir("logs", 0700);
	}

	char testname[30] = "logs/";
	char currenttime[20];
	sprintf(currenttime, "day-%d hour-%d min-%d.txt", time->tm_mday, time->tm_hour, time->tm_min);
	strcat(testname, currenttime);
	logfile = fopen(testname, "w");
	return logfile;
}


int ConnectSerialPort(){

	if(RS232_OpenComport(cportnumber, bdrate)){
		printf("Can not open comport\n");
		return 0;
	}

	return 1;
}

int ProcessInput(){
	int i;

	for(i=0; i < buffersize; i++){
		unsigned short temp = (unsigned short) buf[i];
		fprintf(logfile, "%d", temp);
	}

	return 0;
}

int main(){
//	ConnectSerialPort();
	OpenFile();
	char userinput = '!';

	while(1){
		scanf("%c",&userinput);
		if(userinput == 'x' || userinput == 'c'){
			printf("Closing logfile...\n");
			fclose(logfile);
			printf("Logfile closed\n");
			break;
		}
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

