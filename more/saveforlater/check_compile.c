#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
  char buffer256[256];

  // This file contains tests that must pass in order to verify
  // that the compile worked on your platform



  //
  // Check Standard Types
  //
  if(sizeof(int) != 4) {
    fprintf(stderr, "Expected 'int' to be %u, but it was %u\n",
	    4, sizeof(unsigned long));
    return -1;
  }
  if(sizeof(long) != 8) {
    fprintf(stderr, "Expected 'long' to be %u, but it was %u\n",
	    8, sizeof(long));
    return -1;
  }
  if(sizeof(long long) != 8) {
    fprintf(stderr, "Expected 'long long' to be %u, but it was %u\n",
	    8, sizeof(long long));
    return -1;
  }


  //
  //
  //
  sprintf(buffer256, "%lu", UINT64_MAX);
  unsigned long value = strtoul(buffer256, NULL, 10);
  if(value != UINT64_MAX) {
    fprintf(stderr, "strtoul failed, expected %lu, but got %lu\n",
	    UINT64_MAX, value);
    return -1;
  }


  return 0;
}
