#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hex.h"

//
// Return Value: NULL on success, error message on error
//
const char *parseHex(const char *hexString, unsigned char **outBuffer, size_t *outLength)
{
  size_t searchLength = strlen(hexString);
  if(searchLength % 2) return "the string did not have even number of digits";

  searchLength = searchLength / 2;
  *outBuffer = malloc(searchLength);
  if(*outBuffer == NULL) return "malloc returned NULL";

  unsigned int ret = hextoarray(hexString, *outBuffer);
  if(ret != searchLength) {
    free(*outBuffer);
    return "the string contained invalid characters";
  }

  *outLength = searchLength;
  return NULL;
}
