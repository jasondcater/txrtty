CC = gcc
CFLAGS = -Wall -lm -lasound -lpthread
TARGET = txrtty

all: $(TARGET)

$(TARGET): $(TARGET).c msg_procesing.c audio.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c msg_processing.c audio.c

clean:
	rm $(TARGET)

build_test: test_$(TARGET).c msg_processing.c
	$(CC) $(CFLAGS) -o test_$(TARGET) test_$(TARGET).c msg_processing.c

test:
	test_$(TARGET)
