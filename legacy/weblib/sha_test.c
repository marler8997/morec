#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "sha1.h"

void printHash(uint32_t *hash) {
  printf("%08X %08X %08X %08X %08X\n",hash[0],hash[1],hash[2],hash[3],hash[4]);
}

int testSingleCall(SHA1 *sha, char *str, uint32_t *expected) {
  uint32_t hash[SHA1_HASH_UINT32_LENGTH];

  printf("Test: '%s'\n", str);
  
  // Test Single Function Call (c-string)
  if(sha1String(str, hash)) {
    printf("sha1 returned error\n");
    return -1;
  }
  if(!sha1Equal(hash, expected)) {
    printf("sha1 call did not match\n");
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(hash);
    return -1;
  }

  // Test Single Function Call (binary)
  if(sha1Binary(str, strlen(str), hash)) {
    printf("sha1 returned error\n");
    return -1;
  }
  if(!sha1Equal(hash, expected)) {
    printf("sha1 call did not match\n");
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(hash);
    return -1;
  }

  // Test Multi-Function Call (c-string)
  sha1Init(sha);
  sha1AddString(sha, str);
  sha1Finish(sha);
  if(!sha1Equal(sha->hash, expected)) {
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(sha->hash);
    return -1;
  }

  // Test Multi-Function Call (binary)
  sha1Init(sha);
  sha1AddBinary(sha, str, strlen(str));
  sha1Finish(sha);
  if(!sha1Equal(sha->hash, expected)) {
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(sha->hash);
    return -1;
  }

  return 0;
}

int testMultipleCall(SHA1 *sha, char *strings[], uint32_t *expected) {
  uint32_t hash[SHA1_HASH_UINT32_LENGTH];

  printf("Test:\n");

  for(int i = 0; strings[i]; i++) {
    printf("  [%u] '%s'\n", i, strings[i]);
  }

  // Test Multi-Function Call (c-string)
  sha1Init(sha);
  for(int i = 0; strings[i]; i++) {
    sha1AddString(sha, strings[i]);
  }
  sha1Finish(sha);
  if(!sha1Equal(sha->hash, expected)) {
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(sha->hash);
    return -1;
  }

  // Test Multi-Function Call (binary)
  sha1Init(sha);
  for(int i = 0; strings[i]; i++) {
    sha1AddBinary(sha, strings[i], strlen(strings[i]));
  }
  sha1Finish(sha);
  if(!sha1Equal(sha->hash, expected)) {
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(sha->hash);
    return -1;
  }

  return 0;
}

int main()
{
  int i;
  SHA1 sha;

  char *abc[] = {"a","b","c", NULL};
  char *l[] = {"abcdbcdecd","efdefgefghfghighij","hijkijkljkl",
    "m","kl","mnlmnomnopnopq",NULL};

  uint32_t abcExpected[] = {
    0xA9993E36,
    0x4706816A,
    0xBA3E2571,
    0x7850C26C,
    0x9CD0D89D};
  uint32_t longExpected[] = {
    0x84983E44,
    0x1C3BD26E,
    0xBAAE4AA1,
    0xF95129E5,
    0xE54670F1};

  if(testSingleCall(&sha, "abc", abcExpected)) {
    fprintf(stderr, "Failed\n");
    return -1;
  }

  if(testSingleCall(&sha, "abcdbcdecdefdefgefghfghighij" "hijkijkljklmklmnlmnomnopnopq",
	  longExpected)) {
    fprintf(stderr, "Failed\n");
    return -1;
  }

  if(testMultipleCall(&sha, abc, abcExpected)) {
    fprintf(stderr, "Failed\n");
    return -1;
  }

  if(testMultipleCall(&sha, l, longExpected)) {
    fprintf(stderr, "Failed\n");
    return -1;
  }

  printf("Success\n");
  return 0;
}
