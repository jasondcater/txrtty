#include "../common.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void test_open_audio(){
  uint32_t status = open_audio();
  if(status) printf("test_audio_open() failed. Wrong status code.\n");
}

void test_close_audio(){
  close_audio();
}

void test_generate_sine(){
  uint32_t status = generate_sine(2125.0);
  if(status) printf("test_generate_sine() failed. Wrong status code.\n");

  write_audio_file();
}

int main(int argc, char *argv[]){
  //test_open_audio();
  //test_close_audio();
  test_generate_sine();
  exit(0);
}
