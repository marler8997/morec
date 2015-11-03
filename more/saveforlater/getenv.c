#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char **envp)
{
  printf("Content-Type: text/plain\r\n");
  printf("\r\n");
  
  unsigned int count;
  for(count = 0; envp[count]; count++);

  printf("%d Environment Variables:\r\n", count);
  for(unsigned int i = 0; i < count; i++) {
    printf("[%d] %s\r\n", i, envp[i]);
  }

  return 0;
}
