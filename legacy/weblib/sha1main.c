#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "sha1.h"

#define FILE_BUFFER_SIZE 1024

#define throw(printArgs...) {fprintf(stderr, printArgs); return -1; }

#define checkOptionAlreadySet(condition, option)			\
  if(condition) throw("You specified the " option " option twice\n")

void usage()
{
  printf("This program calculates and prints the sha of the given data.\n");
  printf("\n");
  printf("usage: sha [OPTIONS]\n");
  printf("  -i,--inline  <data>   provide the data on the command line\n");
  printf("  -x,--hex              print hash in hex\n");
  printf("  -n,--no-newline       omit the newline '\\n' after the hash\n");
  printf("  -v,--verbose          verbose\n");
  printf("  -h,--help             display usage/help\n");
}

int main(int argc, char *argv[])
{
  int i;
  char *a;

  char *inlineData = NULL;
  char printHex = 0;
  char omitNewline = 0;
  char verbose = 0;

  SHA1 sha;

  char fileBuffer[FILE_BUFFER_SIZE];
  int bytesRead;

  //
  // Read Command Line Options
  //
  for(i = 1; i < argc; i++) {
    a = argv[i];

    if(a[0] == '-') {
      a++;
      if(*a == '\0') throw("The character '-' by itself is not a valid option\n")

      if(*a == '-') {

	a++;
	if(strcmp(a, "inline") == 0) {
	  checkOptionAlreadySet(inlineData, "-i,--inline")
	  i++;
	  if(i >= argc) throw("The '-i,--inline' option (inline mode) requires an argument\n");
	  inlineData = argv[i];
	} else if(strcmp(a, "verbose") == 0) {
	  checkOptionAlreadySet(verbose, "-v,--verbose")
	  verbose = 1;
	} else if(strcmp(a, "hex") == 0) {
	  checkOptionAlreadySet(verbose, "-x,--hex")
	  printHex = 1;
	} else if(strcmp(a, "no-newline") == 0) {
	  checkOptionAlreadySet(verbose, "-n,--no-newline")
	  omitNewline = 1;
	} else if(strcmp(a, "help") == 0) {
	  usage();
	  return 1;
	} else {
	  throw("Unrecognized option '--%s'\n", a)
	}

      } else {

	do {

	  if(*a == 'i') {
	    if(*(a+1) != '\0') throw("The '-i' option requires an argument and must be the last letter in the list\n");
	    checkOptionAlreadySet(inlineData, "-i,--inline");
            i++;
	    if(i >= argc) throw("The '-i,--inline' option (inline mode) requires an argument\n")
	    inlineData = argv[i];
	  } else if(*a == 'v') {
	    checkOptionAlreadySet(verbose, "-v,--verbose");
	    verbose = 1;
	  } else if(*a == 'x') {
	    checkOptionAlreadySet(verbose, "-x,--hex");
	    printHex = 1;
	  } else if(*a == 'n') {
	    checkOptionAlreadySet(verbose, "-n,--no-newline");
	    omitNewline = 1;
	  } else if(*a == 'h' || *a == '?') {
	    usage();
	    return 1;
	  } else {
	    fprintf(stderr, "Unknown option '%s'\n", a);
	    return -1;
	  }

	  a++;
	}while(*a != '\0');
      }
      
    } else {
      fprintf(stderr, "Invalid argument '%s'\n", a);
      return -1;
    }

  }

  //
  // Perform Sha
  //
  sha1Init(&sha);

  if(inlineData) {

    if(verbose) {
      printf("(Inline Mode) data = '%s'\n", inlineData);
    }
    sha1AddString(&sha, inlineData);

  } else {

    if(verbose) {
      printf("--- HASH DATA START ---\n");
      fflush(stdout);
    }
    
    while(1) {
      bytesRead = read(0, fileBuffer, FILE_BUFFER_SIZE);
      if(bytesRead <= 0 ) break;
      if(verbose) {
	if(bytesRead != write(1, fileBuffer, bytesRead)) {
	  throw("failed to log hash data, write failed\n");
	}
      }
      sha1AddBinary(&sha, fileBuffer, bytesRead);
    }
    
    if(verbose) printf("\n--- HASH DATA END ---\n");
  }

  sha1Finish(&sha);

  if(printHex) {
    printf(omitNewline ? "%08X%08X%08X%08X%08X":
	                 "%08X%08X%08X%08X%08X\n",
	   sha.hash[0],
	   sha.hash[1],
	   sha.hash[2],
	   sha.hash[3],
	   sha.hash[4]);
  } else {
    fflush(stdout);
    write(1, sha.hash, 5);
    if(!omitNewline) {
      putchar('\n');
    }
  }

  return 0;
}
