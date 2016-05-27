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

#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

typedef struct {
  char *mem;         // memory buffer
  unsigned int len;  // current length of buffer
  unsigned int off;  // current offset of data in buffer
} StringBuilder;

#endif
