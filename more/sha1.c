//   Sha1 Overview
//   
//   According to the standard, the message must be padded to an even
//   512 bits (64 bytes).  The first padding bit must be a 1 (byte must be 0x80).  
//   The last 64 bits (2 bytes) represent the length of the original message.
//   All bits in between the first pad bit and the message size should be 0.
//
#define using_more

#include <more/sha1.h>

//
// TODO: Utilize the processor's circular shift operator
//
#define circularShift(bits,word)					\
  ( ( ((word) << (bits)) & 0xFFFFFFFF) | ((word) >> (32-(bits))))

#define initFields(hash, blockIndex, messageBitLength)	\
  blockIndex  = 0;						\
  messageBitLength   = 0;					\
  hash[0]   = 0x67452301;					\
  hash[1]   = 0xEFCDAB89;					\
  hash[2]   = 0x98BADCFE;					\
  hash[3]   = 0x10325476;					\
  hash[4]   = 0xC3D2E1F0;

#define pad(block, blockIndex, hash)				\
  block[blockIndex++] = 0x80;						\
  if (blockIndex > 56) {						\
    while(blockIndex < 64) block[blockIndex++] = 0;			\
    hashBlock(block, hash);						\
    blockIndex = 0;							\
  }									\
  while(blockIndex < 56) block[blockIndex++] = 0;

#define appendLength(block, messageBitLength)	\
  block[56] = (messageBitLength >> 56);			\
  block[57] = (messageBitLength >> 48);			\
  block[58] = (messageBitLength >> 40);			\
  block[59] = (messageBitLength >> 32);			\
  block[60] = (messageBitLength >> 24);			\
  block[61] = (messageBitLength >> 16);			\
  block[62] = (messageBitLength >>  8);			\
  block[63] = (messageBitLength      );

static void hashBlock(const unsigned char *block, uint32 *hash);

// Returns Non-Zero if match, 0 otherwise
int Sha1_Equal(uint32 *a, uint32 *b) {
  return
    a[0] == b[0] &&
    a[1] == b[1] &&
    a[2] == b[2] &&
    a[3] == b[3] &&
    a[4] == b[4];
}

// Performs sha1 of null-terminated input string and stores it in hash
int Sha1_String(    //      [RET] : 0 = Success
  const char *msg,  // [IN]       : Null-terminated string to hash
  uint32 *hash      //      [OUT] : Output buffer (At least SHA1_HASH_UINT32_LENGTH (5) uint32's long)
) {
  uint64 messageBitLength;                     // Message length in bits
  unsigned char block[SHA1_BLOCK_BYTE_LENGTH]; // 512-bit message block
  byte blockIndex;                             // Index into msbBlock

  initFields(hash, blockIndex, messageBitLength);

  // Perform the hash
  for(; *msg ; msg++) {
    block[blockIndex++] = *msg;
    
    messageBitLength += 8;

    if(messageBitLength <= 0) return -2; // message exceeded 2^64 bytes
    
    if (blockIndex == SHA1_BLOCK_BYTE_LENGTH) {
      hashBlock(block, hash);
      blockIndex = 0;
    }
  }

  // Finish hash
  pad(block, blockIndex, hash);
  appendLength(block, messageBitLength);
  hashBlock(block, hash);

  return 0;
}

// Performs sha1 of input buffer and stores it in hash
int Sha1_Binary(            //      [RET] : 0 = Success
  const unsigned char *msg, // [IN]       : Null-terminated string to hash
  unsigned length,          // [IN]       : lenth of input 
  uint32 *hash              //      [OUT] : Output buffer (At least HASH_UINT32_LENGTH (5) uint32's long)
) {
  uint64 messageBitLength;                     // Message length in bits
  unsigned char block[SHA1_BLOCK_BYTE_LENGTH]; // 512-bit message block
  byte blockIndex;                             // Index into msbBlock

  initFields(hash, blockIndex, messageBitLength);

  // Perform the hash
  for(; length > 0 ; msg++) {
    block[blockIndex++] = *msg;
    
    messageBitLength += 8;

    if(messageBitLength <= 0) return -2; // message exceeded 2^64 bytes
    
    if (blockIndex == SHA1_BLOCK_BYTE_LENGTH) {
      hashBlock(block, hash);
      blockIndex = 0;
    }
    
    length--;
  }

  // Finish hash
  pad(block, blockIndex, hash);
  appendLength(block, messageBitLength);
  hashBlock(block, hash);

  return 0;
}

