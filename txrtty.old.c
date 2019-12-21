#include "/usr/include/pthread.h"
#include "./common.h"
#include <stdio.h>
#include <string.h>

// 0 == false, 1 == true
int running = 0;
int has_data = 0;// Lets the system know there is data ready for transmission.

// Commands
char cmd[BUFSIZ];
const char quit[] = "quit";
const char help[] = "help";

// Various
char  *p;   // Newline pointer;
int *msg_buf;   // Message buffer, gets filled by baudot();
double *trs_buf; // Binary message buf, converted to a multidimentional array.

// Enumerated message buffer.
int msg_buffer[BUFSIZ];

// Binary message buffer
double trans_buffer[BUFSIZ][8];

double blank[8] = {2295.0,2295.0,2295.0,2295.0,2295.0,2295.0,2295.0,2295.0};

void *inputLoop(void *threadid){
  while(running){
    fgets(cmd, sizeof(cmd), stdin);

    // Remove newline/carrige return from input.
    if((p = strchr(cmd, '\n')) != NULL){
      *p = '\0';
    }

    // Quit application.
    if(strcmp(cmd, quit) == 0){

      // Need to shutdown audio device.
    	running = 0;
    }

    // Print help.
    else if(strcmp(cmd, help) == 0){
      printf("help yo!\n");
    }

    // Transmit message.
    else{

      // Convert letters to baudot code.
      baudot(cmd, msg_buf);

      // Convert baudot code to binary, frequency mark and space.
      parse(msg_buf, trs_buf);

      has_data = 1;
    }
  }

  pthread_exit(NULL);
}

void transmitBlank(){
  int bit = 0;
  while(bit < 8){
    write_wave(blank[bit]);
    ++bit;
  }
}

void transmitMessage(){
  int bit = 8;
  int bit_count = *(trs_buf) * 8; // Total bits, letters times 8 (bytes)
  while(bit < bit_count){
    write_wave(*(trs_buf + bit));
    ++bit;
  }

  has_data = 0; // Finishes the message transimission.
}

void *transmitLoop(void *thread_id){
  while(running){
    switch(has_data){
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

int main(int argc, char *argv[]){

  // Get the handles for the message buffers.
  msg_buf = msg_buffer;
  trs_buf = &trans_buffer[0][0];

  // Open audio channel.
  if(open_audio() == 1){
    running = 1;
    long t = 0;

    // User input thread.
    int il_handle;
    pthread_t il;

    // Transimission processing thread.
    int tl_handle;
    pthread_t tl;

    il_handle = pthread_create(&il, NULL, inputLoop, (void *)t);
    tl_handle = pthread_create(&tl, NULL, transmitLoop, (void *)t);

    while(1){
      if(running){
      	//printf("%s\n", "running just fine");
      	//return 0;
      }
      else{
        printf("quitting...\n");
        return 0;
      }
    }
  }
  else{
    running = 0;
    return 0;
  }

  return 0;
}
