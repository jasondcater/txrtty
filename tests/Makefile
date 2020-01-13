CC = gcc
CFLAGS = -Wall -g
CLIBS = -lm -lasound -lpthread

msg_processing_test: msg_processing_test.c ../msg_processing.c
	$(CC) $(CFLAGS) -o ./bin/msg_processing_test msg_processing_test.c ../msg_processing.c $(CLIBS)

audio_test: audio_test.c ../audio.c
	$(CC) $(CFLAGS) -o ./bin/audio_test audio_test.c ../audio.c $(CLIBS)

clean:
	rm ./bin/*
