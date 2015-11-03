#define using_more
#include <more/base64.h>

#define DEC_ERR -1

static char enc64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static char dec64[] = {
           // (index) (ascii char)
  DEC_ERR, // 0
  DEC_ERR, // 1
  DEC_ERR, // 2
  DEC_ERR, // 3 
  DEC_ERR, // 4
  DEC_ERR, // 5
  DEC_ERR, // 6
  DEC_ERR, // 7
  DEC_ERR, // 8
  DEC_ERR, // 9
  DEC_ERR, // 10
  DEC_ERR, // 11
  DEC_ERR, // 12
  DEC_ERR, // 13
  DEC_ERR, // 14
  DEC_ERR, // 15
  DEC_ERR, // 16
  DEC_ERR, // 17
  DEC_ERR, // 18
  DEC_ERR, // 19
  DEC_ERR, // 20
  DEC_ERR, // 21
  DEC_ERR, // 22
  DEC_ERR, // 23
  DEC_ERR, // 24
  DEC_ERR, // 25
  DEC_ERR, // 26
  DEC_ERR, // 27
  DEC_ERR, // 28
  DEC_ERR, // 29
  DEC_ERR, // 30
  DEC_ERR, // 31
  DEC_ERR, // 32
  DEC_ERR, // 33
  DEC_ERR, // 34
  DEC_ERR, // 35
  DEC_ERR, // 36
  DEC_ERR, // 37
  DEC_ERR, // 38
  DEC_ERR, // 39
  DEC_ERR, // 40
  DEC_ERR, // 41
  DEC_ERR, // 42
  62,      // 43 '+'
  DEC_ERR, // 44
  DEC_ERR, // 45
  DEC_ERR, // 46
  63,      // 47 '/'
  52,      // 48 '0'
  53,      // 49 '1'
  54,      // 50 '2'
  55,      // 51 '3'
  56,      // 52 '4'
  57,      // 53 '5'
  58,      // 54 '6'
  59,      // 55 '7'
  60,      // 56 '8'
  61,      // 57 '9'
  DEC_ERR, // 58
  DEC_ERR, // 59
  DEC_ERR, // 60
  DEC_ERR, // 61
  DEC_ERR, // 62
  DEC_ERR, // 63
  DEC_ERR, // 64
  0,       // 65 'A'
  1,       // 66 'B'
  2,       // 67 'C'
  3,       // 68 'D'
  4,       // 69 'E'
  5,       // 70 'F'
  6,       // 71 'G'
  7,       // 72 'H'
  8,       // 73 'I'
  9,       // 74 'J'
  10,      // 75 'K'
  11,      // 76 'L'
  12,      // 77 'N'
  13,      // 78 'M'
  14,      // 79 'O'
  15,      // 80 'P'
  16,      // 81 'Q'
  17,      // 82 'R'
  18,      // 83 'S'
  19,      // 84 'T'
  20,      // 85 'U'
  21,      // 86 'V'
  22,      // 87 'W'
  23,      // 88 'X'
  24,      // 89 'Y'
  25,      // 90 'Z'
  DEC_ERR, // 91
  DEC_ERR, // 92
  DEC_ERR, // 93
  DEC_ERR, // 94
  DEC_ERR, // 95
  DEC_ERR, // 96
  26,      // 97  'a'
  27,      // 98  'b'
  28,      // 99  'c'
  29,      // 100 'd'
  30,      // 101 'e'
  31,      // 102 'f'
  32,      // 103 'g'
  33,      // 104 'h'
  34,      // 105 'i'
  35,      // 106 'j'
  36,      // 107 'k'
  37,      // 108 'l'
  38,      // 109 'm'
  39,      // 110 'n'
  40,      // 111 'o'
  41,      // 112 'p'
  42,      // 113 'q'
  43,      // 114 'r'
  44,      // 115 's'
  45,      // 116 't'
  46,      // 117 'u'
  47,      // 118 'v'
  48,      // 119 'w'
  49,      // 120 'x'
  50,      // 121 'y'
  51,      // 122 'z'
  DEC_ERR, // 123
  DEC_ERR, // 124
  DEC_ERR, // 125
  DEC_ERR, // 126
  DEC_ERR, // 127
};

