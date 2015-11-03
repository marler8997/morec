#include <stdint.h>

#define using_more
#include <more/fields.h>
#include <more/utf8.h>

#define UTF8_ERROR 1
#define PARSE_ERROR 2

#define controlCharFlag 0x01
#define whitespaceFlag  0x02
#define tokenStartFlag  0x04

#define TEXT_ERROR_UTF8 0x01

#define charTableLength 256
ubyte charTable[charTableLength] = {
  0, // [0] '\0'
  0, // [1] ''
  0, // [2] ''
  0, // [3] ''
  0, // [4] ''
  0, // [5] ''
  0, // [6] ''
  0, // [7] ''
  0, // [8] ''
  controlCharFlag | whitespaceFlag, //  [9] '\t'
  controlCharFlag | whitespaceFlag, // [10] '\n'
  controlCharFlag | whitespaceFlag, // [11] '\v'
  controlCharFlag | whitespaceFlag, // [12] '\f'
  controlCharFlag | whitespaceFlag, // [13] '\r'
  0, // [14] ''
  0, // [15] ''
  0, // [16] ''
  0, // [17] ''
  0, // [18] ''
  0, // [19] ''
  0, // [20] ''
  0, // [21] ''
  0, // [22] ''
  0, // [23] ''
  0, // [24] ''
  0, // [25] ''
  0, // [26] ''
  0, // [27] ''
  0, // [28] ''
  0, // [29] ''
  0, // [30] ''
  0, // [31] ''
  controlCharFlag | whitespaceFlag, // [32] ' '
  0, // [33] ''
  0, // [34] ''
  controlCharFlag, // [35] '#'
  0, // [36] ''
  0, // [37] ''
  0, // [38] ''
  0, // [39] ''
  0, // [40] ''
  0, // [41] ''
  0, // [42] ''
  0, // [43] ''
  0, // [44] ''
  0, // [45] ''
  0, // [46] ''
  controlCharFlag, // [47] '/'
  0, // [48] ''
  0, // [49] ''
  0, // [50] ''
  0, // [51] ''
  0, // [52] ''
  0, // [53] ''
  0, // [54] ''
  0, // [55] ''
  0, // [56] ''
  0, // [57] ''
  0, // [58] ''
  0, // [59] ''
  0, // [60] ''
  0, // [61] ''
  0, // [62] ''
  0, // [63] ''
  0, // [64] ''
  0, // [65] ''
  0, // [66] ''
  0, // [67] ''
  0, // [68] ''
  0, // [69] ''
  0, // [70] ''
  0, // [71] ''
  0, // [72] ''
  0, // [73] ''
  0, // [74] ''
  0, // [75] ''
  0, // [76] ''
  0, // [77] ''
  0, // [78] ''
  0, // [79] ''
  0, // [80] ''
  0, // [81] ''
  0, // [82] ''
  0, // [83] ''
  0, // [84] ''
  0, // [85] ''
  0, // [86] ''
  0, // [87] ''
  0, // [88] ''
  0, // [89] ''
  0, // [90] ''
  controlCharFlag, // [91] '['
  0, // [92] '\\'
  controlCharFlag, // [93] ']'
  0, // [94] ''
  0, // [95] '_'
  0, // [96] ''
  0, // [97] ''
  0, // [98] ''
  0, // [99] ''
  0, // [100] ''
  0, // [101] ''
  0, // [102] ''
  0, // [103] ''
  0, // [104] ''
  0, // [105] ''
  0, // [106] ''
  0, // [107] ''
  0, // [108] ''
  0, // [109] ''
  0, // [110] ''
  0, // [111] ''
  0, // [112] ''
  0, // [113] ''
  0, // [114] ''
  0, // [115] ''
  0, // [116] ''
  0, // [117] ''
  0, // [118] ''
  0, // [119] ''
  0, // [120] ''
  0, // [121] ''
  0, // [122] ''
  controlCharFlag, // [123] '{'
  0, // [124] '|'
  controlCharFlag, // [125] '}'
  0, // [126] ''
  0, // [127] ''
  0, // [128] ''
  0, // [129] ''
  0, // [130] ''
  0, // [131] ''
  0, // [132] ''
  0, // [133] ''
  0, // [134] ''
  0, // [135] ''
  0, // [136] ''
  0, // [137] ''
  0, // [138] ''
  0, // [139] ''
  0, // [140] ''
  0, // [141] ''
  0, // [142] ''
  0, // [143] ''
  0, // [144] ''
  0, // [145] ''
  0, // [146] ''
  0, // [147] ''
  0, // [148] ''
  0, // [149] ''
  0, // [150] ''
  0, // [151] ''
  0, // [152] ''
  0, // [153] ''
  0, // [154] ''
  0, // [155] ''
  0, // [156] ''
  0, // [157] ''
  0, // [158] ''
  0, // [159] ''
  0, // [160] ''
  0, // [161] ''
  0, // [162] ''
  0, // [163] ''
  0, // [164] ''
  0, // [165] ''
  0, // [166] ''
  0, // [167] ''
  0, // [168] ''
  0, // [169] ''
  0, // [170] ''
  0, // [171] ''
  0, // [172] ''
  0, // [173] ''
  0, // [174] ''
  0, // [175] ''
  0, // [176] ''
  0, // [177] ''
  0, // [178] ''
  0, // [179] ''
  0, // [180] ''
  0, // [181] ''
  0, // [182] ''
  0, // [183] ''
  0, // [184] ''
  0, // [185] ''
  0, // [186] ''
  0, // [187] ''
  0, // [188] ''
  0, // [189] ''
  0, // [190] ''
  0, // [191] ''
  0, // [192] ''
  0, // [193] ''
  0, // [194] ''
  0, // [195] ''
  0, // [196] ''
  0, // [197] ''
  0, // [198] ''
  0, // [199] ''
  0, // [200] ''
  0, // [201] ''
  0, // [202] ''
  0, // [203] ''
  0, // [204] ''
  0, // [205] ''
  0, // [206] ''
  0, // [207] ''
  0, // [208] ''
  0, // [209] ''
  0, // [210] ''
  0, // [211] ''
  0, // [212] ''
  0, // [213] ''
  0, // [214] ''
  0, // [215] ''
  0, // [216] ''
  0, // [217] ''
  0, // [218] ''
  0, // [219] ''
  0, // [220] ''
  0, // [221] ''
  0, // [222] ''
  0, // [223] ''
  0, // [224] ''
  0, // [225] ''
  0, // [226] ''
  0, // [227] ''
  0, // [228] ''
  0, // [229] ''
  0, // [230] ''
  0, // [231] ''
  0, // [232] ''
  0, // [233] ''
  0, // [234] ''
  0, // [235] ''
  0, // [236] ''
  0, // [237] ''
  0, // [238] ''
  0, // [239] ''
  0, // [240] ''
  0, // [241] ''
  0, // [242] ''
  0, // [243] ''
  0, // [244] ''
  0, // [245] ''
  0, // [246] ''
  0, // [247] ''
  0, // [248] ''
  0, // [249] ''
  0, // [250] ''
  0, // [251] ''
  0, // [252] ''
  0, // [253] ''
  0, // [254] ''
  0, // [255] ''
};
bool isControlChar(ubyte c) {
  //return (c < charTableLength) && ( (charTable[c] & controlCharFlag) != 0 );
  return (charTable[c] & controlCharFlag) != 0;
}
bool isWhitespace(ubyte c) {
  //return (c < charTableLength) && ( (charTable[c] & whitespaceFlag) != 0 );
  return (charTable[c] & whitespaceFlag) != 0;
}


