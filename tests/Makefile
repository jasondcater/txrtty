CC = gcc
CFLAGS = -Wall -lm -lasound -lpthread -g

build_msg_processing_test: msg_processing_test.c ../msg_processing.c
	$(CC) $(CFLAGS) -o ./bin/msg_processing_test msg_processing_test.c ../msg_processing.c

