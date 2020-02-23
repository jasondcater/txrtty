#include "../common.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void test_msg_to_baudot(){
  uint32_t *p_msg_buffer;
  uint32_t msg_buffer[BUFSIZ];

  // This would be the message you would want to send.
  char msg[13] = "spcstn-kj6zrf";

  /*
   * This should be the message converted into our Baudot code. The first index
   * is reserved as a count of elements of the message.
   */
  uint32_t baudot[] = {
    20, 127, 82, 55, 59, 82, 7, 26, 110, 14, 127, 122, 107, 110, 87, 127, 70,
    42, 91, 35
  };

  p_msg_buffer = &msg_buffer[0];
  msg_to_baudot(msg, p_msg_buffer);

  size_t baudot_size = sizeof(baudot) / sizeof(int);

  uint32_t a;
  for(a = 0; a < baudot_size; a++){
    if(baudot[a] != msg_buffer[a]){
      printf("msg_to_baudot() failed\n");
      return;
    }
  }
}

void test_baudot_to_fsk(){
  uint32_t *p_msg_buffer;
  uint32_t msg_buffer[BUFSIZ];
  uint32_t *p_tx_buffer;
  uint32_t tx_buffer[BUFSIZ][8];

  // This is the baudot encoding for "spcstn-kj6zrf".
  uint32_t baudot[] = {
    20, 127, 82, 55, 59, 82, 7, 26, 110, 14, 127, 122, 107, 110, 87, 127, 70,
    42, 91, 35
  };

  // This is the fsk binary representation of the baudot code above.
  uint32_t fsk[][8] = {
    {20, 0, 0, 0, 0, 0, 0, 0},
    {2125, 2295, 2295, 2295, 2295, 2295, 2295, 2295},
    {2125, 2295, 2125, 2295, 2125, 2125, 2295, 2125},
    {2125, 2125, 2295, 2295, 2125, 2295, 2295, 2295},
    {2125, 2125, 2295, 2295, 2295, 2125, 2295, 2295},
    {2125, 2295, 2125, 2295, 2125, 2125, 2295, 2125},
    {2125, 2125, 2125, 2125, 2125, 2295, 2295, 2295},
    {2125, 2125, 2125, 2295, 2295, 2125, 2295, 2125},
    {2125, 2295, 2295, 2125, 2295, 2295, 2295, 2125},
    {2125, 2125, 2125, 2125, 2295, 2295, 2295, 2125},
    {2125, 2295, 2295, 2295, 2295, 2295, 2295, 2295},
    {2125, 2295, 2295, 2295, 2295, 2125, 2295, 2125},
    {2125, 2295, 2295, 2125, 2295, 2125, 2295, 2295},
    {2125, 2295, 2295, 2125, 2295, 2295, 2295, 2125},
    {2125, 2295, 2125, 2295, 2125, 2295, 2295, 2295},
    {2125, 2295, 2295, 2295, 2295, 2295, 2295, 2295},
    {2125, 2295, 2125, 2125, 2125, 2295, 2295, 2125},
    {2125, 2125, 2295, 2125, 2295, 2125, 2295, 2125},
    {2125, 2295, 2125, 2295, 2295, 2125, 2295, 2295},
    {2125, 2125, 2295, 2125, 2125, 2125, 2295, 2295}
  };

  p_msg_buffer = &msg_buffer[0];
  p_tx_buffer = &tx_buffer[0][0];

  uint32_t a;
  for(a = 0; a < baudot[0]; a++){
    p_msg_buffer[a] = baudot[a];
  }

  baudot_to_fsk(p_msg_buffer, p_tx_buffer);

  uint32_t b, c;
  for(b = 0; b < baudot[0]; b++){
    for(c = 0; c < 8; c++){
      if(fsk[b][c] != tx_buffer[b][c]){
        printf("baudot_to_fsk() failed\n");
        return;
      }
    }
  }
}

int main(int argc, char *argv[]){
  test_msg_to_baudot();
  test_baudot_to_fsk();
  exit(0);
}
