#include "../common.h"

#include <stdlib.h>
#include <stdio.h>

void test_open_audio(){
  open_audio();
}

void test_close_audio(){
  close_audio();
}

int main(int argc, char *argv[]){
  test_open_audio();
  test_close_audio();
  exit(0);
}
