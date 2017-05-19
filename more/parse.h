#ifndef _PARSE_H_
#define _PARSE_H_



#define declare_parse_error_strings()                                      \
    extern const char parse_error_overflow[];                              \
    extern const char parse_error_too_large[];                             \
    extern const char parse_error_bad_chars[];                             \
    struct {int trash;} /*force macro caller to put in a semi-colon*/

#define define_parse_error_strings()                                       \
    const char parse_error_overflow[]  = "overflows the type";             \
    const char parse_error_too_large[] = "is to large";                    \
    const char parse_error_bad_chars[] = "contains invalid character(s)";  \
    struct {int trash;} /*force macro caller to put in a semi-colon*/

// NOTE: the current implementation will not always detect overflow.
#define implement_unsigned_parser(type)                                                 \
const char* parse_##type(type* output, type max, const char* ptr, const char* limit)    \
{                                                                                       \
    type ret = 0;                                                                       \
    for( ; ptr < limit; ptr++)                                                          \
    {                                                                                   \
        char c = *ptr;                                                                  \
        type next;                                                                      \
        if(c < '0' || c > '9')                                                          \
        {                                                                               \
            return parse_error_bad_chars;                                               \
        }                                                                               \
        next = (ret * 10) + (c - '0');                                                  \
        if(next > max)                                                                  \
        {                                                                               \
            return parse_error_too_large;                                               \
        }                                                                               \
        if(next <= ret && ret != 0)                                                     \
        {                                                                               \
            return parse_error_overflow;                                                \
        }                                                                               \
        ret = next;                                                                     \
    }                                                                                   \
    *output = ret;                                                                      \
    return NULL; /* no error */                                                         \
}                                                                                       \
struct {int trash;} /*force macro caller to put in a semi-colon*/

#endif