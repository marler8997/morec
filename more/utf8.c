#if defined(more_test) || defined(more_utf8_test)
#include <stdio.h>
#endif

#define using_more

#include <more/common.h>
#include <more/utf8.h>

#define STARTED_INSIDE_CODE_POINT (dchar_max    )
#define MISSING_BYTES             (dchar_max - 1)
#define OUT_OF_RANGE              (dchar_max - 2)
#define LOWEST_ERROR_CODE         OUT_OF_RANGE

#if LOWEST_ERROR_CODE != MORE_UTF8_DECODE_ERROR_LIMIT
#error Need to update MORE_UTF8_DECODE_ERROR_LIMIT in utf8.h because new error codes have been added.
#endif

lenstring decodeUtf8ErrorMessage(dchar error)
{
  switch(error) {
  case OUT_OF_RANGE: return lenstring_literal("the utf8 code point is out of range");
  case MISSING_BYTES: return lenstring_literal("utf8 encoding is missing some bytes");
  case STARTED_INSIDE_CODE_POINT: return lenstring_literal("utf8 data started inside a code point");
  default: return lenstring_literal("unknown error");
  }
}

// Use decodeUtf8Error macro to check if the return code is an error
dchar tryDecodeUtf8(const char** utf8Buffer, const char* limit)
{
  const char* utf8 = *utf8Buffer;
  dchar c = *utf8;
  utf8++;
  if(c <= 0x7F) {
    *utf8Buffer = utf8;
    return c;
  }
  if((c & 0x40) == 0) {
    return STARTED_INSIDE_CODE_POINT;
  }
  if((c & 0x20) == 0) {
    if(utf8 >= limit) return MISSING_BYTES;
    *utf8Buffer = utf8 + 1;
    return ((c << 6) & 0x7C0) | (*(utf8) & 0x3F);
  }

  if((c & 0x10) == 0) {
    utf8++;
    if(utf8 >= limit) return MISSING_BYTES;
    *utf8Buffer = utf8 + 1;
    return ((c << 12) & 0xF000) | ((*(utf8 - 1) << 6) & 0xFC0) | (*(utf8) & 0x3F);
  }
  if((c & 0x08) == 0) {
    utf8 += 2;
    if(utf8 >= limit) return MISSING_BYTES;
    *utf8Buffer = utf8 + 1;
    return ((c << 18) & 0x1C0000) | ((*(utf8 - 2) << 12) & 0x3F000) |
      ((*(utf8 - 1) << 6) & 0xFC0) | (*(utf8) & 0x3F);
  }

  return OUT_OF_RANGE;
}


#if defined(more_test) || defined(more_utf8_test)
#define assert(x) if(!(x)) return false;
typedef lenarray(char) mutable_lenstring;
static bool testDecodeUtf8(dchar expected, mutable_lenstring encoded)
{
  const char* ptr = encoded.ptr;
  dchar actual = tryDecodeUtf8(&ptr, encoded.ptr + encoded.len);

  if(decodeUtf8Error(actual)) {
    printf("decodeUtf8 failed\n");
    return false;
  }

  if(expected != actual) {
    printf("Expected %d actual %d\n", expected, actual);
    return false;
  }
  if(ptr - encoded.ptr != encoded.len) {
    printf("Expected to decode %zu chars but decoded %zu\n", encoded.len, (size_t)(ptr - encoded.ptr));
    return false;
  }

  //printf("[DEBUG] Expected %d actual %d\n", expected, actual);

  return true;
}
bool testInvalidUtf8(dchar expectedError, mutable_lenstring encoded)
{
  const char* ptr = encoded.ptr;
  dchar actual = tryDecodeUtf8(&ptr, encoded.ptr + encoded.len);

  if(!decodeUtf8Error(actual)) {
    printf("Expected decodeUtf8 to fail but did not\n");
    return false;
  }
  if(actual != expectedError) {
    printf("Expected error '%s' but got error '%s'\n",
	   decodeUtf8ErrorMessage(expectedError).ptr,
	   decodeUtf8ErrorMessage(actual).ptr);
    return false;
  }

  {
    Error thrownError;
    ptr = encoded.ptr;
    decodeUtf8(thrownError, CATCH_UTF8_ERROR, actual, &ptr, encoded.ptr + encoded.len);

    printf("Expected decodeUtf8 to fail but did not\n");
    return false;

  CATCH_UTF8_ERROR:
    if(actual != thrownError.code) {
      printf("Expected error '%s' but got error '%s'\n",
	     decodeUtf8ErrorMessage(expectedError).ptr,
	     decodeUtf8ErrorMessage(actual).ptr);
      return false;
    }
  }

  //printf("[DEBUG] got expected error '%s'\n", decodeUtf8ErrorMessage(actual));

  return true;  
}
bool test_more_utf8()
{
  char buffer[16];
  mutable_lenstring encoded;
  ubyte i;
  encoded.ptr = buffer;

  encoded.len = 1;
  for(i = 0; i <= 0x7F; i++) {
    encoded.ptr[0] = i;
    assert(testDecodeUtf8(i, encoded));
  }

  encoded.ptr[0] = 0xC2;
  encoded.ptr[1] = 0xA2;
  encoded.len = 2;
  assert(testDecodeUtf8(0x00A2, encoded));

  encoded.ptr[0] = 0xE2;
  encoded.ptr[1] = 0x82;
  encoded.ptr[2] = 0xAC;
  encoded.len = 3;
  assert(testDecodeUtf8(0x20AC, encoded));

  encoded.ptr[0] = 0xF0;
  encoded.ptr[1] = 0xA4;
  encoded.ptr[2] = 0xAD;
  encoded.ptr[3] = 0xA2;
  encoded.len = 4;
  assert(testDecodeUtf8(0x24B62, encoded));

  //
  // Test Expected Errors
  //
  encoded.ptr[0] = 0x80;
  encoded.len = 1;
  testInvalidUtf8(STARTED_INSIDE_CODE_POINT, encoded);
  encoded.ptr[0] = 0xC0;
  encoded.len = 1;
  testInvalidUtf8(MISSING_BYTES, encoded);
  encoded.ptr[0] = 0xE0;
  encoded.ptr[1] = 0x80;
  encoded.len = 2;
  testInvalidUtf8(MISSING_BYTES, encoded);

  return true;
}
#endif
