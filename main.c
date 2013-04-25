
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

int StoreInput(signed int measurement){
		switch(typeinput){
			case 0:
				Samples[samplenumber].n = measurement;
				break;
			case 1:
				break;
			case 2:	
				Samples[samplenumber].xaccel = measurement;
				break;
			case 3:
				break;
			case 4:
				Samples[samplenumber].yaccel = measurement;
				break;
			case 5:
				break;
			case 6:
				Samples[samplenumber].zaccel = measurement;
				break;
			case 7:
				break;
			case 8:
				Samples[samplenumber].xrot = measurement;
				break;
			case 9:
				break;
			case 10:
				Samples[samplenumber].yrot = measurement;
				break;
			case 11:
				break;
			case 12:
				Samples[samplenumber].zrot = measurement;
				break;
			case 13:
				if(samplenumber>1){
					printf("%i",Samples[samplenumber].xaccel - Samples[samplenumber-1].xaccel);
					printf("%i",Samples[samplenumber].yaccel - Samples[samplenumber-1].yaccel);
					printf("%i",Samples[samplenumber].zaccel - Samples[samplenumber-1].zaccel);
					printf("%i",Samples[samplenumber].xrot - Samples[samplenumber-1].xrot);
					printf("%i",Samples[samplenumber].yrot - Samples[samplenumber-1].yrot);					
					printf("%i",Samples[samplenumber].zrot - Samples[samplenumber-1].zrot);
				}
				break;
		}
}


int ProcessInput(){
	int i;
	signed int measurement;
	signed int sign_exten_mask = 0x00008000;
	for(i=0; i < buffersize; i++){
		char inputbyte =  buf[i];
		if(typeinput == 0 || typeinput == 13){
			if(inputbyte == 85){
				printf("%i ", inputbyte);
				typeinput = 1;
			}
		}else if(typeinput == 1 || typeinput == 3 || typeinput == 5 || typeinput == 7 || typeinput == 9 || typeinput == 11){
			measurement = (inputbyte & 0x000000FF) << 8;
			typeinput++;
		}else if(typeinput == 2 || typeinput == 4 || typeinput == 6 || typeinput == 8 || typeinput == 10 || typeinput == 12){
			measurement |= inputbyte & 0x000000FF;
			if(sign_exten_mask & measurement){
				measurement |= 0xFFFF0000;
			}
			StoreInput(measurement);
			printf("%i ",measurement);
			typeinput++;
		}		
		if(typeinput == 13){
			StoreInput(measurement);
			printf("\n");
		}
	
				
		if(samplenumber < 999){
				samplenumber++;
		}else{
				samplenumber = 0;
		}

	}

	return 0;
}


int main(){
	ConnectSerialPort();
//	OpenFile();
//	printf("Opened log file\n");
	char userinput = '!';

	while(1){
	//	if(){
//			scanf("%c",&userinput);
	//	}

//		if(userinput == 'x' || userinput == 'c'){
//			printf("Closing logfile...\n");
//			fclose(logfile);
//			printf("Logfile closed\n");
//			break;
//		}
//		printf("Polling comport...\n");
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

