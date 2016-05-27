#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

int str2ip(char *str, uint32_t *ip);
int printFile(char *filename);
void hashBytes2Hex(const unsigned char *bytes, char *hex);

// create atoi for uint64_t
// returns 0 on success, 1 otherwise
//int str2uint64_t(const char *str, uint64_t *out);
uint64_t str2uint64_t(const char *str);

#endif
