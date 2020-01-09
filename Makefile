CC = gcc
CFLAGS = -Wall -lm -lasound -lpthread
TARGET = txrtty

all: $(TARGET)

$(TARGET): $(TARGET).c msg_procesing.c audio.c
	$(CC) $(CFLAGS) -o ./bin/$(TARGET) $(TARGET).c msg_processing.c audio.c

clean:
	rm $(TARGET)

