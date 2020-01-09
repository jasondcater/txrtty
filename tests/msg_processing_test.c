#include "../common.h"

#include <stdlib.h>
#include <stdio.h>

void test_msg_processing(){
  int *p_msg_buffer;
  int msg_buffer[BUFSIZ];
  char msg[13] = "spcstn-kj6zrf";
  int baudot[] = {
    127, 83, 55, 59, 83, 7, 27, 111, 15, 127, 123, 107, 111, 87, 127, 71, 43, 91
  };

  p_msg_buffer = &msg_buffer[0];
  msg_to_baudot(msg, p_msg_buffer);

  size_t baudot_size = sizeof(baudot) / sizeof(int);

  int a;
  for(a = 0; a < baudot_size; a++){
    printf("%d, %d\n", baudot[a], msg_buffer[a]); 
  }

//  printf("%zu\n", baudot_size);
}

int main(int argc, char *argv[]){
  test_msg_processing();
  exit(0);
}
