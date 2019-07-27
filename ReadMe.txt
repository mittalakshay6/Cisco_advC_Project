Supply the path to observations.bin, birds.bin, a new observations.bin file to be created after deletion of records, a new birds.bin file to be created after deletion of records, in the macros which have been left empty in the source code of all the .c files before compiling the project.

cleanup.c
=========

supply the command line args as follows:
1. path to observations.bin
2. path to birds.bin
3. path to new observations.bin file that will be created.

Since observations.bin is a large file, the code would take some time to completely execute.

menuSystem.c
============

since deletion operation would spoil the original file, every delete operation performed will create a new file, hence, path to this new file also needs to be supplied to the macro left empty in the source code. 
This has been done just to avoid altering the original file.

NOTE:
====
Since every file is a standalone file, they can be compiled separately using command line, or makefile can be used to compile all the files together.