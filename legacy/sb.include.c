/*
 * Summary:
 *    To use this file, add the following include:
 *       #include "string-builder.include.c"
 *
 *    All functions that use the stringBuilderAdd or stringBuilderPtrAdd macros must define
 *    the label STRING_BUILDER_ADD_FAILED to set a place in the function to jump to if the
 *    add fails becuase the stringbuilder could not allocate more memory.
 *
 * Options:
 *    You may define STRING_BUILDER_EXTEND_LENGTH to be the length used to extend a string
 *    builder when it runs out of memory.  The default is 64.
 *
 */

#ifndef STRING_BUILDER_INCLUDE_C
#define STRING_BUIDLER_INCLUDE_C

#include <stdlib.h>

#include "sb.h"

#ifndef STRING_BUILDER_EXTEND_LENGTH
#define STRING_BUILDER_EXTEND_LENGTH 64
#endif

#define sbIsEmpty(b) (b.off == 0)

#define sbRemove(b) if(b.off > 0) b.off--

#define sbAdd(b, c)							\
  if(b.off >= b.len) {						\
    b.mem = realloc(b.mem, b.len + STRING_BUILDER_EXTEND_LENGTH);	\
    if(b.mem == NULL) goto STRING_BUILDER_ADD_FAILED;		        \
    b.len = b.len + STRING_BUILDER_EXTEND_LENGTH;			\
  }									\
  b.mem[(b.off)++] = c


#define sbpAdd(bp, c)							   \
  if(bp->off >= bp->len) {					   \
    bp->mem = realloc(bp->mem, bp->len + STRING_BUILDER_EXTEND_LENGTH); \
    if(bp->mem == NULL) goto STRING_BUILDER_ADD_FAILED;			   \
    bp->len = bp->len + STRING_BUILDER_EXTEND_LENGTH;		   \
  }									   \
  bp->mem[(bp->off)++] = c


#define sbInit(b, init_len) b.off = 0; b.len = init_len; b.mem = (char*)malloc(init_len)


static inline StringBuilder * sbNew(unsigned int length) {
  StringBuilder * b = (StringBuilder*)malloc(sizeof(StringBuilder));
  if(b == NULL) return NULL;
  b->mem = (char*)malloc(length);
  if(b->mem == NULL) {
    free((void*)b);
    return NULL;
  }
  b->off = 0;
  b->len = length;
}

static inline int sbShrink(StringBuilder *b) {
  b->mem = realloc(b->mem, b->off);
  if(b->mem == NULL) return -1;
  return 0;
}

#endif
