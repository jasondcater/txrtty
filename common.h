/*---------- baudot.c ----------*/
void baudot(char msg[], int *msg_buf);
void parse(int *msg_buf, double *trs_buf);

/*---------- audio.c ----------*/
int open_audio();
void write_wave(double hertz);
