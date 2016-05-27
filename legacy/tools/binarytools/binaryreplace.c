#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "binarycommon.h"

void usage()
{
  fprintf(stderr, "Usage: ./binaryreplace search-hex-block replace-hex-block < in-file > out-file\n");
  fprintf(stderr, "   example: ./binaryreplace af0103 00ff00ff00ff\n");
  fprintf(stderr, "This program replaces all search-hex-block instances with replace-hex-block.\n");
  fprintf(stderr, "It prints the byte offset of each replacement to STDERR, and the file with replacements to STDOUT.\n");
}

int main(int argc, char *argv[])
{
  unsigned char buffer[4096];

  if(argc != 3) {
    usage();
    return -1;
  }

  //
  // Parse Command Line Arguments
  //
  unsigned char *search;
  size_t searchLength;

  unsigned char *replace;
  size_t replaceLength;

  const char *error;

  error = parseHex(argv[1], &search, &searchLength);
  if(error) {
    fprintf(stderr, "Error: could not parse search-hex-block '%s' because %s\n", argv[1], error);
    return -1;
  }

  error = parseHex(argv[2], &replace, &replaceLength);
  if(error) {
    fprintf(stderr, "Error: could not parse replace-hex-block '%s' because %s\n", argv[2], error);
    return -1;
  }

  //
  // Check Search Parameters
  //
  if(searchLength > sizeof(buffer)) {
    fprintf(stderr, "Error: search length (%d) is > buffer length %d\n", searchLength, sizeof(buffer));
    return -1;
  }

  //
  // Search Loop
  //
  unsigned long totalBytesProcessed = 0;
  size_t blockByteLength = 0;

  while(1) {
    do {
      int bytesRead = read(0, buffer + blockByteLength, sizeof(buffer) - blockByteLength);
      if(bytesRead <= 0) {
	if(bytesRead < 0) goto READ_ERROR;
	goto DONE;
      }
      blockByteLength += bytesRead;
    } while(blockByteLength < searchLength);

    // Look for a match
    size_t offset;
    for(offset = 0; offset <= blockByteLength - searchLength; offset++) {

      if(search[0] == buffer[offset]) {
	for(ssize_t i = 1; i < searchLength; i++) {
	  if(search[i] != buffer[offset+i]) goto NO_MATCH;
	}
	
	// replace match
	for(ssize_t i = 0; i < replaceLength; i++) {
	  putchar(replace[i]);
	}

	offset += searchLength-1;
	fprintf(stderr, "%lu\n", totalBytesProcessed);
	totalBytesProcessed += searchLength; // TODO: make sure this is correct
	continue;
      }
    NO_MATCH:
      putchar(buffer[offset]);

      totalBytesProcessed++;
    }

    // Copy last bytes to beginning of buffer
    size_t i;
    for(i = 0; offset < blockByteLength; i++) {
      buffer[i] = buffer[offset];
      offset++;
    }

    blockByteLength = i;
  }

 DONE:

  //
  // Finish printing last bytes
  //
  for(ssize_t offset = 0; offset < blockByteLength; offset++) {
    putchar(buffer[offset]);
  }
  //fprintf(stderr, "Total Bytes Processed: %lu\n", totalBytesProcessed);

  return 0;

 READ_ERROR:
  fprintf(stderr, "Error: read returned error\n");
  return -1;
}
