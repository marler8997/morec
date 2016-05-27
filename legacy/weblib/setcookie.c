#include <stdio.h>
#include <stdlib.h>

int main()
{
  printf("Content-Type: text/plain\r\n");

  char * query = getenv("QUERY_STRING");

  if(query==NULL || query[0] == '\0') goto NO_COOKIE;
  printf("Set-Cookie: %s\r\n", query);
  printf("\r\n");
  printf("The following cookie '%s' has been set\r\n", query);

  return 0;

 NO_COOKIE:
  printf("\r\n");
  printf("No cookie was set.\r\n");
  printf("If you are in a browser, try adding '?cookie1=value;cookie2=value' to the end of your Url.\r\n");

  return 1;
}
