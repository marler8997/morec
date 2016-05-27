#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

char* str2ip(const char *str, unsigned int *ip);
char * str2mac(const char *str, unsigned char *mac_out);
void hashBytes2Hex(const unsigned char *bytes, char *hex);

#endif