// Initialize or re-initialize a SHA1 structure
// NOTE: handle must point to a valid SHA1 already allocated
void Sha1_Init(Sha1* handle)
{
  initFields(handle->hash, handle->blockIndex, handle->messageBitLength);
}

// Finishes the hash and stores the result in the hash array.
// Returns 0 on success
void Sha1_Finish(Sha1* handle)
{
  //
  // Cache highly used values from handle
  //
  unsigned char *block = handle->block;
  byte blockIndex = handle->blockIndex;
  uint64 messageBitLength = handle->messageBitLength;

  if(blockIndex >= 0) {
    pad(block, blockIndex, handle->hash);
    appendLength(block, messageBitLength);
    hashBlock(block, handle->hash);
    handle->blockIndex = -1; // flag to say the hash is done
  }
}

// Accepts an array of octets as the next portion of the message.
// Returns 0 on success
int Sha1_AddString(Sha1* handle, const char *msg)
{
  // Cache highly used values from handle
  unsigned char *block = handle->block;
  byte blockIndex = handle->blockIndex;
  uint64 messageBitLength = handle->messageBitLength;

  if(blockIndex < 0) return -1;

  for(;*msg; msg++) {
    block[blockIndex++] = *msg;
    messageBitLength += 8;

    if(messageBitLength <= 0) return -2; // message exceeded 2^64 bytes
    
    if (blockIndex == SHA1_BLOCK_BYTE_LENGTH) {
      hashBlock(block, handle->hash);
      blockIndex = 0;
    }
  }

  // Store cached values
  handle->blockIndex = blockIndex;
  handle->messageBitLength = messageBitLength;

  return 0;
}

// Accepts an array of octets as the next portion of the message.
// Returns 0 on success
int Sha1_AddBinary(Sha1* handle, const unsigned char *msg, unsigned int length)
{
  // Cache highly used values from handle
  unsigned char *block = handle->block;
  byte blockIndex = handle->blockIndex;
  uint64 messageBitLength = handle->messageBitLength;

  if(blockIndex < 0) return -1;

  for(; length > 0; msg++) {
    block[blockIndex++] = *msg;
    messageBitLength += 8;

    if(messageBitLength <= 0) return -2; // message exceeded 2^64 bytes
    
    if (blockIndex == SHA1_BLOCK_BYTE_LENGTH) {
      hashBlock(block, handle->hash);
      blockIndex = 0;
    }
    
    length--;
  }

  // Store cached values
  handle->blockIndex = blockIndex;
  handle->messageBitLength = messageBitLength;

  return 0;
}

// Process the SHA1_BLOCK_BYTE_LENGTH (64) byte block and store in the hash
static void hashBlock(const unsigned char *block, uint32 *hash)
{
  static const uint32 K[] = {      /* Constants defined in SHA-1   */      
    0x5A827999,
    0x6ED9EBA1,
    0x8F1BBCDC,
    0xCA62C1D6
  };

  int i;
  uint32 temp32;
  char temp8;

  uint32    W[80];              /* Word sequence                */
  uint32    A, B, C, D, E;      /* Word buffers                 */

  // Initialize the first 16 words in array W
  for(i = 0; i < 16; i++) {
    temp8 = i << 2; 
    W[i]  = 
      (((uint32) block[temp8    ]) << 24) |
      (((uint32) block[temp8 + 1]) << 16) |
      (((uint32) block[temp8 + 2]) <<  8) |
      (((uint32) block[temp8 + 3])      ) ;
  }

  // Initialize the rest of the words in array W
  for(i = 16; i < 80; i++) {
    W[i] = circularShift(1,W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]);
  }

  // Cache current hash
  A = hash[0];
  B = hash[1];
  C = hash[2];
  D = hash[3];
  E = hash[4];

  for(i = 0; i < 20; i++) {
    temp32 =  circularShift(5,A) +
      ((B & C) | ((~B) & D)) + E + W[i] + K[0];
    E = D;
    D = C;
    C = circularShift(30,B);
    B = A;
    A = temp32;
  }

  for(i = 20; i < 40; i++) {
    temp32 = circularShift(5,A) + (B ^ C ^ D) + E + W[i] + K[1];
    E = D;
    D = C;
    C = circularShift(30,B);
    B = A;
    A = temp32;
  }

  for(i = 40; i < 60; i++) {
    temp32 = circularShift(5,A) +
      ((B & C) | (B & D) | (C & D)) + E + W[i] + K[2];
    E = D;
    D = C;
    C = circularShift(30,B);
    B = A;
    A = temp32;
  }

  for(i = 60; i < 80; i++) {
    temp32 = circularShift(5,A) + (B ^ C ^ D) + E + W[i] + K[3];
    E = D;
    D = C;
    C = circularShift(30,B);
    B = A;
    A = temp32;
  }

  hash[0] += A;
  hash[1] += B;
  hash[2] += C;
  hash[3] += D;
  hash[4] += E;
}


