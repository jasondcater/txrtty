#include "common.h"

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

/*
 * These are the mark/space frequencies for the FSK. A mark will map to a
 * high bit and a space will map to a low bit. For now these are hard set, but
 * in the future will be pulled from a config file.
 */
uint32_t mark = 2295;
uint32_t space = 2125;

// Baudot to RTTY tables. See the README.md for an explination on these arrays.
uint32_t table_size = 30;
uint32_t shift_to_letters = 127;

uint8_t letters[] = {
  '\0', ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D',
  'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '\r', '\n'
};

uint32_t letter_codes[] = {
  2, 19, 118, 103, 67, 42, 7, 87, 115, 50, 14, 55, 98, 82, 74, 91, 47, 22, 107,
  122, 38, 70, 94, 59, 62, 78, 26, 31, 11, 35
};

uint32_t shift_to_figures = 110;

uint8_t figures[] = {
  '\0', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '\a', '$',
  '!', '&', '#', '\'', '(', ')', '\"', '/', ':', ';', '\?', ',', '.', '\r',
  '\n'
};

uint32_t figure_codes[] = {
  2, 19, 94, 79, 7, 42, 67, 87, 31, 26, 98, 91, 14, 22, 38, 55, 107, 82, 47,
  62, 74, 70, 118, 59, 122, 103, 50, 115, 35, 11
};

void msg_to_baudot(const char *msg, uint32_t *p_msg_buffer){
  int32_t a, b;
  uint32_t c = 1;

  /*
   * When we start the encoding sequence we need to set the code for a figure or
   * character shift. We set a `-1` to force an initial shift on the first
   * character.
   */
  uint32_t shift = -1;

  /*
   * For each character in the message we just loop the arrays and match the 
   * correct code. We also get a count of characters in the message here as
   * well.
   */
  for(a = 0; a < strlen(msg); a++){
    for(b = 0; b < table_size; b++){

      if(toupper(msg[a]) == letters[b]){
        if(shift != 0){
          shift = 0;
          p_msg_buffer[c] = shift_to_letters;
          c++;
        }

         p_msg_buffer[c] = letter_codes[b];
         c++;
         break;
      }

      if(toupper(msg[a]) == figures[b]){
        if(shift != 1){
          shift = 1;
          p_msg_buffer[c] = shift_to_figures;
          c++;
        }

        p_msg_buffer[c] = figure_codes[b];
        c++;
        break;
      }
    }
  }

  // After we convert the message we add the `newline` code.
  switch(shift){
    case 0:
      p_msg_buffer[c] = letter_codes[29];
      c++;
      break;
    case 1:
      p_msg_buffer[c] = figure_codes[29];
      c++;
      break;
    default:
      break;
  }

  p_msg_buffer[0] = c; // Set the array size in the first index of the array.
}

/*
 * To convert a decimal to an encoded binary sequence of marks and spaces we get
 * the modulo of 2 from the decimal then divide that decimal by two and repeat.
 */
void baudot_to_fsk(uint32_t *p_msg_buffer, uint32_t *p_tx_buffer){
  *(p_tx_buffer) = p_msg_buffer[0]; // Set the msg size at the beginning.
  int32_t a, bit = 0;
  uint32_t decimal;

  // Remeber, we reserve the first index for recording the message length.
  for(a = 1; a < *(p_tx_buffer); a++){
    decimal = p_msg_buffer[a];
    for(bit = 7; bit >= 0; bit--){
      uint32_t bit_count = (a * 8) + bit;
      if(decimal%2){
        *(p_tx_buffer + bit_count) = mark;
      }
      else{
        *(p_tx_buffer + bit_count) = space;
      }

      decimal /= 2;
    }
  }
}
