#include <alsa/asoundlib.h>
#include <math.h>
#include <stdint.h>

#define M_PI 3.14159265358979323846
#define M_TAU 6.283185307179586 
#define DEVICE_ID "plughw:0,0" // "default" also works here.
#define SAMPLE_RATE 48000.0
#define NUM_CHANNELS 1
#define BIT_DEPTH 16
#define BAUD 1200.0

// Audio device
snd_output_t *output = NULL;
snd_pcm_t *audio_device_handle;
uint32_t latency = 500000; // In microseconds.

// Sample generation
double sample_length = 1.0/SAMPLE_RATE;
double amplitude = 0.8;
uint8_t audio_buffer[64 * 1024]; // Currently set at 64K

/*
 * As we generate wave forms we want to keep the last angle/phase we calculated.
 * When we start a new round of calcuations we just pick up from the angle of
 * the previous set. When we generate wave forms back to back this prevents
 * "clipping" between the wave forms since each new wave form doens't start from
 * zero, but from the last calcuated value.
 */
double theta = 0.0;

uint32_t open_audio(){
  uint32_t err;

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

  /*
   * There is an extensive list of output formats enumerated in ALSA. Since this
   * program is fairly simple we will stick with 8 and 16 bit depths for now.
   */
  uint32_t audio_format = SND_PCM_FORMAT_U8;
  if(BIT_DEPTH == 8) audio_format = SND_PCM_FORMAT_U8;
  if(BIT_DEPTH == 16) audio_format = SND_PCM_FORMAT_U16_LE;

  err = snd_pcm_set_params(
    audio_device_handle,
    audio_format,
    SND_PCM_ACCESS_RW_INTERLEAVED,
    NUM_CHANNELS,
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

/*
 * Each character in a message to send will be comprised of 8 bits. In this AFSK
 * encoding schema high and low bits are encoded to high and low frequencies set
 * at 170 hertz appart (see msg_processing.c). When we convert these mark/space
 * values to audio samples we need to know how many samples per mark/space to
 * generate.
 *
 * To prevent the accumulation (and possible overflow) of our theta value we can
 * simply take the modulus of tau.
 */
void generate_sine(double frequency){
  double radian_per_sample = (frequency / SAMPLE_RATE) * M_TAU;

  for(uint32_t a = 0; a < (SAMPLE_RATE / BAUD); a++){
    theta = fmod((theta + radian_per_sample), M_TAU);
    double sample = amplitude * sin(theta);
    audio_buffer[a] = sample;
  }
}
/*
// Writes the audio_buffer to disk as raw audio.
void write_audio_file(){
  write_audio_to_file(
    audio_buffer,
    data_size, // Data size in bytes.
    (uint32_t)SAMPLE_RATE,
    (uint16_t)NUM_CHANNELS,
    (uint16_t)BIT_DEPTH
  ); 
}*/

/*
// Writes the wave buffer to the audio device.
void write_wave(){

}
*/
