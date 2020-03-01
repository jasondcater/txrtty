#include "./common.h"

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

uint32_t running = 1;
uint32_t has_data = 0; // Lets the system know there is data ready for transmission.

// Commands
int8_t cmd[BUFSIZ];
const uint8_t quit[] = "quit";
const uint8_t help[] = "help";

// Data buffers
uint32_t *p_msg_buffer;
uint32_t msg_buffer[BUFSIZ];
uint32_t *p_tx_buffer;
uint32_t tx_buffer[BUFSIZ][8];
/*
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
*/

void *input_loop(void *thread_id){
  FILE *fd;
  fd = fopen((char *)"tests/bin/tests.log", "rb");

  if(fd == NULL){
    printf("Cannot open log file.");
    running = 0;
    pthread_exit(&errno);
  }

  while(running){

    /*
     * Loop backwards from the end of the file looking for newline chars that
     * demarcate the lines. We seek backwards until we have the second newline
     * char. This indicates that we have an index range for the last line. Also,
     * we check that we are still within the size of the file.
     */
    int64_t char_count = -1;
    int64_t newline_count = 0;

    fseek(fd, 0, SEEK_END);
    int64_t file_size = ftell(fd);

    // Seek the file discriptor to the end of the file.
    fseek(fd, char_count, SEEK_END);

    while(newline_count < 2 && abs(char_count) <= file_size){
      char c[] = {fgetc(fd)};

      if(strcmp("\n", c) == 0){
        newline_count++;
      }
      else{
        // Add the char to the message buffer.
        msg_buffer[abs(char_count)] = c[0];
      }

      char_count--;
      fseek(fd, char_count, SEEK_END);
    }

    // Add the message size to the message buffer.
    msg_buffer[0] = abs(char_count);
  }

  if(fd != NULL) fclose(fd);
  pthread_exit(0);
}

void transmit_blank(){
  int bit = 0;
  while(bit < 8){
    //write_wave(blank[bit]);
    bit++;
  }
}

void transmit_message(){
  int bit = 8;
  //int bit_count = *(tx_buffer) * 8; // Total bits to transmit.
  int bit_count = 10;
  while(bit < bit_count){
    //write_wave(*(tx_buffer + bit));
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
    exit(errno);
  }

  // Spawn the threads.
  if(
    pthread_create(&input_thread, NULL, input_loop, (void *)t) != 0 //||
//    pthread_create(&tx_thread, NULL, tx_loop, (void *)t) != 0
  ){
    perror("pthread_create() error");
    exit(errno);
  }

  // Wait for the threads to exit before finishing.

  if(
    pthread_join(input_thread, &status) != 0 //||
  //  pthread_join(tx_thread, &status) != 0
  ){
    perror("pthread_join() error");
    exit(errno);
  }

  exit(0);
}
