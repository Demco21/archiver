#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constheader.h"
#include "archprototypes.h"
#include "infoprototypes.h"

  /* command line arguments will be as follows:
     archiver -a archivename file1 ... fileX
     archiver -u archivename
     archiver -l archivename
     archiver -v archivename file1 ... fileX
     
     -a flag archives the given files
     -u unarchives the given archive
     -l list some facts about the given archive
     -v verifies that the archive is not corrupt */


void main(int argc, char **argv)
{
  
  /* there will be 'X' number of files specified in the command line
  X = (the total number of arguements) - (the 3 arguments that arent files) */
  
  int numOfFiles = argc - NON_FILE_ARGS;
  char ** filenames = &argv[3];
  char * archiveName = &argv[2][0];
 
  //handles -a flag
	if(numOfFiles > 0 && strcmp(argv[1], "-a") == 0)
	{
    archive(filenames, numOfFiles, archiveName);
	}	
  //handles -u flag
  else if(numOfFiles == 0 && strcmp(argv[1], "-u") == 0)
  {
    unarchive(archiveName);
	}	
  //handles -l flag
  else if(numOfFiles == 0 && strcmp(argv[1], "-l") == 0)
  {
    listInfo(archiveName);
  }
  //handles -v flag
  else if(numOfFiles > 0 && strcmp(argv[1], "-v") == 0)
  {
    verify(filenames, numOfFiles, archiveName);
  }
  else
  {
    fprintf(stderr, "ERROR: Please enter command line argument as follows: archiver -flag archivename file1 ... fileX\n");
    fflush(stderr);
    exit(1);
  }
}
