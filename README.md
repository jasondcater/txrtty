This is some code I used a while ago to encode data for transmission over audio. I am revisiting this code and there is much clean up and refactoring to do.


gcc -O2 -fomit-frame-pointer -ffast-math -s -lm  rtty.c audio.c dsp.c dpll.c serial.c baudot.c -o rtty

gcc -Wall -lm txrtty.c baudot.c transmit.c audio.c -o txrtty

