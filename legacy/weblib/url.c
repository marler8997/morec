#include <stdio.h>
#include <stdlib.h>


/* Converts a hex character to its integer value */
static inline char char2code(char c) {
  return ((c >= '0' && c <= '9') ? c - '0' : c + 10 - ((c >= 'a') ? 'a' : 'A'));
}

/* Decodes the given url in memory */
int urlDecode(char *str) {
  char *strPaste = str;

  while (*str) {
    if (*str == '%') {
      if(str[1] == '\0' || str[2] == '\0') return -1; // ERROR
      *strPaste = char2code(str[1]) << 4 | char2code(str[2]);
      strPaste++;
      str += 3;
    } else {

      if (*str == '+') { 
	*strPaste = ' ';
      } else {
	*strPaste = *str;
      }
      strPaste++;
      str++;
    }
  }
  *strPaste = '\0';
  return 0;
}

#define code2char(c) "0123456789abcdef"[c]

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
/*
char *url_encode(char *str) {
  char *pstr = str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
      *pbuf++ = *pstr;
    else if (*pstr == ' ') 
      *pbuf++ = '+';
    else 
      *pbuf++ = '%', *pbuf++ = code2char(*pstr >> 4), *pbuf++ = code2char(*pstr & 15);
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

*/
