#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "sha1.h"

int str2ip(char *str, uint32_t *ip_out)
{
  unsigned char b;
  int shift = 24;
  uint32_t ip = 0;

  if(str == NULL) return -1;

  while(shift > 0) {

    // 1st digit
    if(*str < '0' || *str > '9') return -1;
    b = *str - '0';
    str++;

    // 2nd digit
    if(*str >= '0' && *str <= '9') {
      b *= 10;
      b += *str - '0';
      str++;

      //3rd digit
      if(*str >= '0' && *str <= '9') {
	b *= 10;
	b += *str - '0';
	str++;
      }
    }

    if(*str != '.') return -1;
    ip |= ((uint32_t)b) << shift;
    str++;

    shift -= 8;
  }
  
  // 1st digit
  if(*str < '0' || *str > '9') return -1;
  b = *str - '0';
  str++;
    
  // 2nd digit
  if(*str >= '0' && *str <= '9') {
    b *= 10;
    b += *str - '0';
    str++;
    
    //3rd digit
    if(*str >= '0' && *str <= '9') {
      b *= 10;
      b += *str - '0';
    }

  }

  *ip_out = ip | ((uint32_t)b);
  return 0;
}

void hashBytes2Hex(const unsigned char *bytes, char *hex)
{
  const char hexChar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  for(int i = 0; i < SHA1_HASH_BYTE_LENGTH; i++) {
    hex[2*i    ] = hexChar[ bytes[i] >>   4 ];
    hex[2*i + 1] = hexChar[ bytes[i] &  0xF ];
  }
  hex[2* SHA1_HASH_BYTE_LENGTH] = '\0';
}

