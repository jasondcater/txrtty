gcc -O2 -fomit-frame-pointer -ffast-math -s -lm  rtty.c audio.c dsp.c dpll.c serial.c baudot.c -o rtty

gcc -Wall -lm txrtty.c baudot.c transmit.c audio.c -o txrtty

