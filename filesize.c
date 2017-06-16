#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constheader.h"

//returns the number of bytes inside the file
int fileSize(char *finp)
{
  int size = 0;
  FILE *file;
  
  //print error message if file cant be opened for reading
  if((file = fopen(finp, "r")) == NULL)
  {
    fprintf(stderr, "Could not open file %s for reading.\n", file);
    exit(1);
  }
  
  //seek to end of file and tell the position
  fseek(file, NO_OFFSET, SEEK_END);
  size = ftell(file);
  rewind(file);
  
  fclose(file);
  
return size;
}
