

#ifndef ERROR_FILE
#define ERROR_FILE stderr
#endif


// 
// Throw Error Operations
//
#define throwf(error_code, print_args...) if(ERROR_FILE){fprintf(ERROR_FILE, print_args);}return error_code

