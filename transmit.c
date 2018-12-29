#include <stdio.h>
#include <stdlib.h>
//#include <sys/timeb.h>
#include <time.h>
#include "common.h"

double default_msg[20] = {20,127,82,55,59,82,7,26,110,14,127,122,107,110,87,127,70,42,91,35}; //default message

int has_data = 0;//lets the system know there is data ready for transmission
int msg_length = 0;//trans_buf array size

//takes the buffer of byte encoded rtty numbers and then runs a clock to output the 
int parse(int *buf)
{	
	//count up baudot symbols to create a byte array
	int running = 1; //true = 1, false = 0

	while(running == 1)
	{	
		switch(buf[msg_length])
		{
			case 11:
				running = 0;
				break;
			case 35:
				running = 0;
				break;
			default:
				break;
		}

		++msg_length;
	}

	int bit = 0;
	int b = 0;

	for(b = 0 ; b < msg_length; ++b)
	{
		for(bit = 7; bit >= 0; --bit)
    		{
        		trans_buf[b][bit] = buf[b]%2;
        		buf[b]/=2;
    		}
	}	

	//tells the transmission loop that there is data to transmit
	has_data = 1;

	return 1;
}


//this will transmit null frames until the has_data flag is set.
//at the end of data transmission this will continue to transmit null frames to keep timing.
void transmit()
{
	
	double test_char[8] = {2295.0,2295.0,2295.0,2295.0,2295.0,2295.0,2295.0,2295.0};

	int freq = 0;
	int bit = 0;
	
	//sets the rate per second of the timer
	int baud = 45;
	int rate;
	rate = CLOCKS_PER_SEC/baud;

	//sub second timer
	clock_t time_mark;
	clock_t current;

	//primes the timer
	current = clock();
	time_mark = current + rate;

	//runs the timer in a loop, polls as fast as the system can run the process
	while(1)
	{
		switch(has_data)
		{
			
		}
		write_wave(test_char[bit%8]);
		++bit;
		/*
		//get the current time
		current = clock();
		
		//if(bit > 8) bit = 0;
		//freq = null_char[bit];

		//checks to see if the current marker is up to the point of our tic
		if(current >= time_mark)
		{
			//execute timed code
			time_mark = current + rate;
			write_wave(2125.0);
			//++bit;
		}
		*/
	}
}





