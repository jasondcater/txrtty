#include <stdint.h>

// msg_processing.c
void msg_to_baudot(char msg[], int *p_msg_buffer);
void baudot_to_fsk(int *p_msg_buffer, int *p_tx_buffer);

// audio.c
uint32_t open_audio();
void close_audio();
uint32_t generate_sine(double hertz);
void write_audio_file();

// audio_utils.c
void write_audio_to_file();
