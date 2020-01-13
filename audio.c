#include <alsa/asoundlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define DEVICE_ID "plughw:0,0" // "default" also works here.
#define SAMPLE_RATE 48000.0
#define BAUD 1200.0

// Audio device
snd_output_t *output = NULL;
snd_pcm_t *audio_device_handle;
unsigned int latency = 500000; // In microseconds.

// Sample generation
double time_in_secs = 0.0; // Used in the calcuation of audio samples.
double sample_length = 1.0/SAMPLE_RATE;
double amplitude = 0.8;
double sample_range = pow(2,16)/2; //16 bits divided by 2, 32768 to -32768
double audio_buffer[64 * 1024];

/*
 * Each character in a message to send will be comprised of 8 bits. In this AFSK
 * encoding schema high and low bits are encoded to high and low frequencies set
 * at 170 hertz appart (see msg_processing.c). When we convert these mark/space
 * values to audio samples we need to know how many samples per mark/space to
 * generate.
 */
int samples_per_mark = SAMPLE_RATE/BAUD;

int open_audio(){
  int err;

  err = snd_pcm_open(
    &audio_device_handle,
    DEVICE_ID,
    SND_PCM_STREAM_PLAYBACK,
    0
  );

  if(err < 0){
    printf("Playback open error: %s\n", snd_strerror(err));
    return 1;
  }

  err = snd_pcm_set_params(
    audio_device_handle,
    SND_PCM_FORMAT_U8,
    SND_PCM_ACCESS_RW_INTERLEAVED,
    1,
    SAMPLE_RATE,
    1,
    latency
  );

  if(err < 0){
    printf("Playback open error: %s\n", snd_strerror(err));
    return 1;
  }

  return 0;
}

void close_audio(){
  snd_pcm_close(audio_device_handle);
}

// Takes a frequency and then loads a buffer with generated sine wave data.
void generate_sine(double hertz){
  double cycles_per_sec = (360.0 * hertz)/(180.0/M_PI);
  for(int a = 0; a < samples_per_mark; a++){
    double sample = (amplitude*sin(cycles_per_sec*time_in_secs))*sample_range;
    time_in_secs = time_in_secs + sample_length;
    audio_buffer[a] = sample;
  }
}

/*
// Takes a frequency and then loads a buffer with generated square wave data.
void generate_square(double hertz){
  double cycles_per_sec = (360.0 * hertz)/(180.0/M_PI);
}

// Writes the wave buffer to the audio device.
void write_wave(){

}
*/