bool FieldToken_eof(FieldToken* token)
{
  return token->text.len == 0;
}

void Text_tryInit(Text* text, lenstring str)
{
  Error_init(text->error);

  text->lineNumber = 1;
  text->column = 1;

  text->text = str;
  text->limit = str.ptr + str.len;
  text->cpos = str.ptr;
  text->next = str.ptr;

  if(str.len > 0) {
    decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
  }
 catch:
  return;
}
bool Text_empty(Text* text)
{
  return text->cpos >= text->limit;
}
void Text_trySkipChar(Text* text)
{
  text->cpos = text->next;
  if(text->next < text->limit) {
    decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
  }
 catch:
  return;
}
void Text_tryToNextLine(Text* text)
{
  while(text->next < text->limit) {
    decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
    if(text->c == '\n') {
      text->lineNumber++;
      text->column = 1;
      break;
    }
  }

  // consume the newline
  text->cpos = text->next;

  if(text->next < text->limit) {
    decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
  }

 catch:
  // TODO: column number will probably be wrong
  return;
}
void Text_tryToNewline(Text* text)
{
  do {
    text->cpos = text->next;
    if(text->next >= text->limit) break;
    decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
    text->column++;
  } while(text->c != '\n');
 catch:
  return;
}
void Text_tryToEndOfToken(Text* text)
{
  if(text->c == '"') {
    text->error.message = lenstring_literal("need to implemented quoted tokens");
    return;
  } else {
    
    while(1) {
      text->cpos = text->next;
      if(text->next >= text->limit) break;
      decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
      text->column++;
      if(isControlChar(text->c)) {
	// Handle slashes that aren't comments
	if(text->c != '/') break;
	if(text->next >= text->limit) {
	  text->cpos = text->next;
	  break;
	}
	const char* saveNext = text->next;
	decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
	text->next = saveNext;

	if(text->c == '*' || text->c == '/') {
	  break;
	}

      }
    }

  }
 catch:
  return;
}
// If skipNewlines is true, c/cpos will be pointing at the newline if no field was found
void Text_trySkipWhitespaceAndComments(Text* text, bool skipNewlines)
{
  while(true) {

    // TODO: maybe use a lookup table here
    if(text->c == ' ' || text->c == '\t' || text->c =='\v' || text->c == '\f' || text->c == '\r') {

      // do nothing (check first as this is the most likely case)

    } else if(text->c == '\n') {

      if(!skipNewlines) return;

      text->lineNumber++;
      text->column = 1;

    } else if(text->c == '#') {

      if(!skipNewlines) {
	Text_toNewline(catch, text);
	return;
      }

      Text_toNextLine(catch, text);

    } else if(text->c == '/') {

      if(text->next >= text->limit) return;
      decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
	
      if(text->c == '/') {

	if(!skipNewlines) {
	  Text_toNewline(catch, text);
	  return;
	}

	Text_toNextLine(catch, text);

      } else if(text->c == '*') {

	if(!skipNewlines) {
	  text->error.message = lenstring_literal("found multiline comment but currently not skipping lines");
	  return;
	}
	  
	text->column++;

	while(text->next < text->limit) {

	  decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
	  text->column++;

	  if(text->c == '\n') {
	    text->lineNumber++;
	    text->column = 0;
	    text->lineNumber++;
	  } else if(text->c == '*') {
	    // loop assume c is pointing to a '*' and next is pointing to the next characer
	    while(text->next < text->limit) {

	      decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
	      text->column++;
	      if(text->c == '/') goto END_OF_MULTILINE_COMMENT_LOOP;
	      if(text->c == '\n') {
		text->lineNumber++;
		text->column = 0;
	      } else if(text->c != '*') {
		break;
	      }
	    }
	  }
	}

      END_OF_MULTILINE_COMMENT_LOOP:
	;

      } else {
	return;
      }

    } else {

      return; // Found non-whitespace and non-comment

    }

    //
    // Goto next character
    //
    text->cpos = text->next;
    if(text->next >= text->limit) return;
    decodeUtf8(text->error, catch, text->c, &text->next, text->limit);
    text->column++;
  }
 catch:
  return;
}
void Text_tryParseField(Text* text, FieldToken* token, bool sameLine)
{
  Text_skipWhitespaceAndComments(catch, text, !sameLine);
  if(text->cpos >= text->limit || (sameLine && text->c == '\n')) {
    token->text.ptr = NULL;
    token->text.len = 0;
    token->lineNumber = text->lineNumber;
    token->column = text->column;
  } else {

    if(isControlChar(text->c)) {
      //text->error.message = lenstring_literal(format("Expected non-control character but got '%s' (charcode=%s)",c, cast(uint)c));
      text->error.message = lenstring_literal("Expected non-control character");
      return;
    }

    {
      const char* startOfToken = text->cpos;
      token->lineNumber = text->lineNumber;
      token->column = text->column;
      Text_toEndOfToken(catch, text);
      token->text.ptr = startOfToken;
      token->text.len = text->cpos - startOfToken;
    }
  }
 catch:
  return;
}
/*
bool Text_noMoreFieldsOnThisLine(Text* text)
{
  skipWhitespaceAndComments(false);
  return cpos >= limit || c == '\n';
}


void Text_parseString(Text* text, FieldToken* token)
{
  skipWhitespaceAndComments(true);
  token->lineNumber = text->lineNumber;
  token->column = text->column;
  if(cpos >= limit || isControlChar(c)) {
    token->text = null;
  } else {
    const(char)* startOfToken = cpos;
    toEndOfToken();
    token->text = startOfToken[0..cpos-startOfToken];
  }
}


alias parseString parseObjectFieldName;

// An object starts with an open curly brace '{' or omits its curly
// brace section with a semi-colon ';'
// A 'NamelessObjectField' is a field before the curly-brace section
void Text_parseNamelessObjectField(Text* text, FieldToken* token)
{
  skipWhitespaceAndComments(true);
  token->lineNumber = text->lineNumber;
  token->column = text->column;
  if(cpos >= limit || isControlChar(c)) {
    token->text = null;
  } else {
    const(char)* startOfToken = cpos;
    toEndOfToken();
    token->text = startOfToken[0..cpos-startOfToken];
  }
}
bool Text_atObjectStart(Text* text)
{
  skipWhitespaceAndComments(true);
  if(cpos >= limit || c != '{') return false;

  cpos = next;
  if(next < limit) {
    c = tryDecodeUtf8(next, limit);
  }
  return true;
}
*/

#if defined(more_test) || defined(more_fields_test)
#define assert(x) if(!(x)) return false;
bool test_more_fields()
{
  assert(isControlChar(' '));
  assert(isControlChar('\t'));
  assert(isControlChar('\n'));
  assert(isControlChar('\v'));
  assert(isControlChar('\f'));
  assert(isControlChar('\r'));
  assert(isControlChar('{'));
  assert(isControlChar('}'));
  assert(isControlChar('['));
  assert(isControlChar(']'));
  assert(isControlChar('/'));
  assert(isControlChar('#'));

  assert(isWhitespace(' '));
  assert(isWhitespace('\t'));
  assert(isWhitespace('\n'));
  assert(isWhitespace('\v'));
  assert(isWhitespace('\f'));
  assert(isWhitespace('\r'));


  return true;
}
#endif
