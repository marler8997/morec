#ifndef MORE_SHA1_H
#define MORE_SHA1_H

#include <more/common.h>

#if defined(using_more) || defined(using_more_sha1)
#define Sha1           more_Sha1
#define Sha1_Equal     more_Sha1_Equal
#define Sha1_String    more_Sha1_String
#define Sha1_Binary    more_Sha1_Binary
#define Sha1_Init      more_Sha1_Init
#define Sha1_Finish    more_Sha1_Finish
#define Sha1_AddString more_Sha1_AddString
#define Sha1_AddBinary more_Sha1_AddBinary
#endif

#if defined(more_test) || defined(more_sha1_test)
bool test_more_sha1();
#endif


#define SHA1_HASH_UINT32_LENGTH  5
#define SHA1_HASH_BYTE_LENGTH   20
#define SHA1_HASH_BASE64_LENGTH 27

#define SHA1_BLOCK_BYTE_LENGTH 64

typedef struct
{
  uint32 hash[SHA1_HASH_UINT32_LENGTH]; // Message Hash
  uint64 messageBitLength;              // Message length in bits
  ubyte block[SHA1_BLOCK_BYTE_LENGTH];  // 512 bit (64 byte) message block
  byte blockIndex;                      // Current block index (A value of -1 implies
                                        // the hash is finished)
} more_Sha1;

int more_Sha1_Equal(uint32 *a, uint32 *b);

// Hash a single null-terminated string
int more_Sha1_String(const char *msg, uint32 *hash);
// Has a single buffer, with a given length
int more_Sha1_Binary(const unsigned char *msg, unsigned length, uint32 *hash);

// Initialize SHA1 structure to hash multiple buffers together
void more_Sha1_Init(more_Sha1 *);
// Finish hash of multiple buffers
void more_Sha1_Finish(more_Sha1 *);
// Add null-terminated string to hash
int more_Sha1_AddString(more_Sha1 *,const char*);
// Add buffer with given length to null-terminated string
int more_Sha1_AddBinary(more_Sha1 *,const unsigned char*,unsigned);

#endif
