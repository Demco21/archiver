#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sizeprototypes.h"
#include "constheader.h"

//handles -a flag
void archive(char **filenames, int numFiles, char *archiveName)
{
  FILE *file;
  FILE *archive;
  unsigned char filenameLen;
  unsigned char fileContents;
  int fsize = 0;
  int i = 0; //for loop indexes
  int j = 0;
  
  //print error message if archive cant be opened for writing
  if((archive = fopen(archiveName, "w")) == NULL)
  {
    fprintf(stderr, "Could not open archive %s for writing.\n", filenames[i]);
    exit(1);
  }
  
  //first 4 bytes of archive represents how many files are inside of it
  fwrite(&numFiles, sizeof(int), WRITE_ONCE, archive);

  //iterates through each file
  for (i = 0; i < numFiles; i++)
  {
    //print error message if file cant be opened for reading
    if((file = fopen(filenames[i], "r")) == NULL)
    {
      fprintf(stderr, "Could not open file %s for reading.\n", filenames[i]);
      exit(1);
    }
    
    //get length of the file's name
    filenameLen = strlen(filenames[i]);
    
    //next byte in archive is the length of the file's name
    fwrite(&filenameLen, sizeof(unsigned char), WRITE_ONCE, archive);
    
    
    for(j = 0; j < filenameLen + 1; j++)
    {
        fwrite(&filenames[i][j], WRITE_ONCE, WRITE_ONCE, archive);
    }
    
    //get size of the file
    fsize = fileSize(filenames[i]);
    
    //next 4 bytes in archive is the files size
    fwrite(&fsize, sizeof(int), WRITE_ONCE, archive);
    
    //next fileSize # of bytes in archive is the contents of the file
    while((fread(&fileContents, sizeof(unsigned char), READ_ONCE, file)) != 0)
    {
      fwrite(&fileContents, sizeof(unsigned char), WRITE_ONCE, archive);
    }
    
    fclose(file);
  }
  
  fclose(archive);
  printf("Your files were successfully archived as %s\n", archiveName);
}

//handles -u flag
void unarchive(char *archiveName)
{
  FILE* file;
  FILE* archive;
  int numFiles = 0; 
  int fileLen = 0;
  char fileContents;
  unsigned char filenameLen = 0;
  int i = 0; //for loop indexes
  int j = 0;
  
  //print error message if archive cant be opened for reading
  if((archive = fopen(archiveName, "r")) == NULL)
  {
    fprintf(stderr, "Could not open archive %s for reading.\n", archiveName);
    exit(1);
  }
  
  //get the number of files, store value into numFiles
  fread(&numFiles, sizeof(int), READ_ONCE, archive);
  
  //iterates through each file
  for(i = 0; i < numFiles; i++)
  { 
    //get the length of the file name
    fread(&filenameLen, sizeof(unsigned char), READ_ONCE, archive);
    
    //allocate enough memory to fit the filename
    char fileName[filenameLen + 1];
    
    //get the name of the file
    fread(fileName, filenameLen + 1, READ_ONCE, archive);
    
    //get the length of the contents of the file
    fread(&fileLen, sizeof(int), READ_ONCE, archive);
    
    //print error message if file cant be opened for writing
    if((file = fopen(fileName, "w")) == NULL)
    {
      fprintf(stderr, "Could not open file %s for writing.\n", fileName);
      exit(1);
    }
    
    for(j = 0; j < fileLen; j++)
    {
      //read the file contents from the archive and store in filetext
      fread(&fileContents, READ_ONCE, READ_ONCE, archive);
      
      //write the contents to the file
      fwrite(&fileContents, WRITE_ONCE, WRITE_ONCE, file);
    }
    
    free(fileName);
    fclose(file);
  }
  
  fclose(archive);
  printf("Your files were successfully unarchived.\n");
}