#if defined(more_test) || defined(more_sha1_test)
#include <stdio.h>
#include <string.h>
void printHash(uint32 *hash) {
  printf("%08X %08X %08X %08X %08X\n",hash[0],hash[1],hash[2],hash[3],hash[4]);
}

int testSingleCall(Sha1 *sha, char *str, uint32 *expected) {
  uint32 hash[SHA1_HASH_UINT32_LENGTH];

  printf("Test: '%s'\n", str);
  
  // Test Single Function Call (c-string)
  if(Sha1_String(str, hash)) {
    printf("sha1 returned error\n");
    return -1;
  }
  if(!Sha1_Equal(hash, expected)) {
    printf("sha1 call did not match\n");
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(hash);
    return -1;
  }

  // Test Single Function Call (binary)
  if(Sha1_Binary(str, strlen(str), hash)) {
    printf("sha1 returned error\n");
    return -1;
  }
  if(!Sha1_Equal(hash, expected)) {
    printf("sha1 call did not match\n");
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(hash);
    return -1;
  }

  // Test Multi-Function Call (c-string)
  Sha1_Init(sha);
  Sha1_AddString(sha, str);
  Sha1_Finish(sha);
  if(!Sha1_Equal(sha->hash, expected)) {
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(sha->hash);
    return -1;
  }

  // Test Multi-Function Call (binary)
  Sha1_Init(sha);
  Sha1_AddBinary(sha, str, strlen(str));
  Sha1_Finish(sha);
  if(!Sha1_Equal(sha->hash, expected)) {
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(sha->hash);
    return -1;
  }

  return 0;
}

int testMultipleCall(Sha1 *sha, char *strings[], uint32 *expected) {
  uint32 hash[SHA1_HASH_UINT32_LENGTH];

  printf("Test:\n");

  for(int i = 0; strings[i]; i++) {
    printf("  [%u] '%s'\n", i, strings[i]);
  }

  // Test Multi-Function Call (c-string)
  Sha1_Init(sha);
  for(int i = 0; strings[i]; i++) {
    Sha1_AddString(sha, strings[i]);
  }
  Sha1_Finish(sha);
  if(!Sha1_Equal(sha->hash, expected)) {
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(sha->hash);
    return -1;
  }

  // Test Multi-Function Call (binary)
  Sha1_Init(sha);
  for(int i = 0; strings[i]; i++) {
    Sha1_AddBinary(sha, strings[i], strlen(strings[i]));
  }
  Sha1_Finish(sha);
  if(!Sha1_Equal(sha->hash, expected)) {
    printf("Expected: ");
    printHash(expected);
    printf("Actual  : ");
    printHash(sha->hash);
    return -1;
  }

  return 0;
}

bool test_more_sha1()
{
  int i;
  Sha1 sha;

  char *abc[] = {"a","b","c", NULL};
  char *l[] = {"abcdbcdecd","efdefgefghfghighij","hijkijkljkl",
    "m","kl","mnlmnomnopnopq",NULL};

  uint32 abcExpected[] = {
    0xA9993E36,
    0x4706816A,
    0xBA3E2571,
    0x7850C26C,
    0x9CD0D89D};
  uint32 longExpected[] = {
    0x84983E44,
    0x1C3BD26E,
    0xBAAE4AA1,
    0xF95129E5,
    0xE54670F1};

  if(testSingleCall(&sha, "abc", abcExpected)) {
    fprintf(stderr, "Failed\n");
    return false;
  }

  if(testSingleCall(&sha, "abcdbcdecdefdefgefghfghighij" "hijkijkljklmklmnlmnomnopnopq",
	  longExpected)) {
    fprintf(stderr, "Failed\n");
    return false;
  }

  if(testMultipleCall(&sha, abc, abcExpected)) {
    fprintf(stderr, "Failed\n");
    return false;
  }

  if(testMultipleCall(&sha, l, longExpected)) {
    fprintf(stderr, "Failed\n");
    return false;
  }

  printf("Success\n");
  return true;
}
#endif
