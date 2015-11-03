#define using_more

#include <more/common.h>

// Note: this function only exists because outside of
//       the declaration, there's no way to assign a struct
//       in C wihout using a function.
lenstring more_lenstring_make(const char* ptr, size_t len)
{
  return (lenstring){ptr, len};
}
limstring more_limstring_make(const char* ptr, const char* lim)
{
  return (limstring){ptr, lim};
}
