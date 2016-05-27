#ifndef REQUEST_H
#define REQUEST_H

char *                           //            : If NULL, then could not get any request variables.
                                 //            : Otherwise, is a pointer to a buffer of the request variables.
                                 //            : If the request variables were read from STDIN, then it will
                                 //            : just be a pointer to the input varBuffer, otherwise, it will
                                 //            : be a pointer to the environment variable containing the request vars.
getRequestVariablesDefaultPost(
  char *varBuffer                //      [OUT] : Buffer to store the request vars if they come from STDIN.
                                 //            : This variable MUST NOT BE NULL and MUST be >= to MAX_REQUEST_VARS_STRING
			       );

#endif
