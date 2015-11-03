#ifndef MORE_BASE_64_H
#define MORE_BASE_64_H

#include <more/common.h>

unsigned base64CountValidEncodedChars(const char *ascii);

void base64Encode(
  const unsigned char *binary,  // [IN]       : 
  unsigned length,              // [IN]       : byte length
  char *ascii                   //      [OUT] : output buffer, length must be at
                                //            : least length*(4/3)
);

unsigned base64DecodeCString( //      [RET] : returns size of decoded data in bytes
  const char* base64,         // [IN]       : 
  ubyte* out                  //      [OUT] : output buffer, must be at
                              //            : least strlen(ascii)*(3/4) bytes long
);
unsigned base64DecodeString( //      [RET] : returns size of decoded data in bytes
  more_limstring base64,     // [IN]       : 
  ubyte* out                 //      [OUT] : output buffer, must be at
                             //            : least strlen(ascii)*(3/4) bytes long
);

#endif