// returns length of given encoding.
// if the length found is 1 mod 4, it will return 0
unsigned base64CountValidEncodedChars(const char *ascii) {
  unsigned length;

  for(length = 0; ascii[length] <= 'z' && dec64[ascii[length]] != DEC_ERR; length++) {
    // do nothing, just getting the length
  }

  return length;
}

void base64Encode(
  const unsigned char *binary,  // [IN]       : 
  unsigned length,              // [IN]       : byte length
  char *ascii                   //      [OUT] : output buffer, length must be at
                                //            : least length*(4/3)
) {
  for(; length >= 3; length -= 3) {
    *ascii++ = enc64[ *binary >> 2 ];

    *ascii++ = enc64[ 0x3F & ((*binary << 4) | (*(binary+1) >> 4)) ];
    binary++;

    *ascii++ = enc64[ 0x3F & (*binary << 2 | (*(binary+1) >> 6))   ];
    binary++;

    *ascii++ = enc64[ 0x3F & *binary ];
    binary++;
  }

  if(length == 2) {
    *ascii++ = enc64[ *binary >> 2 ];

    *ascii++ = enc64[ 0x3F & ((*binary << 4) | (*(binary+1) >> 4)) ];
    binary++;

    *ascii++ = enc64[ 0x3F & (*binary << 2 )   ];
  } else if(length == 1) {
    *ascii++ = enc64[ *binary >> 2 ];

    *ascii++ = enc64[ 0x3F & (*binary << 4) ];
  }

  *ascii = '\0';
}

unsigned base64DecodeCString( //      [RET] : returns size of decoded data in bytes
  const char* base64,         // [IN]       : 
  ubyte* out                  //      [OUT] : output buffer, must be at
                              //            : least strlen(base64)*(3/4) bytes long
) {
  ubyte *outStart = out;
  ubyte temp1,temp2;

  while(*base64 != '\0' && (*base64 != '=')) {
    //printf("Decoding '%c%c%c%c'\n", *base64, *(base64+1), *(base64+2), *(base64+3));
    temp1 = dec64[*(base64+1)];

    *out++ = (dec64[*base64] << 2) | (temp1 >> 4);
    base64 += 2;
    if(*base64 == '\0' || *base64 == '=') break;

    temp2 = dec64[*base64];

    *out++ = (temp1 << 4) | (temp2 >> 2);
    base64++;
    if(*base64 == '\0' || *base64 == '=') break;

    *out++ = temp2 << 6 | dec64[*base64];
    base64++;
  }

  return out - outStart;
}
unsigned base64DecodeString(  //      [RET] : returns size of decoded data in bytes
  limstring base64,           // [IN]       : 
  ubyte* out                  //      [OUT] : output buffer, must be at
                              //            : least strlen(base64)*(3/4) bytes long
) {
  ubyte *outStart = out;
  ubyte temp1,temp2;

  while(base64.ptr < base64.lim && (*base64.ptr != '=')) {
    //printf("Decoding '%c%c%c%c'\n", *base64, *(base64+1), *(base64+2), *(base64+3));
    temp1 = dec64[*(base64.ptr+1)];

    *out++ = (dec64[*base64.ptr] << 2) | (temp1 >> 4);
    base64.ptr += 2;
    if(*base64.ptr == '\0' || *base64.ptr == '=') break;

    temp2 = dec64[*base64.ptr];

    *out++ = (temp1 << 4) | (temp2 >> 2);
    base64.ptr++;
    if(*base64.ptr == '\0' || *base64.ptr == '=') break;

    *out++ = temp2 << 6 | dec64[*base64.ptr];
    base64.ptr++;
  }

  return out - outStart;
}


