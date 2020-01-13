// msg_processing.c
void msg_to_baudot(char msg[], int *p_msg_buffer);
void baudot_to_fsk(int *p_msg_buffer, int *p_tx_buffer);

// audio.c
int open_audio();
void close_audio();
