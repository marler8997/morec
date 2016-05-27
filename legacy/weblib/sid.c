#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define DEBUG

#include "sha1.h"

int generateSid(uint32_t *hash)
{
  SHA1 sha;
  char *str;

  sha1Init(&sha);

  //
  // Insert 20 random bytes
  //
  srand(time(NULL));
  for(int i = 0; i < 5; i++) {
    int a = rand();
    sha1AddBinary(&sha, (const unsigned char*)&a,sizeof(int));
  }

  //
  // Insert The clients IP Address
  //
  str = getenv("REMOTE_ADDR");
  if(str == NULL) {
    return -1;
  } else {
    if(sha1AddString(&sha, str)) return -1;
  }

  sha1Finish(&sha);

  hash[0] = sha.hash[0];
  hash[1] = sha.hash[1];
  hash[2] = sha.hash[2];
  hash[3] = sha.hash[3];
  hash[4] = sha.hash[4];

  return 0;
}


