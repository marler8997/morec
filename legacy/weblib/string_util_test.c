#include <stdio.h>
#include <stdlib.h>

#include "string_util.h"

int bruteForceStr2IPTest() {
  uint32_t ip;
  char ipString[16];

  printf("Brute Force Testing All IP Addresses...\n");
  for(int ip1 = 0; ip1 <= 0xFF; ip1++) {
    printf("%u.*.*.*\n", (unsigned char)ip1);
    for(int ip2 = 0; ip2 <= 0xFF; ip2++) {
      printf("%u.%u.*.*\n", (unsigned char)ip1, (unsigned char)ip2);
      for(int ip3 = 0; ip3 < 0xFF; ip3++) {
	for(int ip4 = 0; ip4 <= 0xFF; ip4++) {

	  sprintf(ipString,"%u.%u.%u.%u", ip1, ip2, ip3, ip4);
	  //printf("testing '%s'\n", ipString);

	  if(str2ip(ipString, &ip)) {
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


int main()
{
  uint32_t u;

  printf("test str2ip\n");

  struct testStruct {char *str;unsigned char ip1,ip2,ip3,ip4;} t[] = {
    "0.0.0.0", 0,0,0,0,
    "1.0.0.0", 1,0,0,0,
    "9.0.0.0", 9,0,0,0,

    "00.0.0.0", 0,0,0,0,
    "11.0.0.0", 11,0,0,0,
    "255.0.0.0", 255,0,0,0,
    "1.1.1.1", 1,1,1,1,
    "127.0.0.1", 127,0,0,1,
    "100.0.70.44", 100,0,70,44,
    "200.201.168.99", 200,201,168,99,
    "192.168.0.1", 192,168,0,1,
    "255.255.255.255", 255,255,255,255,

    NULL
  };

  for(int i = 0; t[i].str; i++) {
    printf("testing '%s'\n", t[i].str);
    if(str2ip(t[i].str, &u)) {
      printf("str2ip returned error\n");
      return -1;
    }
    if(
       ((unsigned char)(u >> 24) != t[i].ip1) ||
       ((unsigned char)(u >> 16) != t[i].ip2) ||
       ((unsigned char)(u >>  8) != t[i].ip3) ||
       ((unsigned char) u        != t[i].ip4)) {
      printf("Failed. Expected '%u.%u.%u.%u' but got '%u.%u.%u.%u'\n", 
	     (unsigned char) t[i].ip1, (unsigned char) t[i].ip2, (unsigned char) t[i].ip3, (unsigned char)t[i].ip4,
	     (unsigned char)(u >> 24), (unsigned char)(u >> 16), (unsigned char)(u >>  8), (unsigned char)u       );
      return -1;
    }
  }

  //if(bruteForceStr2IPTest()) {
  //return -1;
  //}

  printf("Success\n");
  return 0;
}


