#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "dataTable.h"
#include "input.h"

/*get label and check which attribute it is
 *
 *parameters:
 *sym - given label to serach
 *head - pointer to the start of symbol table
 *
 *Returns - 'R' if it external attribute, 'E' if it entry attribute
 */
 char getARE(Node* head, char sym[])
 {
  while(head != NULL)
  {
   if(strcmp(head->symbol,sym) == 0)
   	{
   	 if(strcmp(head->attribute,"external"))
   		return 'R';
   	 if(strcmp(head->attribute,"entry"))
   		return 'E';
   	}		 	
   head = head->next;   	
  }
  return 'A';
 }

/*gets number (string) and filename and print the hex value to file
 *
 *parameters:
 *number - the number we want to print to file
 *filename - the name of the file that will print into
 */
void itoHex(char number[], char filename[])
{  
int num; 
FILE *filep = NULL;
 num = atoi(number);  
 if(num < 0)
 	num = num + MEMORY_SIZE;  	
  filep = fopen(filename, "a");
 fprintf(filep, "%03X ", num);     		
 fclose(filep); 
}

/*gets register (string) and filename and print the hex value of register to file
 *
 *parameters:
 *number - the number we want to print to file
 *filename - the name of the file that will print into
 */
void regtoHex(char number[], char filename[])
{  
unsigned int tmp=1;
int num, index;  
FILE *filep = NULL;
 num = atoi(number);
 for(index =0; index< num; index++)
 	tmp = tmp*2;  
 filep = fopen(filename, "a"); 
 fprintf(filep, "%03X ", tmp); 		
 fclose(filep); 
}


/*create a file with the name of the given file .ext , with all extern label that in use
 *
 *parameters:
 *filename - current filename
 *label - the label to write into file
 *IC - instarction counter
 *head - pointer to the stmbol table
 *
 */
void printToFileExt(char filename[],char label[], int IC, Node* head)
{
 char Newfilename[MAX];
 FILE *fileToWirtep;
 strcpy(Newfilename,filename);
 Newfilename[strlen(Newfilename) - 3] = '\0';
 strcat(Newfilename,END_EXT); 
 while(head != NULL)
  {
   if(strcmp(head->symbol,label) == 0 && strcmp(head->attribute,"external") == 0)
   	{
   	 fileToWirtep = fopen(Newfilename,"a");
   	 if(IC >= THREE_DIGIT)
 	 	fprintf(fileToWirtep, "%s\t%d\n", label,IC); 	
 	 else
 	 	fprintf(fileToWirtep, "%s\t0%d\n", label,IC); 	 
 	 fclose(fileToWirtep);
   	}
   head = head->next;   	
  }   
} 



/*create a file with the name of the given file .ent , with all entry label that in use
 *
 *parameters:
 *filename - current filename
 *head - pointer to the symbol table
 *
 */
void printToEnt(char filename[], Node* head)
 {  
  int wroteToFile = FALSE;
  char Newfilename[50];
  FILE *fileToWirtep;
  
    
  strcpy(Newfilename,filename);
  Newfilename[strlen(Newfilename) - 3] = '\0';
  strcat(Newfilename,END_ENT);  
  fileToWirtep = fopen(Newfilename,"w");
  fclose(fileToWirtep);  
  while(head != NULL)
  {
   if(strcmp(head->attribute2,"entry") == 0)
   	{   	 
   	 fileToWirtep = fopen(Newfilename,"a");
   	 if(head->value >= THREE_DIGIT)
 	 	fprintf(fileToWirtep, "%s\t%d\n", head->symbol,head->value); 
 	 else
 	 	fprintf(fileToWirtep, "%s\t0%d\n", head->symbol,head->value);		 
 	 fclose(fileToWirtep);
 	 wroteToFile = TRUE; 	
   	}
   head = head->next;   	
  }   	
  if(wroteToFile == FALSE)
  	remove(Newfilename);
}

/*Print to end of file the data code 
 *
 *parameters:
 *head - Data list that need to be print in end of file
 *filename - the name of the current file will be print into
 *IC - instraction counter
 *
 */
void printToFile(DataNode* head, char filename[], int IC)
{
 int tmpIC, tmpValue;
 FILE *fileToWirtep; 
 fileToWirtep = fopen(filename,"a");
 if(head != NULL) 
 	head = head->next;
 while(head != NULL)
  	{
  	 tmpIC = IC + head->DC;
  	 tmpValue = head->value;
  	 if(tmpValue < 0)
  	 	tmpValue = tmpValue + MEMORY_SIZE;
  	 if(tmpIC < THREE_DIGIT)
  	 	fprintf(fileToWirtep, "0%d\t%03X A\n",tmpIC, tmpValue);
  	 else
  	 	fprintf(fileToWirtep, "%d\t%03X A\n",tmpIC,tmpValue);
  	 head = head->next;
  	} 
  fclose(fileToWirtep);
}

