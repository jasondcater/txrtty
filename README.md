# TXRTTY

This is some code I used a while ago to encode data for transmission over audio. I am revisiting this code and there is much clean up and refactoring to do.

## RTTY table

These tables represents the baudot rtty codes. The baudot code is a 5 bit code that represents 58 symbols. The symbols and bit code mappings are almost arbitrary. The original baudot mapping was based upon the visual look of the bits and the "qwerty" keyboard layout.

The rtty broadcast of the baudot codes require 1 start bit and two stop bits. When you include these 3 bits to the 5 bit baudot code you end up with a nice 8 bit (byte) The structure of the new byte is 1 space bit (0), then the 5 bit baudot code, and ended with 2 mark bits (1). It would look a bit like this, 0XXXXX11 (where the X's are the baudot code) and is sorted by LSB.

These new bytes map nicely to regular integer data types. The mapping to letters/symbols are based upon the "C" programming character set. These DO NOT map to the ascii table (you are going to have to do that yourself).

This first table is the standard baudot table mapping: !!! In LSB
```
00000      '\0'      00000     '\0'
00100      ' '       00100     ' '
10111      '0'       11101     'Q'
10011      '2'       11001     'W'
00001      '3'       10000     'E'
01010      '4'       01010     'R'
10000      '5'       00001     'T'
10101      '6'       10101     'Y'
00111      '7'       11100     'U'
00110      '8'       01100     'I'
11000      '9'       00011     'O'
10110      '0'       01101     'P'
00011      '-'       11000     'A'
00101      '\a'      10100     'S'
01001      '$'       10010     'D'
01101      '!'       10110     'F'
11010      '&'       01011     'G'
10100      '#'       00101     'H'
01011      '\''      11010     'J'
01111      '('       11110     'K'
10010      ')'       01001     'L'
10001      '\"'      10001     'Z'
11101      '/'       10111     'X'
01110      ':'       01110     'C'
11110      ';'       01111     'V'
11001      '\?'      10011     'B'
01100      ','       00110     'N'
11100      '.'       00111     'M'
01000      '\r''     00010     '\r
00010      '\n''     0100      '\n
11011      STF       11011     STF
11111      STL       11111     STL
```

This second table is the baudot code with the start bit and two ending bits plus the decimal conversion from the newly created byte.
```
00000011, 3    '\0'   00000011, 3      '\0'
00010011, 19   ' '    00010011, 19     ' '
01011111, 95   '1'    01110111, 119    'Q'
01001111, 79   '2'    01100111, 103    'W'
00000111, 7    '3'    01000011, 67     'E'
00101011, 43   '4'    00101011, 43     'R'
01000011, 67   '5'    00000111, 7      'T'
01010111, 87   '6'    01010111, 87     'Y'
00011111, 31   '7'    01110011, 115    'U'
00011011, 27   '8'    00110011, 51     'I'
01100011, 99   '9'    00001111, 15     'O'
01011011, 91   '0'    00110111, 55     'P'
00001111, 15   '-'    01100011, 99     'A'
00010111, 23   '\a'   01010011, 83     'S'
00100111, 39   '$'    01001011, 75     'D'
00110111, 55   '!'    01011011, 91     'F'
01101011, 107  '&'    00101111, 47     'G'
01010011, 83   '#'    00010111, 23     'H'
00101111, 47   '\''   01101011, 107    'J'
00111111, 63   '('    01111011, 123    'K'
01001011, 75   ')'    00100111, 39     'L'
01000111, 71   '\"'   01000111, 71     'Z'
01110111, 119  '/'    01011111, 95     'X'
00111011, 59   ':'    00111011, 59     'C'
01111011, 123  ';'    00111111, 63     'V'
01100111, 103  '\?'   01001111, 79     'B'
00110011, 51   ','    00011011, 27     'N'
01110011, 115  '.'    00011111, 31     'M'
00100011, 35   '\r'   00001011, 11     '\r'
00001011, 11   '\n'   00100011, 35     '\n'
01101111, 111  STF    01101111, 111    STF
01111111, 127  STL    01111111, 127    STL
```
This third table is the baudot code with the start bit, one ending bit and one parity bit. Parity is set to odd.
```
00000010, 2   '\0'    00000010, 2      '\0'
00010011, 19  ' '     00010011, 19     ' '
01011110, 94  '1'     01110110, 118    'Q'
01001111, 79  '2'     01100111, 103    'W'
00000111, 7   '3'     01000011, 67     'E'
00101010, 42  '4'     00101010, 42     'R'
01000011, 67  '5'     00000111, 7      'T'
01010111, 87  '6'     01010111, 87     'Y'
00011111, 31  '7'     01110011, 115    'U'
00011010, 26  '8'     00110010, 50     'I'
01100010, 98  '9'     00001110, 14     'O'
01011011, 91  '0'     00110111, 55     'P'
00001110, 14  '-'     01100010, 98     'A'
00010110, 22  '\a'    01010010, 82     'S'
00100110, 38  '$'     01001010, 74     'D'
00110111, 55  '!'     01011011, 91     'F'
01101011, 107 '&'     00101111, 47     'G'
01010010, 82  '#'     00010110, 22     'H'
00101111, 47  '\''    01101011, 107    'J'
00111110, 62  '('     01111010, 122    'K'
01001010, 74  ')'     00100110, 38     'L'
01000110, 70  '\"'    01000110, 70     'Z'
01110110, 118 '/'     01011110, 94     'X'
00111011, 59  ':'     00111011, 59     'C'
01111010, 122 ';'     00111110, 62     'V'
01100111, 103 '\?'    01001110, 78     'B'
00110010, 50  ','     00011010, 26     'N'
01110011, 115 '.'     00011111, 31     'M'
00100011, 35  '\r'    00001011, 11     '\r'
00001011, 11  '\n'    00100011, 35     '\n'
01101110, 110  STF     01101110, 110    STF
01111111, 127  STL     01111111, 127    STL
```
Example (my call sign):
SPCSTN-KJ6ZRF, SPACE STATION - KJ6ZRF
```
01111111, 127    STL
01010010, 82     'S'
00110111, 55     'P'
00111011, 59     'C'
01010010, 82     'S'
00000111, 7      'T'
00011010, 26     'N'
01101110, 110    STF
00001110, 14     '-'
01111111, 127    STL
01111010, 122    'K'
01101011, 107    'J'
01101110, 110    STF
01010111, 87     '6'
01111111, 127    STL
01000110, 70     'Z'
00101010, 42     'R'
01011011, 91     'F'
00100011, 35     '\n'
```
## Old notes

gcc -O2 -fomit-frame-pointer -ffast-math -s -lm  rtty.c audio.c dsp.c dpll.c serial.c baudot.c -o rtty

gcc -Wall -lm txrtty.c baudot.c transmit.c audio.c -o txrtty
