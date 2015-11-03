#ifndef MORE_UTF8_H
#define MORE_UTF8_H

#include <more/common.h>

#if defined(using_more) || defined(using_more_utf8)
#define decodeUtf8Error        more_decodeUtf8Error
#define decodeUtf8ErrorMessage more_decodeUtf8ErrorMessage
#define tryDecodeUtf8          more_tryDecodeUtf8
#define decodeUtf8             more_decodeUtf8
#endif

#if defined(more_test) || defined(more_utf8_test)
bool test_more_utf8();
#endif


#define MORE_UTF8_DECODE_ERROR_LIMIT (dchar_max - 2)
#define more_decodeUtf8Error(c) ((c) >= MORE_UTF8_DECODE_ERROR_LIMIT)

more_lenstring more_decodeUtf8ErrorMessage(dchar error);

dchar more_tryDecodeUtf8(const char** utf8Buffer, const char* limit);

/**
   
 */
#define more_decodeUtf8(error, catch, c, ...)			\
  c = more_tryDecodeUtf8(__VA_ARGS__);				\
  if(more_decodeUtf8Error(c)) {					\
    error.message = more_decodeUtf8ErrorMessage(c);		\
    error.code = c;						\
    goto catch;							\
  }

#endif
