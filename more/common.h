#ifndef MORE_COMMON_H
#define MORE_COMMON_H

#if defined(using_more) || defined(using_more_common)
#define lenstring         more_lenstring
#define lenstring_make    more_lenstring_make
#define lenstring_literal more_lenstring_literal

#define limstring         more_limstring
#define limstring_make    more_limstring_make
#define limstring_literal more_limstring_literal

#define Error more_Error
#define Error_hasThrown more_Error_hasThrown
#define Error_init more_Error_init
#define Bug more_Bug
#endif

#include <stdint.h>
#include <stddef.h>

//
// Define types
//
#define false 0
#define true 1
typedef unsigned char bool;

typedef unsigned char ubyte;
typedef char          byte;

typedef uint16_t      uint16;
typedef int16_t       int16;

typedef uint32_t      uint32;
typedef int32_t       int32;

typedef uint64_t      uint64;
typedef int64_t       int64;

typedef ubyte         error_code;
typedef uint32        dchar;
#define dchar_max 0xFFFFFF

#define lenarray(type) struct { type* ptr; size_t len; }
#define limarray(type) struct { type* ptr; type* lim; }

typedef lenarray(const char) more_lenstring;
typedef limarray(const char) more_limstring;

// Note: this function only exists because outside of
//       the declaration, there's no way to assign a struct
//       in C wihout using a function.
more_lenstring more_lenstring_make(const char* ptr, size_t len);
more_limstring more_limstring_make(const char* ptr, const char* lim);

#define more_lenstring_literal(str) (more_lenstring){(str),sizeof(str)-1}
#define more_limstring_literal(str) (more_limstring){(str),(str)+sizeof(str)-1}

typedef void (*Sink)(more_lenstring message);
typedef void (*VariadicFormat)(more_lenstring fmt, ...);
typedef void (*VariadicSink)(VariadicFormat func, ...);

/*
--------------------------------------------------------------------------------
Bug
--------------------------------------------------------------------------------
A "bug" is a misuse of code or an error in program logic.  A bug should include
a message, the source code file/line number and stack trace. i.e.
  * Invalid argument passed to a function (include file/line number of caller)
  * Forgetting to call a function or calling a function at the wrong time
  * Unexpected program state (note the code line that finds this will not be
    the same code that is misusing the program. it would be nice to indicate
    this somehow)
--------------------------------------------------------------------------------
Error
--------------------------------------------------------------------------------
An "error" is a problem with the environment or indicates that another entity
has failed in providing some feature/function that prevents the program from
doing something. An error should include a message.  The source code location
is unnecessary since an error is not a problem with the code. i.e.
  * Data received from something was invalid
    - user input error
    - file content error
    - socket content error
    - device malfunction, has bad data
  * A system call failed (note: it is important to distinguish between a system
    call that failed and a system call that was misused.  A misuse of a system
    call is a bug, not an error.)


--------------------------------------------------------------------------------
How to handle invalid arguments
--------------------------------------------------------------------------------
If arguments can be validated before the call they should. The function call
should be a macro that checks the arguments first. If the argument is invalid,
it should throw a Bug.

Questions to ask yourself when determining the error type:
 1. Should this error contain the code location? If so, then it is probably a
    bug.
*/


// An error is caused by one of the following:
//   1. Invalid data
//     a. A buffer contains invalid bytes
//     b. Does not include invalid argument values (those indicate bugs)
//   2. A system function 
typedef struct {
  more_lenstring message;
  size_t code;
} more_Error;

#define more_Error_hasThrown(error) ((error).message.len)
#define more_Error_init(error) error = (more_Error){{0,0},0}

// A bug indicates a code bug.  That's why it includes the file name and line number.
typedef struct {
  more_Error error;

  more_lenstring file;
  size_t line;
} more_Bug;

//
// Testing
//


#endif
