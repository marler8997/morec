#include <stdio.h>

#include "url.h"

int main(int argc, char *argv[])
{
  if(argc > 2) {
    fprintf(stderr, "ERROR: only one argument is accepted, you gave %d\n", argc - 1);
    return -1;
  }
  urlDecode(argv[1]);
  printf("%s", argv[1]);
}
