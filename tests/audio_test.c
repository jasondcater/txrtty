#include "../common.h"

#include <stdlib.h>
#include <stdio.h>

void test_open_audio(){
  open_audio();
}

void test_close_audio(){
  close_audio();
}

void test_generate_sine(){
  generate_sine(2125.0);
}

int main(int argc, char *argv[]){
  test_open_audio();
  test_close_audio();
  test_generate_sine();
  exit(0);
}
