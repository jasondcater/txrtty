/*
 * Sourced from:
 *   http://blog.acipo.com/generating-wave-files-in-c/
 *   http://www.topherlee.com/software/pcm-tut-wavformat.html
 */
#include "./common.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct wav_header{
  uint8_t file_marker[4]; // Marks the file as a "RIFF" file.
  uint32_t file_size; // The total size of both the header and data.
  uint8_t file_type[4]; // File type header.  For our use this equals "WAVE". 
  uint8_t fmt[4]; // "Format Chunk Marker", Marks the start of the format section.
  uint32_t fmt_size; // Length of the format data.
  uint16_t audio_format; // '1' == PCM
  uint16_t num_channels; // '1' == mono; '2' == stereo
  uint32_t sample_rate; // ex: 44100, 48000, 96000
  uint32_t byte_rate; // The count of bytes per second.
  uint16_t block_align; // The total byte size of a sample.
  uint16_t bit_depth; // ex: 8, 16, 24, 32
  uint8_t data_header[4]; // Marks the beginning of the data section.
  uint32_t data_size; // The total size of just the data.
} wav_header;

typedef struct wav{
  wav_header header;
  uint8_t *data;
  uint64_t index;
  uint64_t size; // In bytes
  uint64_t n_samples;
} wav;

wav_header make_wav_header(
  uint32_t const sample_rate,
  uint16_t const num_channels,
  uint16_t const bit_depth
){
  wav_header header;

  header.file_marker[0] = 'R'; 
  header.file_marker[1] = 'I'; 
  header.file_marker[2] = 'F'; 
  header.file_marker[3] = 'F'; 

  /*
   * The initial size of the file is just the size of the header (in bytes).
   */
  header.file_size = 44;

  header.file_type[0] = 'W';
  header.file_type[1] = 'A';
  header.file_type[2] = 'V';
  header.file_type[3] = 'E';

  header.fmt[0] = 'f';
  header.fmt[1] = 'm';
  header.fmt[2] = 't';
  header.fmt[3] = '\0';

  /*
   * The size, in bytes, of the header up to this point.
   */
  header.fmt_size = 16;

  header.audio_format = 1;
  header.num_channels = 1;
  header.sample_rate = sample_rate;
  header.byte_rate = (sample_rate * num_channels * bit_depth) / 8;
  header.block_align = (num_channels * bit_depth) / 8;
  header.bit_depth = bit_depth;

  header.data_header[0] = 'd';
  header.data_header[1] = 'a';
  header.data_header[2] = 't';
  header.data_header[3] = 'a';

  header.data_size = 0;

  return header;
}

void set_wav_header(
  wav *audio_wav,
  uint32_t data_size,
  uint32_t sample_rate,
  uint16_t num_channels,
  uint16_t bit_depth
){
  audio_wav->header = make_wav_header(sample_rate, num_channels, bit_depth);
  audio_wav->header.file_size = data_size + audio_wav->header.file_size;
  audio_wav->header.data_size = data_size;
}

void set_wav_attributes(
  wav *audio_wav,
  uint32_t data_size,
  uint16_t num_channels,
  uint16_t bit_depth
){
  audio_wav->data = (uint8_t*)malloc(data_size);
  audio_wav->index = 0;
  audio_wav->size = (uint64_t)data_size;
  audio_wav->n_samples = (uint64_t)(data_size/((num_channels*bit_depth)/8));
}

void set_wav_data(wav *audio_wav, uint8_t *audio_buffer, uint32_t data_size){
  for(uint32_t a = 0; a < data_size; a++){
    audio_wav->data[a] = audio_buffer[a];
  }
}
/*
 * reverse_endianness(
 *   sizeof(rand_data),
 *   sizeof(uint32_t),
 *   (uint8_t *)&rand_data
 * );
 */
/*
void reverse_header_endian(wav *audio_wav){
  reverse_endian();
}
*/

void set_wav_to_file(wav *audio_wav, const uint8_t *file_name){
  //if(is_big_endian()) reverse_header_endian(audio_wav);

  FILE *file;
  file = fopen((char *)file_name, "wb");
  fwrite(&(audio_wav->header), sizeof(wav_header), 1, file);
  fwrite((void *)(audio_wav->data), sizeof(uint8_t), audio_wav->size, file);
  fclose(file);
}

void write_audio_to_file(
  uint8_t *audio_buffer,
  uint32_t data_size, // data_size is the number of bytes in the audio buffer.
  uint32_t sample_rate,
  uint16_t num_channels,
  uint16_t bit_depth
){
  uint8_t file_name[] = "/tmp/example.wav";
  wav audio_wav;
  set_wav_header(&audio_wav, data_size, sample_rate, num_channels, bit_depth);
  set_wav_attributes(&audio_wav, data_size, num_channels, bit_depth);
  set_wav_data(&audio_wav, audio_buffer, data_size);
  set_wav_to_file(&audio_wav, file_name);
  free(audio_wav.data);
}
