#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "url.h"

int main()
{
  int i;

  char *TEST[] = {
    "person%40gmail.com", "person@gmail.com",
    "%00", "\0",
    "%0A", "\n",
    "%0a", "\n",
    NULL};

  // Create a mutable copy of the test strings

  for(i = 0; TEST[i]; i+=2);

  char **t = (char**)malloc((sizeof(char*) * i) + 1);
  if(t == NULL) {
    printf("FAILED, out of memory\n");
    return -1;
  }

  t[i] = NULL;
  i--;
  while(i >= 0) {
    t[i] = (char*)malloc(strlen(TEST[i]));
    if(t[i] == NULL) {
      printf("FAILED, out of memory\n");
      return -1;
    }
    strcpy(t[i], TEST[i]);
    i--;
  }

  printf("Starting Tests\n");

  for(i = 0; t[i] && t[i+1]; i+= 2) {

    printf("Test %d: '%s' to '%s'\n", i/2, t[i], t[i+1]);
    if(urlDecode(t[i])) {
      printf("Failed,  urlDecide returned error\n");
    }
    if(strcmp(t[i], t[i+1])) {
      printf("Failed, expected '%s', but got '%s'\n", t[i+1], t[i]);
      return -1;
    }
    
  }


  printf("Success\n");
  return 0;
}
