#include <stdint.h>

/*
 * 'data_size' is the count of bytes for whatever object we need to reverse.
 * 'width' is the size of your type.
 * We are also performing a little trick with '*data'. By casting a pointer to 
 * a uint8_t we can read off whatever is at that pointer per byte.
 *
 * Assuing 'rand_data' is a uint32_t type, we need to call this fucntion like
 * so:
 *
 * reverse_endianness(
 *   sizeof(rand_data),
 *   sizeof(uint32_t),
 *   (uint8_t *)&rand_data
 * );
 */

void reverse_endian(uint64_t data_size, uint8_t width, uint8_t *data){
  int64_t a;
  uint64_t index, offset = 0;
  uint8_t buffer[data_size]; // Holds a copy of the data to be reversed.

  // Write the value to be copied into the buffer.
  for(a = 0; a < data_size; a++){
    buffer[a] = data[a];
  }

  for(a = 0; a < data_size; a++){
    index = (offset * width) + (width - 1) - (a % width);
    if(!((a + 1) % width)) offset++;
    data[a] = buffer[index];
  }
}

int is_big_endian(){
  uint32_t test = 1;
  uint8_t *p = (uint8_t *)&test;
  return p[0] == 0;
}
