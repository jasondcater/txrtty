#include "../common.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void test_open_audio(){
  int64_t status = open_audio();
  if(status < 0) printf("test_audio_open() failed. Wrong status code.\n");
}

void test_close_audio(){
  close_audio();
}

void test_generate_sine(){

  /*
   * Here we are making "status" do double duty. If the status is less than zero
   * then we can use it as a status code. Otherwise, "status" will be the
   * address of the audio buffer that was filled. As you can see we do some
   * casting of "status" to output the individual bytes in the audio buffer.
   */
  int64_t status = generate_sine(2125.0);
  if(status < 0) printf("test_generate_sine() failed. Wrong status code.\n");

  // This data represents the first 20 bytes of a generated sine wave.
  uint8_t sample_data[] = {
    29, 28, 18, 54, 222, 75, 214, 91, 190, 100, 231, 101, 59, 95, 60, 81, 255,
    60, 17, 36 
  };

  for(uint8_t a = 0; a < 20; a++){
    if((uint8_t)*((int8_t *)status+a) != sample_data[a]){
      printf("test_generate_sine() failed. Generated data does not match.\n");
    }
  }
}

int main(){
  test_open_audio();
  test_close_audio();
  test_generate_sine();
  exit(0);
}
