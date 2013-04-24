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

unsigned char buf[4096];
int cportnumber=22;
int bdrate=9600;
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
	signed int measurement;
	signed int sign_exten_mask = 0x00008000;
	for(i=0; i < buffersize; i++){
		char inputbyte =  buf[i];
		
		switch(typeinput){
			case 0:
				if(inputbyte == 85){
					typeinput++;
					fprintf(logfile, "%d ",samplenumber);
					printf("%d ",samplenumber);
					Samples[samplenumber].n = samplenumber;
				}
				break;

			case 1:
				measurement = inputbyte & 0x000000FF;
				typeinput++;
				break;

			case 2:
				measurement |= (inputbyte & 0x000000FF) << 8;
				if(sign_exten_mask & measurement){
					measurement |= 0xFFFF0000;
				}
				Samples[samplenumber].xaccel = measurement;
				fprintf(logfile,"%d ",Samples[samplenumber].xaccel);
				printf("%d ",Samples[samplenumber].xaccel);
				typeinput++;
				break;

			case 3:
				measurement = inputbyte & 0x000000FF;
				typeinput++;
				break;

			case 4:
				measurement |= (inputbyte & 0x000000FF) << 8;
				if(sign_exten_mask & measurement){
					measurement |= 0xFFFF0000;
				}
				Samples[samplenumber].yaccel = measurement;
				fprintf(logfile,"%d ",Samples[samplenumber].yaccel);
				printf("%d ",Samples[samplenumber].yaccel);
				typeinput++;
				break;

			case 5:
				measurement = inputbyte & 0x000000FF;
				typeinput++;
				break;

			case 6:
				measurement |= (inputbyte & 0x000000FF) << 8;
				if(sign_exten_mask & measurement){
					measurement |= 0xFFFF0000;
				}
				Samples[samplenumber].zaccel = measurement;
				fprintf(logfile,"%d ",Samples[samplenumber].zaccel);
				printf("%d ",Samples[samplenumber].zaccel);
				typeinput++;
				break;

			case 7:
				measurement = inputbyte & 0x000000FF;
				typeinput++;
				break;

			case 8:
				measurement |= (inputbyte & 0x000000FF) << 8;
				if(sign_exten_mask & measurement){
					measurement |= 0xFFFF0000;
				}
				Samples[samplenumber].xrot = measurement;
				fprintf(logfile,"%d ",Samples[samplenumber].xrot);
				printf("%d ",Samples[samplenumber].xrot);
				typeinput++;
				break;

			case 9:
				measurement = inputbyte & 0x000000FF;
				typeinput++;
				break;

			case 10:
				measurement |= (inputbyte & 0x000000FF) << 8;
				if(sign_exten_mask & measurement){
					measurement |= 0xFFFF0000;
				}
				Samples[samplenumber].yrot = measurement;
				fprintf(logfile,"%d ",Samples[samplenumber].yrot);
				printf("%d ",Samples[samplenumber].yrot);
				typeinput++;
				break;

			case 11:
				measurement = inputbyte & 0x000000FF;
				typeinput++;
				break;

			case 12:
				measurement |= (inputbyte & 0x000000FF) << 8;
				if(sign_exten_mask & measurement){
					measurement |= 0xFFFF0000;
				}
				Samples[samplenumber].zrot = measurement;
				fprintf(logfile,"%d ",Samples[samplenumber].zrot);
				printf("%d ",Samples[samplenumber].zrot);
				typeinput = 0;
				ProcessData();
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

