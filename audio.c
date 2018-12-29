#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "/usr/lib/oss/include/sys/soundcard.h"
#include "common.h"

#define AUDIO_DEV "/dev/dsp"

int audio_fd;//audio device
double sample_rate = 48000.0;//48000 samples per second
short buf[1066];//sound wave buffer, its where we keep the rendered audio until we write it.
int outsz = 1066;//half buffer audio buffer size to prevent overflow, 1066 samples per bit at 45 samples a second

//objects to create the sine waveform
//double pi = 3.14159265359;

//double hertz = 330;
double amplitude = .8;
double time = 0.0;
double point = 0.0;//the current sample in double floating point form.
//double phase = 0;
double sample = 0;//the current sample being writen.
double bit_range = pow(2,16)/2; //16 bits divided by 2, 32768 to -32768

int open_audio()
{	
	int tmp;

	if ((audio_fd = open (AUDIO_DEV, O_WRONLY, 0)) == -1)
    	{
      		perror (AUDIO_DEV);
      		exit (-1);
    	}
	
	//set the bit rate 
	tmp = AFMT_S16_NE; // Native 16 bits 	
	if(ioctl(audio_fd, SNDCTL_DSP_SETFMT, &tmp) == -1)
    	{
      		//return -1;
    	}

	if (tmp != AFMT_S16_NE)
    	{
		printf("%s\n", "The device doesn't support the 16 bit sample format, quitting...\n");
      		return -1;
    	}

	//set the channels
	tmp = 1;
	if (ioctl (audio_fd, SNDCTL_DSP_CHANNELS, &tmp) == -1)
    	{
      		//return -1;
    	}

	if (tmp != 1)
    	{
      		printf("%s\n", "The audio device doesn't support mono mode, quitting...\n");
      		return -1;
    	}

	return 1;
}

void write_wave(double hertz)
{
	double cycle = (360.0 * hertz)/(180.0/3.14159265359);
	int a;
	
	for(a = 0; a < outsz; ++a)	
	{
		point = amplitude * sin(cycle * time);//sine wave function, y(t) = A * sin(wt + phase) time as a fraction of a second
		time = time + (1.0/sample_rate);
		sample = point * bit_range;//set the sample relative to the bit range 
		buf[a] = (int)sample;//load the buffer
	}
	
	//write the buffer to the audio device.
	if (write (audio_fd, buf, sizeof (buf)) != sizeof (buf))
    	{
      		perror ("Audio write");
      		exit (-1);
    	}
	
}



