#include <stdio.h>
#include <string.h>
#include "common.h"

//Baudot to RTTY Bytes Table...see rtty_table for an explination on these arrays
int  shift_to_letters = 127;
char letters[30] = {'\0',' ','Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M','\r','\n'};
int  letcode[30] = {2,19,118,103,67,42,7,87,115,50,14,55,98,82,74,91,47,22,107,122,38,70,94,59,62,78,26,31,11,35};

int  shift_to_figures = 110;
char figures[30] = {'\0',' ','1','2','3','4','5','6','7','8','9','0','-','\a','$','!','&','#','\'','(',')','\"','/',':',';','\?',',','.','\r','\n'};
int  figcode[30] = {2,19,94,79,7,42,67,87,31,26,98,91,14,22,38,55,107,82,47,62,74,70,118,59,122,103,50,115,35,11};

int mark = 2295;
int space= 2125;

// SPCSTN-KJ6ZRF, SPACE STATION - KJ6ZRF
double default_msg[20] = {20,127,82,55,59,82,7,26,110,14,127,122,107,110,87,127,70,42,91,35};

// Convert message from letters to a decimal code
void baudot(char msg[], int *msg_buf){
  int shift = -1;//letters = 0, figures = 1;
  char alphanumeric;

  int a = 0; // Message loop counter;
  int b = 0; // Baudot loop counter;
  int c = 1; // Buffer counter, don't overwrite the first index of the buffer, it holds the array length;

  for(a = 0; a < strlen(msg); ++a){

		alphanumeric = toupper(msg[a]);

		for(b = 0; b < sizeof(letters); ++b)
		{
			if(alphanumeric == letters[b])
			{
				if(shift != 0)
				{
					shift = 0;

					//push shift to letters code
					msg_buf[c] = shift_to_letters;
					++c;
				}

				//push letter code
				msg_buf[c] = letcode[b];
				++c;
			}
		}

		for(b = 0; b < sizeof(figures); ++b)
		{
			if(alphanumeric == figures[b])
			{
				if(shift != 1)
				{
					shift = 1;

					//push shift to figures code
					msg_buf[c] = shift_to_figures;
					++c;
				}

				//push figure code
				msg_buf[c] = figcode[b];
				++c;
			}
		}
	}

	//write newline to buffer
	switch(shift)
	{
		case 0:
			msg_buf[c] = letcode[29];
			++c;
			break;
		case 1:
			msg_buf[c] = figcode[29];
			++c;
			break;
		default:
			break;
	}

	msg_buf[0] = c;//set the array size in the first index of the array
}

//convert decimal message to binary message, next stop audio transmission
void parse(int *msg_buf, double *trs_buf)
{
	*(trs_buf) = msg_buf[0]; //set the message size at the head of the array (letter count) at trans_buffer[0][0]

	int a = 0;
	int bit = 0;
	int decimal = msg_buf[1]; //set the first decimal code

	for(a = 1; a < msg_buf[0]; ++a)// a == 1 to leave room at the beginning of the array for a letter count (array size)
	{
		decimal = msg_buf[a];

		*(trs_buf + (a * 8)) = space; //set the initial start bit

		for(bit = 7; bit > 0; --bit)
    		{
			int count = (a * 8) + bit;

			//converting to binary == get modulo of 2 from decimal then divide decimal and repeat (7 - 0 times for a byte)

			if(decimal%2)//if we have a 1 set a mark, if we have 0 set a space
			{
				*(trs_buf + count) = mark;
			}
			else
			{
				*(trs_buf + count) = space;
			}

			//*(trs_buf + count) = decimal%2; //uncomment for binary output

        		decimal /= 2;
		}
	}
}
