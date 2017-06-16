archiver: main.o archive.o archiveinfo.o filesize.o
	gcc main.o archive.o archiveinfo.o filesize.o -o archiver

main.o: main.c constheader.h archprototypes.h infoprototypes.h
	gcc -c main.c

archive.o: archive.c sizeprototypes.h
	gcc -c archive.c

archiveinfo.o: archiveinfo.c
	gcc -c archiveinfo.c

filesize.o: filesize.c
	gcc -c filesize.c

clean:
	rm -f *.o core archiver
