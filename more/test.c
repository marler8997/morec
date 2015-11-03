#include <stdio.h>

#include <more/common.h>
#include <more/sha1.h>
#include <more/utf8.h>
#include <more/fields.h>


#define RUN_TEST(test)					\
  printf("[" #test "-test] Running...\n");		\
  success = test_more_##test();				\
  if(success) {						\
    printf("[" #test "-test] PASSED\n");		\
  } else {						\
    printf("[" #test "-test] FAILED\n");		\
  }


int main(int argc, char* argv[])
{
  bool success;

  RUN_TEST(utf8);
  RUN_TEST(fields);
  RUN_TEST(sha1);

  return 0;
}
