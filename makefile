all: assembler

assembler: main.o firstrun.o symbolTable.o secrun.o input.o dataTable.o writeToFiles.o 
	gcc -ansi -Wall -pedantic -o assembler main.o firstrun.o symbolTable.o secrun.o input.o dataTable.o writeToFiles.o 
main.o: main.c main.h symbolTable.h dataTable.h input.h
	gcc -ansi -Wall -pedantic -c main.c
secrun.o: secrun.c symbolTable.h dataTable.h input.h writeToFiles.h
	gcc -ansi -Wall -pedantic -c secrun.c	
firstrun.o: firstrun.c symbolTable.h dataTable.h input.h writeToFiles.h
	gcc -ansi -Wall -pedantic -c firstrun.c
writeToFiles.o: writeToFiles.c writeToFiles.h symbolTable.h dataTable.h input.h
	gcc -ansi -Wall -pedantic -c writeToFiles.c			
dataTable.o: dataTable.c dataTable.h input.h
	gcc -ansi -Wall -pedantic -c dataTable.c
symbolTable.o: symbolTable.c symbolTable.h
	gcc -ansi -Wall -pedantic -c symbolTable.c		
input.o: input.c 
	gcc -ansi -Wall -pedantic -c input.c				
clean:
	rm *.o
