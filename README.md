# Assembly-Compiler
This project is an assignment in a C programming course. The purpose of this project is to build an assembler for 16-instruction asssembly language, for an imaginary 12-bit CPU and 4096 bits of memory size. For any issues, questions and ideas turn to the Issues tab.

License: MIT License, Affect from March 26st, 2021

# Directory structure (Modules):

main.c - main function definition, argv & argc processing, single file processing.

firstrun.c - contains functions for check if the syntax of given file correct. 

secrun.c - contains function for processing a single line and print into file the hex value of the line.

input.c - contains useful function for processing code.

symbolTable.c - contains definition of table-like linked list data structure, for labels.

dataTable.c - contains definition of table-like linked list data structure, for data.

WriteToFiles.c - contains methods for writing output files: *.ob, *.ext and *.ent

This project was done by @Alonaki
