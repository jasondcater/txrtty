#include <alsa/asoundlib.h>

static char *device_id = "plughw:0,0"; /* "default" also works here */
snd_output_t *output = NULL;
snd_pcm_t *audio_device_handle;

//unsigned char buffer[16*1024];
unsigned int sample_rate = 48000;
unsigned int latency = 500000; // In microseconds.

int open_audio(){
  int err;

  err = snd_pcm_open(
    &audio_device_handle,
    device_id,
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
    sample_rate,
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

}

// Takes a frequency and then loads a buffer with generated square wave data.
void generate_square(double hertz){

}

// Writes the wave buffer to the audio device.
void write_wave(){

}

