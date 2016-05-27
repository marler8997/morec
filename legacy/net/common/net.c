#include <stdio.h>

#include "net.h"

// Function for calculating the checksum
unsigned short checksum(unsigned short *buffer, unsigned int uint2Length)
{
  unsigned long sum;
  for(sum = 0; uint2Length > 0; uint2Length--) {
    sum += *buffer++;
  }
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  return (unsigned short)(~sum);
}