/*gets argument and print to file
 *
 *parameters:
 *argument - the argument that we want to write to the file
 *filename - the name of the file that will print into
 *symbolTable - the symbol Table from the first run
 *IC - instruction counter
 *lineNum - the number of line in the orignal file
 */
void writeTofile(char argument[], char filename[], Node* symbolTable,int IC, int lineNum)
{
 FILE *fileToWirtep;
 int value,index=0;
 char ch, *tmpArg = NULL;  
 index = jumpSpaces(argument,index);
 tmpArg = argument +index; 
 if(strchr(tmpArg, '#') != NULL)
 	{
 	 while(tmpArg[index] != '#')
 	 	tmpArg++; 
 	 tmpArg++;		 		 	 
 	 itoHex(tmpArg, filename);
 	 fileToWirtep = fopen(filename,"a");
 	 putc('A', fileToWirtep);
 	 putc('\n', fileToWirtep);
 	 fclose(fileToWirtep);	 
 	} 	
 else if(strchr(tmpArg, '%') != NULL)
 	{	 
 	 tmpArg++; 
 	 value = getValue(symbolTable , tmpArg,lineNum);
 	 value = value - IC+1;
 	 if(value < 0)
 		value = value + MEMORY_SIZE;
 	 fileToWirtep = fopen(filename,"a");	
 	 fprintf(fileToWirtep, "%03X ", value);
 	 putc('A', fileToWirtep);
 	 putc('\n', fileToWirtep); 
 	 fclose(fileToWirtep);	 	
 	}
 else if(oneOperRegDir(tmpArg) == TRUE)
 	{
 	 tmpArg++;
 	 regtoHex(tmpArg, filename);
 	 fileToWirtep = fopen(filename,"a");
 	 putc('A', fileToWirtep);
 	 putc('\n', fileToWirtep);
 	 fclose(fileToWirtep);		
 	}	
 else if((value = getValue(symbolTable,tmpArg,lineNum)) != UNVALID)
 	{ 
 	 printToFileExt(filename, tmpArg,IC-1,symbolTable); 	 
 	 fileToWirtep = fopen(filename,"a");
 	 fprintf(fileToWirtep, "%03X ", value);
 	 ch = getARE(symbolTable,tmpArg);	 
 	 putc(ch , fileToWirtep);	
 	 putc('\n', fileToWirtep);
 	 fclose(fileToWirtep);
 	}			
}

/*gets function number and print to file
 *
 *parameters:
 *filename - the name of the file that will print into
 *funcIndex - the function number
 */
void writeFuncTofile(char filename[], int funcIndex)
{ 
 FILE *fileToWirtep;
 struct funct{
 int opcode;
 int func;
 };
struct funct function[16] = {{0,0},{1,0},{2,10},{2,11},{4,0},{5,10},{5,11},{5,12}, {5,13}, {9,10}, {9,11}, {9,12}, {12,0}, {13,0}, {14,0},{15,0}};
fileToWirtep = fopen(filename,"a");
fprintf(fileToWirtep, "%X%X",function[funcIndex].opcode,function[funcIndex].func);		
fclose(fileToWirtep);
}

/*gets two arguments and wirte the current addressing ( '0' - immediate
 *
 *parameters:
 *filename - the name of the file that will print into
 *argument1 - the first argument
 *argument2 - the second argument
 */
void writeMionTofile(char filename[], char argument1[], char argument2[])
{ 
 int DestionBits, SourceBits;
 FILE *fileToWirtep; 
 
 if(argument1 == NULL)
 	DestionBits = IMMEDIATE;
 else if(strchr(argument1,'#') != NULL)
 	DestionBits = IMMEDIATE;
 else if(strchr(argument1,'%') != NULL)
 	DestionBits = RELATIVE;
 else if(oneOperRegDir(argument1) == TRUE)
 	DestionBits = REGISTER_DIRECT;
 else
 	DestionBits = DIRECT;
 		
 if(strchr(argument2,'#') != NULL)
 	SourceBits = IMMEDIATE;
 else if(strchr(argument2,'%') != NULL)
 	SourceBits = RELATIVE;
 else if(oneOperRegDir(argument2) == TRUE)
 	SourceBits = REGISTER_DIRECT;
 else
 	SourceBits = DIRECT;	 	
 DestionBits = 4*DestionBits;			
 fileToWirtep = fopen(filename,"a"); 
 fprintf(fileToWirtep, "%X",DestionBits+SourceBits);	
 fprintf(fileToWirtep, " A\n");	
 fclose(fileToWirtep);
}

