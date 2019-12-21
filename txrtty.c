#include "./common.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int running = 1;
int has_data = 0; // Lets the system know there is data ready for transmission.

// Commands
char cmd[BUFSIZ];
const char quit[] = "quit";
const char help[] = "help";

// Data buffers
int *p_msg_buffer;
int msg_buffer[BUFSIZ];
int *p_tx_buffer;
int tx_buffer[BUFSIZ][8];

void *input_loop(void *thread_id){
  while(running){

    fgets(cmd, sizeof(cmd), stdin);

    // Replace newline/carrige return from input.
    char *nl;
    if((nl = strchr(cmd, '\n')) != NULL){
      *nl = '\0';
    }

    // Quit application.
    if(strcmp(cmd, quit) == 0){
      close_audio();
      running = 0;
    }

    msg_to_baudot(cmd, p_msg_buffer); // Convert the message chars to baudot.
    baudot_to_fsk(p_msg_buffer, p_tx_buffer); // Convert baudot msg to FSK vals. 
    has_data = 1;
  }

  pthread_exit(0);
}

void transmit_blank(){
  int bit = 0;
  while(bit < 8){
    write_wave(blank[bit]);
    bit++;
  }
}

void transmit_message(){
  int bit = 8;
  int bit_count = *(tx_buffer) * 8; // Total bits to transmit.
  while(bit < bit_count){
    write_wave(*(tx_buffer + bit));
    bit++;
  }
}

void *tx_loop(void *thread_id){
  while(running){
    printf("tx_loop\n");
    system("sleep 1");

    if(has_data > 0){
      transmit_message();
    }
    else{
      transmit_blank();
    }
  }

  pthread_exit(0);
}

int main(int argc, char *argv[]){

  long t = 0;
  void *status;

  p_msg_buffer = &msg_buffer[0];
  p_tx_buffer = &tx_buffer[0][0];

  pthread_t input_thread;
  pthread_t tx_thread;

  if(open_audio() != 0){
    perror("open_audio() error");
    exit(1);
  }

  // Spawn the threads.
  if(
    pthread_create(&input_thread, NULL, input_loop, (void *)t) != 0 ||
    pthread_create(&tx_thread, NULL, tx_loop, (void *)t) != 0
  ){
    perror("pthread_create() error");
    exit(1);
  }

  // Wait for the threads to exit before finishing.
  if(
    pthread_join(input_thread, &status) != 0 ||
    pthread_join(tx_thread, &status) != 0
  ){
    perror("pthread_join() error");
    exit(3);
  }

  exit(0);
}
