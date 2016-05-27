#include <stdio.h>

char * str2ip(const char *str, unsigned int *ip_out)
{
  unsigned char b;
  int shift = 24;
  unsigned int ip = 0;

  if(str == NULL) return "String was a NULL pointer";

  while(shift > 0) {

    // 1st digit
    if(*str < '0' || *str > '9') return NULL;
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

    if(*str != '.') return NULL;
    ip |= ((unsigned int)b) << shift;
    str++;

    shift -= 8;
  }
  
  // 1st digit
  if(*str < '0' || *str > '9') return NULL;
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

  *ip_out = ip | ((unsigned int)b);
  return (char*)str;
}

char * str2mac(const char *str, unsigned char *mac_out)
{
  unsigned char b;
  if(str == NULL) return NULL;

  int i = 0;
  while(1) {
    b = *str++;
    if(b == '\0') return NULL;

    if(b >= '0' && b <= '9') {
      *mac_out = b - '0';
    } else if(b >= 'A' && b <= 'Z') {
      *mac_out = b - 'A' + 10;
    } else if(b >= 'a' && b <= 'z') {
      *mac_out = b - 'a' + 10;
    } else {
      return NULL;
    }
    *mac_out = *mac_out << 4;

    b = *str++;
    if(b >= '0' && b <= '9') {
      *mac_out += b - '0';
    } else if(b >= 'A' && b <= 'Z') {
      *mac_out += b - 'A' + 10;
    } else if(b >= 'a' && b <= 'z') {
      *mac_out += b - 'a' + 10;
    } else {
      return NULL;
    }

    if(i >= 5) {
      return (char*)str;
    }
    mac_out++;
    b = *str++;
    if(b == '\0') return NULL;
    i++;
  }
}


void hashBytes2Hex(const unsigned char *bytes, char *hex)
{
  int i;
  const char hexChar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  for(i = 0; i < 40; i++) {
    hex[2*i    ] = hexChar[ bytes[i] >>   4 ];
    hex[2*i + 1] = hexChar[ bytes[i] &  0xF ];
  }
  hex[2*40] = '\0';
}

