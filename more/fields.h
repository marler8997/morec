#ifndef MORE_FIELDS_H
#define MORE_FIELDS_H

#if defined(using_more) || defined(using_more_fields)
#define isControlChar more_fields_isControlChar
#define isWhitespace more_fields_isWhitespace

#define FieldToken more_FieldToken
#define FieldToken_eof more_FieldToken_eof

#define Text more_Text
#define Text_tryInit more_Text_tryInit
#define Text_init    more_Text_init
#define Text_empty more_Text_empty
#define Text_trySkipChar more_Text_trySkipChar
#define Text_skipChar    more_Text_skipChar
#define Text_tryToNextLine more_Text_tryToNextLine
#define Text_toNextLine    more_Text_toNextLine
#define Text_tryToNewline more_Text_tryToNewline
#define Text_toNewline    more_Text_toNewline
#define Text_tryToEndOfToken more_Text_tryToEndOfToken
#define Text_toEndOfToken    more_Text_toEndOfToken
#define Text_trySkipWhitespaceAndComments more_Text_trySkipWhitespaceAndComments
#define Text_skipWhitespaceAndComments    more_Text_skipWhitespaceAndComments
#define Text_tryParseField more_Text_tryParseField
#define Text_parseField    more_Text_parseField
#endif

#ifdef using_more_fields_FieldToken

#define eof more_FieldToken_eof

#endif
#ifdef using_more_fields_Text

#define tryInit more_Text_tryInit
#define init    more_Text_init
#define empty more_Text_empty
#define trySkipChar more_Text_trySkipChar
#define skipChar    more_Text_skipChar
#define tryToNextLine more_Text_tryToNextLine
#define toNextLine    more_Text_toNextLine
#define tryToNewline more_Text_tryToNewline
#define toNewline    more_Text_toNewline
#define tryToEndOfToken more_Text_tryToEndOfToken
#define toEndOfToken    more_Text_toEndOfToken
#define trySkipWhitespaceAndComments more_Text_trySkipWhitespaceAndComments
#define skipWhitespaceAndComments    more_Text_skipWhitespaceAndComments
#define tryParseField more_Text_tryParseField
#define parseField    more_Text_parseField

#endif

#include <more/common.h>

#if defined(more_test) || defined(more_fields_test)
bool test_more_fields();
#endif

bool more_fields_isControlChar(ubyte c);
bool more_fields_isWhitespace(ubyte c);

typedef struct {
  more_lenstring text;
  uint32_t lineNumber;
  uint32_t column;
} more_FieldToken;
bool more_FieldToken_eof(more_FieldToken* token);

typedef struct {
  more_Error error;

  uint32_t lineNumber;
  uint32_t column;

  more_lenstring text;
  const char* limit;

  const char* cpos;
  const char* next;

  uint32_t c;
} more_Text;

void more_Text_tryInit(more_Text* text, more_lenstring str);
#define more_Text_init(catch, this, ...)	\
  more_Text_tryInit(this, __VA_ARGS__);		\
  if(more_Error_hasThrown((this)->error)) {	\
    goto catch;					\
  }

bool more_Text_empty(more_Text* text);

void more_Text_trySkipChar(more_Text* text);
#define more_Text_skipChar(catch, this)		\
  more_Text_trySkipChar(this);			\
  if(more_Error_hasThrown((this)->error)) {	\
    goto catch;					\
  }

void more_Text_tryToNextLine(more_Text* text);
#define more_Text_toNextLine(catch, this)	\
  more_Text_tryToNextLine(this);		\
  if(more_Error_hasThrown((this)->error)) {	\
    goto catch;					\
  }

void more_Text_tryToNewline(more_Text* text);
#define more_Text_toNewline(catch, this)	\
  more_Text_tryToNewline(this);			\
  if(more_Error_hasThrown((this)->error)) {	\
    goto catch;					\
  }

void more_Text_tryToEndOfToken(more_Text* text);
#define more_Text_toEndOfToken(catch, this)	\
  more_Text_tryToEndOfToken(this);		\
  if(more_Error_hasThrown((this)->error)) {	\
    goto catch;					\
  }

void more_Text_trySkipWhitespaceAndComments(more_Text* text, bool skipNewlines);
#define more_Text_skipWhitespaceAndComments(catch, this, ...)	\
  more_Text_trySkipWhitespaceAndComments(this, __VA_ARGS__);	\
  if(more_Error_hasThrown((this)->error)) {			\
    goto catch;							\
  }

void more_Text_tryParseField(more_Text* text, more_FieldToken* token, bool sameLine);
#define more_Text_parseField(catch, this, ...)	\
  more_Text_tryParseField(this, __VA_ARGS__);	\
  if(more_Error_hasThrown((this)->error)) {	\
    goto catch;					\
  }

#endif
