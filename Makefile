CC = gcc
CFLAGS = -Wall
CLIBS = -lm -lasound -lpthread
TARGET = txrtty
FILES = msg_processing.c audio_utils.c audio.c

all: $(TARGET)

$(TARGET): $(TARGET).c $(FILES)
	$(CC) $(CFLAGS) -o ./bin/$(TARGET) $(TARGET).c $(FILES) $(CLIBS)

clean:
	rm ./bin/*

