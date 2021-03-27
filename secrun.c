#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "dataTable.h"
#include "input.h"
#include "writeToFiles.h"


/*This function read a given file and wirte to new file
 *
 *parameters:
 *filename - the name of the file that we read from him
 *symbolTable - gets the symbol list from the first run
 *
 *returns - return 1 if it is comment, 0 if it not comment
 */
void secRun(char filename[], Node* symbolTable,DataNode *DataTable)
{ 
 int lineNum =0, index=0,indexLabel=0, IC = 100;
 char *filename1=NULL, line[MAX], *pline = NULL, *pdata = NULL, *Label = NULL, *functionName = NULL, *argument1 = NULL, *argument2 = NULL; 
 char *InitFile = NULL;
 FILE *fileToWirtep, *fileToReadp;
 char function[16][5] = {
 {"mov"},{"cmp"},{"add"},{"sub"},{"lea"},{"clr"},{"not"}, {"inc"}, {"dec"}, {"jmp"}, {"bne"}, {"jsr"}, {"red"}, {"prn"}, {"rts"},{"stop"}};
 
 fileToReadp = fopen(filename,"r");  
 filename1 = (char*)calloc(strlen(filename) +END_OF_FILE, sizeof(char));
 InitFile = (char*)calloc(strlen(filename) +END_OF_FILE, sizeof(char));
 if(filename1 == NULL || InitFile == NULL)
	{
	 fprintf(stderr,"out of memory\n");
	 exit(1);
	}
 strcpy(filename1,filename);
 strcpy(InitFile,filename);
 filename1[strlen(filename) - 3] = '\0';
 InitFile[strlen(filename) - 3] = '\0';  
 strcat(InitFile,END_EXT);
 remove(InitFile);
 free(InitFile);
 strcat(filename1,END_OB);  
 fileToWirtep = fopen(filename1,"a");
 fclose(fileToWirtep);
 	
 while(readMaxChars(line, fileToReadp,lineNum))
 	{ 
 	 lineNum++;  	 
 	 index = 0; 	 
 	 if(isComment(line) == TRUE)
 	 	;
 	 else if(notEmpty(line) == FALSE)
 	 	;	
 	 else if((strstr(line,".data") || strstr(line,".string") || strstr(line,".extern")))
	 	;	
	 else if((pdata = strstr(line,".entry ")) != NULL)
	 	{ 	 
	 	 pdata = pdata +ENTRY_LENGTH;	 	 
	 	 index = jumpSpaces(pdata,index);	 	 	 	 
	 	 Label =(char*)calloc(strlen(line)+1, sizeof(char));
	 	 if(Label == NULL)
		  	{
		  	 fprintf(stderr,"out of memory\n");
		  	 exit(1);
		  	}
	 	 while(isspace(pdata[index]) == FALSE)
	 	 	{
	 	 	 Label[indexLabel] = pdata[index];
	 	 	 indexLabel++;
	 	 	 index++;
	 	 	}
	 	 Label[indexLabel] = '\0';	 	  	 	 	 	 	  	 
	 	 symbolTable = insertEntryLabel(symbolTable,Label,lineNum);	 		 	 	 
	 	 free(Label);
	 	 indexLabel =0;
	 	}	 	
	 else
	 	{  	 		 
	 	 if((strchr(line,':')) != NULL)
	 	  	{
	 	  	 while(line[index] != ':')
	 	  	 	index++;
	 	  	 index++;		 			 		  	 
		  	}
		  pline = line +index;
		  index=0;
		  index = jumpSpaces(pline,index);
		  functionName = (char*)calloc(MAX, sizeof(char));
		  if(functionName == NULL)
		  	{
		  	 fprintf(stderr,"out of memory\n");
		  	 exit(1);
		  	} 
		  index = getFunc(index,pline,functionName);		  
	 	  index = jumpSpaces(pline,index);
		  argument1 = (char*)calloc(MAX, sizeof(char));
		  argument2 = (char*)calloc(MAX, sizeof(char));
		  if(argument1 == NULL || argument2 ==NULL)
		  	{
		  	 fprintf(stderr,"out of memory\n");
		  	 exit(1);
		  	}
		  splitToArguments(argument1,argument2,pline,index,lineNum);			   	  			 
		 for(index =0; index<16;index++)
		 {	 		  
		 	if(!strcmp(functionName,function[index]))
		 		{		 		 		 		 
		 		 free(functionName);
		 		 IC = ICCounter(filename1, IC);
		 		 writeFuncTofile(filename1,index);		 		 	 		 		 		 
		 		 if(index <= TWO_OPERAND_FUNCTION)
		 		 	{	   			 	 	  			 	  			 
					 writeMionTofile(filename1,argument1,argument2);					 
					 IC = ICCounter(filename1, IC);				 
					 writeTofile(argument1,  filename1, symbolTable,IC,lineNum);
					 IC = ICCounter(filename1, IC);					 
					 writeTofile(argument2,  filename1, symbolTable,IC,lineNum);					 
					 free(argument1);
					 free(argument2);					 	
					 break;		  	
			 		 }
			 	else if(index <=ONE_OPERAND_FUNCTION)
			 		{
			 		 writeMionTofile(filename1,NULL,argument1);	
			 		 IC = ICCounter(filename1, IC);		 		 
			 		 writeTofile(argument1,  filename1, symbolTable,IC,lineNum);
			 		 free(argument1);
			 		 free(argument2);
			 		 break;
			 		}	 		  		
			 	else
			 		{
			 		free(argument1);
			 		free(argument2);
			 		fileToWirtep = fopen(filename1,"a");
					fprintf(fileToWirtep, "0 A\n");		
					fclose(fileToWirtep);	
					break;	 		
			 		}
			 	}			 						
	      }	      	      
	   }   
	}   			
 fclose(fileToReadp); 
 printToFile(DataTable, filename1,IC); 
 free(filename1);
 	  
}
