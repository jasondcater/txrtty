#include <alsa/asoundlib.h>

static char *device = "plughw:0,0"; /* "default" also works here */

snd_output_t *output = NULL;
unsigned char buffer[16*1024];

int open_audio(){

  int err;
  unsigned int a;
  snd_pcm_t *handle;
  snd_pcm_sframes_t frames;

  for(a = 0; a < sizeof(buffer); a++){
    buffer[a] = random() & 0xff;
  }

  err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);

  if(err < 0){
    printf("Playback open error: %s\n", snd_strerror(err));
    return 1;
  }

  err = snd_pcm_set_params(
    handle,
    SND_PCM_FORMAT_U8,
    SND_PCM_ACCESS_RW_INTERLEAVED,
    1,
    48000,
    1,
    500000
  );

  if(err < 0){
    printf("Playback open error: %s\n", snd_strerror(err));
    return 1;
  }

  for(a = 0; a < 16; a++){
    frames = snd_pcm_writei(handle, buffer, sizeof(buffer));

    if(frames < 0){
      frames = snd_pcm_recover(handle, frames, 0);
    }

    if(frames < 0){
      printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
      break;
    }

    if(frames > 0 && frames < (long)sizeof(buffer)){
      printf("Short write (expected %li, wrote %li)\n", (long)sizeof(buffer), frames);
    }
  }

  snd_pcm_close(handle);
  return 0;
}

void close_audio(){
}
