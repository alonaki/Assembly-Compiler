#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include "dataTable.h"
#include "input.h"
#include "main.h"
 
int errorFlag;

int main(int argc, char *argv[])
{
 int index =1;
 char filename[MAX-1];   
 Node* symbolTable = NULL;
 DataNode *DataTable = NULL;
 /*Check how many arguments gets --> Expect one argument*/
 if(argc == 1)
 	{
 	 printf("\nOne argument expected.\n");
 	 exit(0); 
 	}
 while(index < argc)
 {
  DataTable = (DataNode*)calloc(1,sizeof(DataNode));
  if(DataTable == NULL)
	{
	 fprintf(stderr,"out of memory\n");
	 exit(1);
	}
  errorFlag =FALSE;	
  strcpy(filename, argv[index]); 
  printf("\n\n ---- First run on file %s.as starting ----\n\n", filename);
  symbolTable = firstRun(filename,DataTable);    
  if(errorFlag == FALSE)
 	{ 	 
 	 printf("\n\t   ---- First run is OK  ----\n");
 	 printf("\n ---- Second run on file %s starting ----\n\n", filename);
 	 secRun(filename , symbolTable,DataTable); 
 	 if(errorFlag == TRUE)
 	 	{
 	 	 printf("\n\t ---- Warning!! second run is found some problems  ----\n");
 	 	 cleanUp(filename);
 	 	} 
 	 else
 	 	{
 	 	 printf("\n\t---- Files created successfully ----\n");		 		
 	 	 printToEnt(filename,symbolTable); 
 	 	} 
 	}
  else
  	cleanUp(filename);	  		 	
  destroy(&symbolTable);	 
  destroyD(&DataTable);
  index++;	
 } 
 return 0;
 }
 

