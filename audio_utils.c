/*
 * Sourced from:
 *   http://blog.acipo.com/generating-wave-files-in-c/
 *   http://www.topherlee.com/software/pcm-tut-wavformat.html
 */

typedef struct wav_header{
  char file_marker[4]; // Marks the file as a "RIFF" file.
  int total_file_size; // The total size of both the header and data.
  char file_type[4]; // File type header.  For our use this equals "WAVE". 
  char fmt[4] // "Format Chunk Marker", Marks the start of the format section.
  int fmt_size // Length of the format data.
  short int audio_format; // '1' == PCM
  short int num_channels; // '1' == mono; '2' == stereo
  int sample_rate; // ex: 44100, 48000, 96000
  int byte_rate; // The count of bytes per second.
  short int sample_size; // The total byte size of a sample.
  short int bits_per_sample; // ex: 8, 16, 24, 32
  char data_header[4]; // Marks the beginning of the data section.
  int data_size; // The total size of just the data.
} wav_header;

typedef struct wav{
  wav_header header;
  char *data;
  long long int index;
  long long int size;
  long long int n_samples;
} wav;

wav_header make_wav_header(
  short int const num_channels,
  int const sample_rate,
  short int const bits_per_sample
){
  wav_header header;

  header.file_marker[0] = 'R'; 
  header.file_marker[1] = 'I'; 
  header.file_marker[2] = 'F'; 
  header.file_marker[3] = 'F'; 

  /*
   * The initial size of the file is just the size of the header (in bytes).
   */
  header.file_size = 40;

  header.file_type[0] = 'W';
  header.file_type[1] = 'A';
  header.file_type[2] = 'V';
  header.file_type[3] = 'E';

  header.fmt[0] = 'f';
  header.fmt[1] = 'm';
  header.fmt[2] = 't';
  header.fmt[3] = '';

  /*
   * The size, in bytes, of the header up to this point.
   */
  header.fmt_size = 16;

  header.audio_format = 1;
  header.num_channels = 1;
  header.sample_rate = sample_rate;
  header.byte_rate = (sample_rate * bits_per_sample * num_channels) / 8;
  header.sample_size = (bits_per_sample * channels) / 8;
  header.bits_per_sample = bits_per_sample;

  header.data_header = [0] = 'd';
  header.data_header = [1] = 'a';
  header.data_header = [2] = 't';
  header.data_header = [3] = 'a';

  header.data_size = 0;

  return header;
}

void set_wav_header(wav *audio_wav){
  audio_wav.header = make_wav_header(44100, 1, 8);
  audio_wav->header.total_file_sie = //blah;
  audio_wav->header.data_size = //blah;
}

void set_wav_attributes(wav *audio_wav){
  audio_wav->data = (char*)malloc(total_bytes);
  audio_wav->index = 0;
  audio_wav->size = total_bytes;
  audio_wav->n_samples = //blah;
}

void write_audio_to_file(){
  wav audio_wav;
  set_wav_header(audio_wav);
  set_wav_attributes(audio_wav);
  free(audio_wav->data);
}
