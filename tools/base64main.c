#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "base64.h"

#define FILE_BUFFER_SIZE 1024 // Make sure this size is NOT 2 mod 4, this is because
                              // Base64 encoded data cannot be 1 mod 4, and to save
                              // room for null, this buffer cannot be 2 mod 4

#define ENCODING_BUFFER_SIZE ((FILE_BUFFER_SIZE*4)/3)

#define throw(printArgs...) {fprintf(stderr, printArgs); return -1; }

#define checkOptionAlreadySet(condition, option)			\
  if(condition) throw("You specified the " option " option twice\n")

void usage()
{
  printf("This program calculates the base64 encoding/decoding of the given data.\n");
  printf("\n");
  printf("usage: base64 [OPTIONS]\n");
  printf("  -d,--decode           decode the data (default is to encode)\n");
  printf("  -i,--inline  <data>   provide the data on the command line\n");
  printf("  -x,--hex              print data in hex (only used when decoding)\n");
  printf("  -n,--no-newline       omit the newline '\\n' after the data\n");
  printf("  -v,--verbose          verbose\n");
  printf("  -h,--help             display usage/help\n");
}

int main(int argc, char *argv[])
{
  unsigned length;
  int i;
  char *a;

  char *inlineData = NULL;
  char decode = 0;
  char printHex = 0;
  char omitNewline = 0;
  char verbose = 0;

  char fileBuffer[FILE_BUFFER_SIZE];
  char defaultEncodingBuffer[ENCODING_BUFFER_SIZE];
  char *encodingBuffer = defaultEncodingBuffer;

  int bytesRead, totalBufferBytesRead;
  int tempInt;

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
	if(strcmp(a, "decode") == 0) {
	  checkOptionAlreadySet(decode, "-d,--decode");
	  decode = 1;
	} else if(strcmp(a, "inline") == 0) {
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

	  if(*a == 'd') {
	    checkOptionAlreadySet(decode, "-d,--decode");
	    decode = 1;
	  } else if(*a == 'i') {
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

  if(!decode && printHex) {
    fprintf(stderr, "ERROR: 'print in hex' option (-x,--hex) is only used when decoding (-d,--decode)\n");
    return -1;
  }



  //
  // Perform Encoding or Decoding
  //
  if(inlineData) {

    if(verbose) {
      printf("(Inline Mode) data = '%s'\n", inlineData);
    }

    if(decode) {

      // Check Data
      length = base64CountValidEncodedChars(inlineData);

      if(inlineData[length] != '\0') {
	fprintf(stderr, "ERROR: You're encoded input can only be valid Base64 characters [+/a-zA-Z].  Your data '%s' contains an invalid character '%c' (hex=0x%04x, dec=%u) at index %d.\n",
		inlineData, inlineData[length], inlineData[length], inlineData[length], length);
	return -1;
      }
      if(length % 4 == 1) {
	fprintf(stderr, "ERROR: the data you provided is misaligned, base64 encoded data requires the data length is anything but 1 mod 4.\n");
	return -1;	
      }

      // Allocate output buffer (if needed)
      i = (length>>2)*3;
      if(i > ENCODING_BUFFER_SIZE) {
	encodingBuffer = malloc(i);
      }

      // Perform decoding
      length = base64dec(inlineData, encodingBuffer);

      if(printHex) {
	for(i = 0; i < length; i++) {
	  printf("%02X ", (unsigned char)encodingBuffer[i]);
	}
      } else {
	for(i = 0; i < length; i++) {
	  putchar((unsigned char)encodingBuffer[i]);
	}
      }
      
    } else {

      i = strlen(inlineData);
      i = (i<<2)/3 + 1;
      if(i > ENCODING_BUFFER_SIZE) {
	encodingBuffer = malloc(i);
      }
      base64enc(inlineData, strlen(inlineData), encodingBuffer);
      printf(omitNewline ? "%s" : "%s\n", encodingBuffer);

    }

  } else {

    if(decode) {

      tempInt = 0;
      while(1) {
	
      GET_DATA_FROM_STDIN:
	bytesRead = tempInt + read(0, fileBuffer + tempInt, FILE_BUFFER_SIZE - tempInt - 1); // -1 for '\0'
	if(bytesRead <= tempInt) break;
	
	tempInt = bytesRead % 4;
	bytesRead -= tempInt;
	  
	if(bytesRead > 0) {
	  char tempChar = fileBuffer[bytesRead];
	  fileBuffer[bytesRead] = '\0';
	  
	  // Check data
	  length = base64CountValidEncodedChars(fileBuffer);
	  if(length != bytesRead) {
	    fprintf(stderr, "ERROR: You're encoded input can only be valid Base64 characters [+/a-zA-Z], but you had a '%c' (hex=0x%04x, dec=%u).\n",
		    fileBuffer[length], fileBuffer[length], fileBuffer[length]);
	    return -1;
	  }
	  // Perform decoding
	  length = base64dec(fileBuffer, encodingBuffer);
	
	  if(printHex) {
	    for(int i = 0; i < length; i++) {
	      printf("%02x ", encodingBuffer[i]);
	    }
	  } else {
	    for(i = 0; i < length; i++) {
	      putchar(encodingBuffer[i]);
	    }
	  }

	  // Copy extra bytes to beginning of buffer
	  fileBuffer[bytesRead] = tempChar;
	  for(int i = 0; i < tempInt; i++) {
 	    fileBuffer[i] = fileBuffer[bytesRead + i];
	  }

	}
	
	totalBufferBytesRead = 0;
      }

      if(tempInt > 0) {
	if(tempInt == 1) {
	  fprintf(stderr, "ERROR: the data you provided is misaligned, base64 encoded data requires the data length is anything but 1 mod 4.\n");
	  return -1;	
	}

	// Check data
	fileBuffer[tempInt] = '\0';
	length = base64CountValidEncodedChars(fileBuffer);
	if(length != bytesRead) {
	  fprintf(stderr, "ERROR: You're encoded input can only be valid Base64 characters [+/a-zA-Z], but you had a '%c' (hex=0x%04x, dec=%u).\n",
		  fileBuffer[length], fileBuffer[length], fileBuffer[length]);
	  return -1;
	}
	// Perform decoding
	length = base64dec(fileBuffer, encodingBuffer);
	if(printHex) {
	  for(int i = 0; i < length; i++) {
	    printf("%02x ", encodingBuffer[i]);
	  }
	} else {
	  for(i = 0; i < length; i++) {
	    putchar(encodingBuffer[i]);
	  }
	}
      }

    } else {

      tempInt = 0;
      while(1) {

	bytesRead = tempInt + read(0, fileBuffer + tempInt, FILE_BUFFER_SIZE - tempInt);
	if(bytesRead <= tempInt) break;
	
	tempInt = bytesRead % 3;
	bytesRead -= tempInt;

	if(bytesRead  > 0) {
	  base64enc(fileBuffer, bytesRead, encodingBuffer);
	  printf("%s", encodingBuffer);

	  // Copy extra bytes to beginning of buffer
	  for(int i = 0; i < tempInt; i++) {
 	    fileBuffer[i] = fileBuffer[bytesRead + i];
	  }
	}
      }

      if(tempInt > 0) {
	base64enc(fileBuffer, tempInt, encodingBuffer);
	printf("%s", encodingBuffer);
      }
      
    }
  }    
  if(!omitNewline) putchar('\n');
    
  return 0;
}

