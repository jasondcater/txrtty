#include "/usr/include/pthread.h"
#include "./common.h"
#include <stdio.h>
#include <string.h>

//0 == false, 1 == true
int running = 0;
int has_data = 0;//lets the system know there is data ready for transmission

//commands
char cmd[BUFSIZ];
const char quit[] = "quit";
const char help[] = "help";

//various
char  *p;   //newline pointer;
int *msg_buf;   //message buffer, gets filled by baudot();
double *trs_buf; //binary message buffer, converted to a multidimentional array

//enumerated message buffer
int msg_buffer[BUFSIZ];

//binary message buffer
double trans_buffer[BUFSIZ][8];

double blank[8] = {2295.0,2295.0,2295.0,2295.0,2295.0,2295.0,2295.0,2295.0};

void *inputLoop(void *threadid)
{
	while(running)
	{
		fgets(cmd, sizeof(cmd), stdin);
		
		//remove newline/carrige return from input
		if((p = strchr(cmd, '\n')) != NULL)
		{
			*p = '\0';
		}

		//quit application
		if(strcmp(cmd, quit) == 0)
		{
			//need to shutdown audio device
			running = 0;
		}
		//print help
		else if(strcmp(cmd, help) == 0)
		{
			printf("help yo!\n");
		}
		//transmit message
		else
		{
			baudot(cmd, msg_buf); //convert letters to baudot code
			parse(msg_buf, trs_buf); //convert baudot code to binary, frequency mark and space
			has_data = 1;
/*
			printf("\n");
			//printf("%d", *(trs_buf+1));
	
			int a = 0;
			int b = 0;
			for(a = 0; a < msg_buf[0]; ++a)
			{
				for(b = 0; b < 8 ;++b)
				{
					int count = (a * 8) + b;
					printf("%d, ", *(trs_buf + count));
				}
				printf("\n");
			}
*/
			
		}
	}

	pthread_exit(NULL);
}

void transmitBlank()
{
	int bit = 0;
	while(bit < 8)
	{
		write_wave(blank[bit]);
		++bit;	
	}
}

void transmitMessage()
{
	int bit = 8;
	int bitCount = *(trs_buf) * 8;//total bits, letters times 8 (bytes)
	while(bit < bitCount)
	{
		write_wave(*(trs_buf + bit));
		++bit;
	}

	has_data = 0;//finishes the message transimission
}

void *transmitLoop(void *threadid)
{
	while(running)
	{
		switch(has_data)
		{
			case 0:
				transmitBlank();
				break;
			case 1:
				transmitMessage();
				break;
			default: 
				transmitBlank();
				break;
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	//set the handles for the message buffers
	msg_buf = msg_buffer;
	trs_buf = &trans_buffer[0][0];

	//open audio channel
	if(open_audio() == 1)
	{
		running = 1;

		long t = 0;
		
		//user input thread
		int il_handle;
		pthread_t il;

		//transimission processing thread
		int tl_handle;
		pthread_t tl;
		
		il_handle = pthread_create(&il, NULL, inputLoop, (void *)t);
		tl_handle = pthread_create(&tl, NULL, transmitLoop, (void *)t);

		while(1)
		{
			if(running)
			{
				//printf("%s\n", "running just fine");
				//return 0;
			}
			else
			{
				printf("quitting...\n");
				return 0;
			}
		}
		
	
	}
	else
	{
		running = 0;
		return 0;
	}
	
	return 0;
}
