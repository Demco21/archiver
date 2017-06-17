To execute, command line arguments can be as follows:
archiver -a archivename file1 ... fileN
archiver -u archivename
archiver -l archivename
archiver -v archivename file1 ... fileN
(ex. archiver -a myarchive.bin file1.txt file2.txt file3.txt)

-a, -u, -l and -v are flags that tell the program what to do.
archivename is the name you want your archive to be created as.
file1 .. fileN is a list of files that you want to be archived.
Each file should be separated by a space.

The following is an explanation of each flag:  
-a flag archives the given files as archivename
-u unarchives the given archive into their respective files
-l list some facts about the given archive
-v verifies that the archive is not corrupt
