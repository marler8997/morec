#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//#define DEBUG

#include "weblib.h"
#include "log.macros.c"


// This page should usually be called using the 'POST' method

// buffer must hold at least MAX_REQUEST_VARS_STRING

char *                           //            : If NULL, then could not get any request variables.
                                 //            : Otherwise, is a pointer to a buffer of the request variables.
                                 //            : If the request variables were read from STDIN, then it will
                                 //            : just be a pointer to the input varBuffer, otherwise, it will
                                 //            : be a pointer to the environment variable containing the request vars.
getRequestVariablesDefaultPost(
  char *varBuffer                //      [OUT] : Buffer to store the request vars if they come from STDIN.
                                 //            : This variable MUST NOT BE NULL and MUST be >= to MAX_REQUEST_VARS_STRING
)
{
  int ret, totalBytesRead;
  char *str = getenv("REQUEST_METHOD");

  if(str == NULL) {

    logMissingEnv("REQUEST_METHOD"); // This should never happen, but if it does
                                     // then check for data on stdin
 
  } else if(str[0] != 'P') {

    if(str[0] == 'G') { // NOTE: If it starts with 'G', it's probably a GET method

      str = getenv("QUERY_STRING");
      return (str == NULL || str[0] == '\0') ? NULL : str;

    }

    // It's not a 'POST' method, and there is no QUERY_STRING request
    return NULL;
    
  }

  //
  // Read the request from post data from STDIN
  //
  // NOTE: I never want to have a CGI script hang waiting for input from STDIN.
  //       In the future I may want to perform a select on the STDIN file descriptor
  //       before reading from it to make sure it has data available.
  //       However, for now I'm just going to put a debug statement here (before I 
  //       read the data) so that if this error ever does happen then it will quicky
  //       be identified in the Debug log.
  //
  totalBytesRead = fcntl(0, F_GETFL);
  logDebug("fcntl = 0x%08x, nonblock = %d", totalBytesRead, (totalBytesRead | O_NONBLOCK));
  logDebug("Reading POST variables from STDIN");

  totalBytesRead = 0;
  while(1) {
    ret = read(0, varBuffer + totalBytesRead,
	       MAX_REQUEST_VARS_STRING - 1 - totalBytesRead); // subtract 1 to save a space for '\0'

    if(ret <= 0) {
      if(ret < 0) {
	logError("while reading stdin for login POST data, read returned %d", ret);
	return NULL;
      }

      varBuffer[totalBytesRead] = '\0'; //add trailing '\0'
      return varBuffer;
    }
    
    // check for '\n'
    ret += totalBytesRead;
    while(totalBytesRead < ret) {
      if(varBuffer[totalBytesRead] == '\n') {
	varBuffer[totalBytesRead] = '\0'; //add trailing '\0'
	return varBuffer;
      }
      totalBytesRead++;
    }
    if(totalBytesRead >= MAX_REQUEST_VARS_STRING - 1) {
      logError("buffer for login POST data is full (%d bytes)", MAX_REQUEST_VARS_STRING);
      return NULL;
    }
  }

}
