#include <stdint.h>

// msg_processing.c
void msg_to_baudot(const char *msg, uint32_t *p_msg_buffer);
void baudot_to_fsk(uint32_t *p_msg_buffer, uint32_t *p_tx_buffer);

// binary_utils.c
void reverse_endian(uint64_t data_size, uint8_t width, uint8_t *data);
uint32_t is_big_endian();

// audio_utils.c
void write_audio_to_file(
  uint8_t *audio_buffer,
  uint32_t data_size,
  uint32_t sample_rate,
  uint16_t num_channels,
  uint16_t bit_depth
);

// audio.c
int64_t open_audio();
void close_audio();
int64_t generate_sine(double hertz);
void write_audio_file();
