#include "../common.h"

#include <stdlib.h>
#include <stdio.h>

void test_msg_to_baudot(){
  int *p_msg_buffer;
  int msg_buffer[BUFSIZ];

  // This would be the message you would want to send.
  char msg[13] = "spcstn-kj6zrf";

  /*
   * This should be the message converted into our Baudot code. The first index
   * is reserved as a count of elements of the message.
   */
  int baudot[] = {
    20, 127, 82, 55, 59, 82, 7, 26, 110, 14, 127, 122, 107, 110, 87, 127, 70,
    42, 91, 35
  };

  p_msg_buffer = &msg_buffer[0];
  msg_to_baudot(msg, p_msg_buffer);

  size_t baudot_size = sizeof(baudot) / sizeof(int);

  int a;
  for(a = 0; a < baudot_size; a++){
    if(baudot[a] != msg_buffer[a]){
      printf("msg_to_baudot() failed\n");
      return;
    }
  }
}

void test_baudot_to_fsk(){
}

int main(int argc, char *argv[]){
  test_msg_to_baudot();
  test_baudot_to_fsk();
  exit(0);
}
