//
// Usage:
// include this macro file
//
// 
// Options:
//  define DEBUG to enable debugging
//  define DEBUGF_PREFIX to add a prefix to debugf messages
//     i.e. #define DEBUGF_PREFIX "MY_MODULE_DEBUG: "
//
// use debugf the same as printf
// use debug(code) to add debug code
//
// 

#ifdef DEBUG

#ifndef DEBUGF_PREFIX
#define DEBUGF_PREFIX
#endif

#define debug_stmt(stmt) stmt
#define debugf(args...) printf(DEBUGF_PREFIX  args)
#define debug_if(condition) if(condition)

#else

#define debug_stmt(stmt)
#define debugf(args...)
#define debug_if(condition) if(0)

#endif
