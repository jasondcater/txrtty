#include <alsa/asoundlib.h>
#include <math.h>
#include <float.h>
#include <stdint.h>

#define M_PI 3.14159265358979323846
#define M_TAU 6.283185307179586
#define DEVICE_ID "plughw:0,0" // "default" also works here.
#define SAMPLE_RATE 48000.0
#define NUM_CHANNELS 1 // 1 == Mono, 2 == Stereo
#define BIT_DEPTH 16
#define BAUD 1200.0

// Audio device
snd_output_t *output = NULL;
snd_pcm_t *audio_device_handle;
uint32_t latency = 500000; // In microseconds.

// Sample generation
double sample_length = 1.0/SAMPLE_RATE;
double amplitude = 0.8; // 0.0 min volume, 1.0 max volume
uint8_t audio_buffer[64 * 1024]; // Currently set at 64K
uint64_t size = 0; // The actual size of the generated sample data in bytes.
uint64_t n_samples = 0;

/*
 * As we generate wave forms we want to keep the last angle/phase we calculated.
 * When we start a new round of calcuations we just pick up from the angle of
 * the previous set. When we generate wave forms back to back this prevents
 * "clipping" between the wave forms since each new wave form doens't start from
 * zero, but from the last calcuated value.
 *
 * We start theta off at 270 deg (3/4 tau) because we will shift the "ground" of
 * the waveform up from (-0.5 to 0.5) to (0.0 to 1.0).
 */
double theta = M_TAU * (3.0/4.0);

uint32_t check_bit_depth(){
  if(BIT_DEPTH == 8 || BIT_DEPTH == 16) return 0;
  return 1;
}

uint32_t check_amplitude(){
	if(amplitude >= 0.0 && amplitude <= 1.0) return 0;
  return 1;
}

uint32_t open_audio(){
  if(check_bit_depth()){
    printf("open_audio() error, wrong BIT_DEPTH set.\n");
    return 1;
  }

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
uint32_t generate_sine(double frequency){
  if(check_bit_depth()){
    printf("generate_sine() error, wrong 'BIT_DEPTH' set.\n");
    return 1;
  }

  if(check_amplitude()){
    printf("generate_sine() error, wrong 'amplitude' set.\n");
    return 1;
  }

  double radian_per_sample = (frequency / SAMPLE_RATE) * M_TAU;

  for(uint32_t a = 0; a < (SAMPLE_RATE / BAUD); a++){
    theta = fmod((theta + radian_per_sample), M_TAU);
    double float_sample = ((amplitude / 2.0) * sin(theta)) + (amplitude / 2.0);

		/*
     * Convert a precision float representation of a sample to an interger 
		 * representation of the correct bit depth.
		 */

    if(BIT_DEPTH == 8){
      uint8_t int_sample = (uint8_t)(float_sample * (double)UINT8_MAX);
      audio_buffer[a] = int_sample;
      size += 1;
    }

    if(BIT_DEPTH == 16){
      uint16_t int_sample = (uint16_t)(float_sample * (double)UINT16_MAX);
      audio_buffer[a*2] = int_sample & 0xff;
      audio_buffer[(a*2)+1] = (int_sample >> 8);
      size += 2;
    }
  }

  return 0;
}
/*
// Writes the audio_buffer to disk as raw audio.
void write_audio_file(){
  write_audio_to_file(
    audio_buffer,
    size, // The size of the generated audio data in bytes.
    (uint32_t)SAMPLE_RATE,
    (uint16_t)NUM_CHANNELS,
    (uint16_t)BIT_DEPTH
  ); 
}
*/
/*
// Writes the wave buffer to the audio device.
void write_wave(){

}
*/
