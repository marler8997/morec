#include <stdio.h>

// returns: On error returns 0 (if the string did not start with a valid hex character or
//          there was an odd number of hex digits) and on success returns the number of bytes
//          written to memOut.
// note: to get the number or hexCharacters processed use returnvalue * 2
// 
size_t hextoarray(const char *hexString, unsigned char *memOut)
{
  unsigned char *memOutStart = memOut;

  while(1) {
    char c = *hexString;

    if(c >= '0' && c <= '9') {
      *memOut = (c - '0') << 4;
    } else if (c >= 'A' && c < 'F') {
      *memOut = (c - 'A' + 10) << 4;
    } else if (c >= 'a' && c <= 'f') {
      *memOut = (c - 'a' + 10) << 4;
    } else {
      return memOut - memOutStart;
    }

    hexString++;
    c = *hexString;

    if(c >= '0' && c <= '9') {
      *memOut += (c - '0');
    } else if (c >= 'A' && c < 'F') {
      *memOut += (c - 'A' + 10);
    } else if (c >= 'a' && c <= 'f') {
      *memOut += (c - 'a' + 10);
    } else {
      // Hex must have even # of digits
      return 0;
    }

    memOut++;
    hexString++;
  }
}
