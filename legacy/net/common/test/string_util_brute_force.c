#include <stdio.h>
#include <stdlib.h>

#include "string_util.h"

int main() {
  unsigned int ip;
  int ip1, ip2, ip3, ip4;

  char ipString[16];
  int ip1StringOffset, ip2StringOffset, ip1Plus2StringOffset, ip1Plus2Plus3StringOffset;

  printf("Brute Force Testing All IP Addresses...\n");
  for(ip1 = 0; ip1 <= 0xFF; ip1++) {
    printf("%u.*.*.*\n", (unsigned char)ip1);
    ip1StringOffset = sprintf(ipString, "%u.", (unsigned char)ip1);

    for(ip2 = 0; ip2 <= 0xFF; ip2++) {
      printf("%u.%u.*.*\n", (unsigned char)ip1, (unsigned char)ip2);
      ip2StringOffset = sprintf(ipString + ip1StringOffset, "%u.", (unsigned char)ip2);
      ip1Plus2StringOffset = ip1StringOffset + ip2StringOffset;

      for(ip3 = 0; ip3 < 0xFF; ip3++) {
	ip1Plus2Plus3StringOffset = ip1Plus2StringOffset + sprintf(ipString + ip1Plus2StringOffset, "%u.", (unsigned char)ip3);

	for(ip4 = 0; ip4 <= 0xFF; ip4++) {
	  sprintf(ipString + ip1Plus2Plus3StringOffset, "%u", (unsigned char)ip4);

	  if(str2ip(ipString, &ip) == NULL) {
	    printf("str2ip('%s') returned error\n", ipString);
	    return -1;
	  }
	  if(
	     ((unsigned char)(ip >> 24) != ip1) ||
	     ((unsigned char)(ip >> 16) != ip2) ||
	     ((unsigned char)(ip >>  8) != ip3) ||
	     ((unsigned char) ip        != ip4)) {
	    printf("Failed. Expected '%u.%u.%u.%u' but got '%u.%u.%u.%u'\n", 
		   (unsigned char) ip1      , (unsigned char) ip2      , (unsigned char) ip3      , (unsigned char)ip4,
		   (unsigned char)(ip >> 24), (unsigned char)(ip >> 16), (unsigned char)(ip >>  8), (unsigned char)ip );
	    return -1;
	  }
	}
      }
    }
  }

  return 0;
}

