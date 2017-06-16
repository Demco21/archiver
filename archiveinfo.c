#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constheader.h"

//handles -l flag
void listInfo(char *archiveName)
{
  FILE *archive;
  int size = fileSize(archiveName);
  int numFiles = 0;
  int fileLen = 0;
  unsigned char filenameLen = 0;
  int i = 0; //for loop index
  
  //print error message if archive cant be opened for reading
  if((archive = fopen(archiveName, "r")) == NULL)
  {
    fprintf(stderr, "Could not open archive %s for reading.\n", archiveName);
    exit(1);
  }
  
  //get the number of files, store value into numFiles
  fread(&numFiles, sizeof(int), READ_ONCE, archive);
  printf("%s contains %d files.\n", archiveName, numFiles);
  printf("%s is %d bytes.\n", archiveName, size);
  
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
    printf("before %d\n", ftell(archive));
    //seek passed the file contents
    printf("%d\n",fseek(archive, fileLen, SEEK_CUR));
    printf("after %d\n", ftell(archive));
    
    printf("%s is %d bytes.\n", fileName, fileLen);
    free(fileName);
    fileLen = 0;
  }
  
  fclose(archive);
}

//handles -v flag
void verify(char **filenames, int numFiles, char *archiveName)
{
  FILE *file;
  FILE *archive;
  char readChar;
  unsigned char filenameLen = 0;
  int numArcFiles = 0;
  int sizeOfFiles = 0;
  int sizeOfArcFiles = 0;
  int missingBytes = 0;
  int fileLen = 0;
  int corrupted = 0; //boolean if files were corrupted
  int i = 0; //for loop indexes
  int j = 0;
  
  //print error message if archive cant be opened for writing
  if((archive = fopen(archiveName, "r")) == NULL)
  {
    fprintf(stderr, "Could not open archive %s for reading.\n", archiveName);
    exit(1);
  }
  
  //first byte of archive represents how many files are inside of it
  fread(&numArcFiles, sizeof(int), READ_ONCE, archive);
  
  //if unequal number of files
  if(numArcFiles < numFiles)
  {
    printf("%s has less than %d files.\n", archiveName, numFiles);
    fclose(archive);
    exit(1);
  }
  if(numArcFiles > numFiles)
  {
    printf("%s has more than %d files.\n", archiveName, numFiles);
    fclose(archive);
    exit(1);
  }
  
  //this will get the total size of the archive and each file from the command line
  for(i = 0; i < numFiles; i++)
  {
    //print error message if archive cant be opened for writing
    if((file = fopen(filenames[i], "r")) == NULL)
    {
      fprintf(stderr, "Could not open file %s for reading.\n", filenames[i]);
      fclose(archive);
      exit(1);
    }
  
    //sum of the bytes in every file******
    sizeOfFiles += fileSize(filenames[i]);
    
    //get the length of the file name
    fread(&filenameLen, sizeof(unsigned char), READ_ONCE, archive);
    
    //seek passed file's name
    fseek(archive, filenameLen+1, SEEK_CUR);

    //get the length of the contents of the file
    fread(&fileLen, sizeof(int), READ_ONCE, archive);
    
    //sum of bytes in each file of archive*****
    sizeOfArcFiles += fileLen;
    
    //allocate enough memory for contents of each file
    char * fileContents = malloc(sizeof(char)*fileSize(filenames[i]));
    char * arcFileContents = malloc(sizeof(char)*fileLen);
    
    //read the file contents from the archive and store in arcFileContents
    fread(arcFileContents, fileLen, READ_ONCE, archive);
    
    //store contents of the file in fileContents
    while((fread(&readChar, sizeof(char), 1, file)) != 0)
    {
      fileContents[j] = readChar;
      j++;
    }
    j = 0;
    
    //if the contents of the files arent equivalent
    if(strcmp(fileContents, arcFileContents) != 0)
    {
      corrupted = 1;
    }
    
    fclose(file);
  }
  
  if(corrupted == 1)
  {
    printf("Archive is corrupted.\n");
    fclose(archive);
    exit(1);
  }
  else
  {
    printf("Archive verified.\n");
  }
  
  fclose(archive);
}
