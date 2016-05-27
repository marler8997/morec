#include <unistd.h>

// return value: NULL on success, error message on failure
const char * mem2File(const char *filename, void *mem, int numBytes)
{

  // Open the temporary file
  int fd = fopen(filename, O_WRONLY | O_CREAT | O_TRUNC,
		 S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  
  if(fd == 0) return "open function returned 0";


  // Write the test data to the file
  int totalWritten = 0;
  while(totalWritten < numBytes) {
    int lastWritten = write(fd, mem + totalWritten, numBytes - totalWritten);
    if(lastWritten <= 0) return "write failed";
    totalWritten += lastWritten;
  }

  close(fd);
}


