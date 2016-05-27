#define ASSERT(x)					\
  if(!(x)) {						\
    printf("ASSERT_FAILED: ASSERT(" #x ") Failed\n");	\
    goto ASSERT_FAILED;					\
  }

#define ASSERT_EQ(expected,actual)					\
  if(expected != actual) {						\
    printf("ASSERT_EQ(" #expected "," #actual ") Failed\n"		\
	   " - Expected (" #expected ")= %d 0x%x\n"			\
	   " - Actual   (" #actual ")  = %d 0x%x\n",			\
	   expected,expected, actual, actual);				\
    goto ASSERT_FAILED;							\
  }

#define ASSERT_STREQ(expected,actual)					\
  if(expected != actual) {						\
    if(expected == NULL || actual == NULL) {				\
      printf("ASSERT_STREQ(" #expected "," #actual ") Failed\n"		\
             " - Expected \"%s\"\n"					\
	     " - Actual    \"%s\"\n", expected, actual);		\
      goto ASSERT_FAILED;						\
    }									\
    int __index;							\
    for(__index = 0; expected[__index]; __index++) {			\
      if(actual[__index] != expected[__index]) {			\
        printf("ASSERT_STREQ(" #expected "," #actual ") Failed\n"	\
	       " - Expected \"%s\"\n"					\
	       " - Actual    \"%s\"\n", expected, actual);		\
	goto ASSERT_FAILED;						\
      }									\
    }									\
    if(actual[__index]) {						\
      printf("ASSERT_STREQ(" #expected "," #actual ") Failed\n"		\
             " - Expected \"%s\"\n"					\
	     " - Actual    \"%s\"\n", expected, actual);		\
      goto ASSERT_FAILED;						\
    }									\
  }								       
  
  


