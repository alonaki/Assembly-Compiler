#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symbolTable.h"
#include "dataTable.h"
#include "input.h"
#include "writeToFiles.h"
#define IC_THREE 3
#define IC_TWO 2
#define IC_ONE 1

/*Checks if the given string if a valid assembly function
 *
 *parameters:
 *line - The current line 
 *lineNum - number of line in a file
 *
 *returns - The number of operand if given function, 1 opernad return 1, 2 two operand return 2, non operand return 3, if not valid assembly function return 0 
 */
int checkFunc(char func[],int lineNum)
{
 int index; 
 char function[16][5] = {
 {"mov"},{"cmp"},{"add"},{"sub"},{"lea"},{"clr"},{"not"}, {"inc"}, {"dec"}, {"jmp"}, {"bne"}, {"jsr"}, {"red"}, {"prn"}, {"rts"},{"stop"}};
for(index=0; index<16; index++)
 {
  if(strcmp(function[index], func) == 0)
  	{
  	 if(index <=TWO_OPERAND_FUNCTION)
  	 	return TWO_ARGUMENTS; /*Two oper*/
  	 else if(index <= ONE_OPERAND_FUNCTION)
  	 	return ONE_ARGUMENT; /*one oper*/
  	 else
  	 	return 0; /*non oper*/	
  	 }		
 }
 errorPrint(lineNum);
 printf("\n\t--> Unvalid func --> %s", func);
 return -1; 
}


/*Checks if the given line is valid
 *
 *parameters:
 *line - Current line to check
 *lineNum - number of line in a file
 *
 *returns - -1 if unvalid, 1 - for function with non operand, 2 - function with one operand, 3 - function with Two operands 
 */
int checkLine(char line[],int lineNum)
{
 int index =0, numOfArgument = 0, numofoper =0;
 char *func = NULL,*argument1 = NULL, *argument2 =NULL;
 index = jumpSpaces(line,index); 
 if(strchr(line,':'))
 	{ 	
 	if(checkLabel(line,lineNum) == FALSE)
 	 		  return UNVALID;  
 	else
 		{
 		 while(line[index] != ':')
 		 	index++;
 		 index++;	
 		 index = jumpSpaces(line,index);	
 		}
 	}	 
 func = (char*)calloc(MAX, sizeof(char));
 if(func == NULL)
  	{
  	 fprintf(stderr,"out of memory\n");
  	 exit(1);
  	}			
 index = getFunc(index,line,func);	
 if((numofoper = checkFunc(func,lineNum)) == UNVALID)
 	{
 	 free(func);
 	 return FALSE;
 	}  
 argument1 = (char*)calloc(MAX, sizeof(char));
 argument2 = (char*)calloc(MAX, sizeof(char));
 if(argument1 == NULL || argument2 == NULL)
  	{
  	 fprintf(stderr,"out of memory\n");
  	 exit(1);
  	}
 numOfArgument = splitToArguments(argument1,argument2,line,index,lineNum); 
 if(numofoper == numOfArgument)
 	{
	 if(numOfArgument == 2)
	 	{	 	 
	 	 if(twoOper(argument1,argument2,func,lineNum) == FALSE)
	 		{
	 		 free(argument1);
	 		 free(argument2);
	 		 free(func);	 		 
	 		 return FALSE;
	 		}
	 	 else
	 	 	{
	 	 	 free(argument1);
	 		 free(argument2);
	 		 free(func);
	 		 return IC_THREE;
	 	 	}		 
	 	}	
	 else if(numOfArgument == 1)
	 	{
	 	 if(oneOper(argument1,func,lineNum) == FALSE)
	 		{
	 		 free(argument1);
	 		 free(argument2);	 		 
	 		 free(func);	 		 
	 		 return FALSE;
	 		}
	 	 else
	 	 	{
	 	 	free(argument1);
	 	 	free(argument2);
	 	 	free(func);
	 		return IC_TWO;
	 	 	}	
	 	}	
	 else if(numofoper == 0)
	 	{
	 	 free(argument1);
	 	 free(argument2);
	 	 free(func);
	 	 return IC_ONE;
	 	}
	 return FALSE;	 	 
	 }
 else
 	{
 	 errorPrint(lineNum);
 	 if(numOfArgument == UNVALID)
 	 	printf("\n\t-->Invalid operand");
 	 else if(numofoper > numOfArgument)
	 	printf("\n\t-->Missing operand");
	 else 
	 	printf("\n\t-->Unexpect char after command");		
 	 free(argument1);
	 free(argument2);
	 free(func);
	 return FALSE;
	}									 
}

/*This function get line and check if the syntax of the line is currect (only data lines --> ".data 1,2,3")
 *
 *parameters:
 *line - the current line
 *lineNum - current line number in the file
 *
 *Returns - "0" - if the syntax worng, "1" - if the syntax good
 */
int checkSyntaxData(char line[],int lineNum)
{
 int index=0;  
 while(line[index] != '.')
 	index++;
 index = index +DATA_LENGTH;	
 index = jumpSpaces(line,index);
 while(line[index] != '\n' && line[index] != '\0')
 	{ 	 
 	 if(line[index] == '+' || line[index] == '-')
 	 	index++;
 	 if(line[index] == '+' || line[index] == '-')
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Cant be twice in a row sign '-' or '+'");
 	 	 return FALSE;
 	 	}  	
 	 else if(isdigit(line[index]) == FALSE)
 	 	{ 	 	 	 
 	 	 if(line[index] == ',')
 	 	 	{
 	 	 	 errorPrint(lineNum);
 	 	 	 printf("\n\t-->Can't be two comma in a row");
 	 	 	} 
 	 	 else
 	 	 	{
 	 	 	 errorPrint(lineNum);
 	 	 	 printf("\n\t-->Unvalid char in this line %s", line);
 	 	 	} 
 	 	 return FALSE;
 	 	} 			
 	 while(isdigit(line[index]) != FALSE)
 	 	index++;
 	 index = jumpSpaces(line,index); 	  
 	 if(index == END_OF_LINE)
 	 	return TRUE;	
 	 if(line[index] == ',')
 	 	index++;	 	
 	 else if(line[index] == '\n')
 	 	return TRUE; 	 		
 	 else
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Unvalid data line");
 	 	 return FALSE;
 	 	}  	 	
 	 index = jumpSpaces(line,index);				
 	}
 errorPrint(lineNum);
 printf("\n\t-->Expecet at number after ,");	
 return FALSE;	  
}

/*This function get line and check if the syntax of the line is currect (only string lines --> .string "hey!" , to print char like " --> \")
 *
 *parameters:
 *line - the current line
 *lineNum - current line number in the file
 *
 *Returns - "0" - if the syntax worng, "1" - if the syntax good
 */
int checkSyntaxString(char line[], int lineNum)
{
 int index=0,countLetters=0;
 while(line[index] != '.')
 	index++;
 index = index +STRING_LENGTH;	 
 while(isspace(line[index]) != FALSE)
 	{
 	 if(line[index] == '\n')
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Expect word after command");
 	 	 return FALSE;
 	 	} 
  	 index++;
  	}   
 if(line[index] == '\"')
 	{ 	 
 	 index++;
 	 while(line[index] != '\"' || (line[index] == '\"' && line[index-1] == '\\'))
 	 	{
 	 	 if(line[index] == '\n')
 	 	 	{
 	 	         errorPrint(lineNum);
		         printf("\n\t-->The string must end with \"");	
		         return FALSE;
 	 	 	}
 	 	 index++;
 	 	 countLetters++;
 	 	}
 	 index++; 	 
 	 /*check after string for unexpeted char*/
 	 while(isspace(line[index]) != FALSE)
	 	{
	 	 if(line[index] == '\n')
	 	 	return TRUE;
	  	 index++;
	  	}
	  errorPrint(lineNum);
	  printf("\n\t-->Unexpect char after string");	
	  return FALSE;
	 }
 else
 	{
 	 errorPrint(lineNum);
 	 printf("\n\t-->String must start with \"");
 	 return FALSE;	
 	}   	   	   	
}

/*Checks if the given entry line is valid
 *
 *parameters:
 *line - Current line to check
 *lineNum - number of line in a file
 *
 *returns - 0 if unvalid 1 if valid
 */
int checkEntry(char line[],int lineNum)
{
 int index=0;
 if(strchr(line,':') != NULL)
 	{
 	 if(checkLabel(line,lineNum) == FALSE)
 		return FALSE;
 	 while(line[index] != ':')
 	 	index++;
 	 index++;		
 	}	
index = jumpSpaces(line,index);
if(line[index] != '.')
  	{
  	 errorPrint(lineNum);
 	 printf("\n\t-->Unvalid string before .entry");
 	 return FALSE;
 	} 
 index = index + ENTRY_LENGTH;
 index = jumpSpaces(line,index);
if(isalpha(line[index]))
	index++;
else
 	{
 	 errorPrint(lineNum);
 	 printf("\n\t-->Unvalid label needs to starts with letter");
 	 return FALSE;
 	} 	 
while(isdigit(line[index]) || isalpha(line[index]))
 	index++;
if(jumpSpaces(line,index) != END_OF_LINE)
 	{
 	 errorPrint(lineNum);
 	 printf("\n\t-->Unvalid label");
 	 return FALSE;
 	} 			
 return TRUE;	
}

/*Checks if the given extern line is valid
 *
 *parameters:
 *line - Current line to check
 *lineNum - number of line in a file
 *
 *returns - 0 if unvalid 1 if valid
 */
int checkExtern(char line[],int lineNum)
{
 int index=0;
 if(strchr(line,':') != NULL)
 	{
 	 if(checkLabel(line,lineNum) == FALSE)
 		return FALSE;
 	 while(line[index] != ':')
 	 	index++;
 	 index++;		
 	}	
index = jumpSpaces(line,index);
if(line[index] != '.')
  	{
  	 errorPrint(lineNum);
 	 printf("\n\t-->Unvalid string before .extern");
 	 return FALSE;
 	} 
 index = index + EXTERN_LENGTH;
 index = jumpSpaces(line,index);
if(isalpha(line[index]))
	index++;
else
 	{
 	 errorPrint(lineNum);
 	 printf("\n\t-->Unvalid label needs to starts with letter");
 	 return FALSE;
 	} 	 
while(isdigit(line[index]) || isalpha(line[index]))
 	index++;
if(jumpSpaces(line,index) != END_OF_LINE)
 	{
 	 errorPrint(lineNum);
 	 printf("\n\t-->Unvalid label");
 	 return FALSE;
 	} 			
 return TRUE;	
}

/*This function read a given file and check if all lines in file are assmebly command , and also bulid a symbol table for the file
 *
 *parameters:
 *filename - the name of the file we want to check
 *
 *Returns - Symbol table for all labels that defined in the file (expet entry's label - only in the second run)
 */
Node* firstRun(char filename[],DataNode *DataTable)
{ 
 int IC =100,numOfIC=0,preDC=0, lineNum=1,index=0, labelIndex =0;  
 char line[MAX], *filenameOB = NULL, *Label =NULL;
 Node* symbolTable = NULL; 
 FILE *fileToReadp = NULL, *fileToWirtep = NULL; 
 
 DataTable = writeDataToList(DataTable,line,INITIALIZATION); /*restert DC for new file*/ 
 filenameOB =(char*)calloc(strlen(filename) +4, sizeof(char));
 if(filenameOB == NULL)
	{
	 fprintf(stderr,"out of memory\n");
	 exit(1);
	}
 strcpy(filenameOB,filename);  
 strcat(filenameOB,END_OB); 
 strcat(filename,END_AS);
 fileToReadp = fopen(filename,"r+");  
 if(fileToReadp == NULL)
	{
	 printf("--- Something goes worng ---\n");
	 printf("File not exist or file ending without '.as' --> %s\n",filename);
	 printf("For run properly (without '.as') --> ./assembler filename\n");
	 free(filenameOB);
	 goto END;	 
	}
	
 while(readMaxChars(line, fileToReadp,lineNum))
 	{	 
 	 labelIndex =index =0;   	 	 	
 	 if(notEmpty(line) == FALSE)
 	 	;
 	 else if(isComment(line) == TRUE)
 	 	;	 	 	 	 		  
	 else if(strstr(line,".data") != NULL || strstr(line,".string") != NULL)
		{ 
		 if(strstr(line,".data ") != NULL || strstr(line,".string ") != NULL)
		 	{		  		 
			 if(strchr(line,':') && checkLabel(line,lineNum) == TRUE)
				 	 symbolTable = defineLabel(line,symbolTable,getDC(DataTable),lineNum);
			 else if(strchr(line,':') == NULL)
			 	{
			 	 while(line[index] != '.')
			 	 	{
			 	 	 if(isspace(line[index]) == 0)
			 	 	 	break; 
			 	 	 index++;	
			 	 	}
			 	 if(line[index] != '.')
			 	 	{
			 	 	 errorPrint(lineNum);			 	 	 
			 	 	 printf("\n\t-->Label needs to end with :");
			 	 	}	
			 	} 	 			 	 					 		
			 if((strstr(line,".data ") != NULL && checkSyntaxData(line,lineNum) == TRUE) || (strstr(line,".string ") != NULL && checkSyntaxString(line,lineNum) ==TRUE))
			 	{		
			 	 preDC = getDC(DataTable);		 		 				 	 	 
			 	 DataTable = writeDataToList(DataTable,line,lineNum);
			 	 IC = IC + getDC(DataTable) - preDC;	
			 	} 	
			 }
		 else
		 	{
		 	 errorPrint(lineNum);
		 	 printf("\n\t-->Expect at laset one argument"); 
		 	} 	 		 		 	 
	 	}
	 else if((strstr(line,".entry")) != NULL)
	 	{
	 	 if(checkEntry(line,lineNum) == TRUE)
	 		;	
	 	}	
	 else if(strstr(line,".extern") != NULL)		
	 	{
	 	 if(checkExtern(line,lineNum) == TRUE)
	 	 	{
	 	 	 while(line[index] != '.')
	 	 	 	index++;
	 	 	 index = index + EXTERN_LENGTH;				 	 
		 	 index = jumpSpaces(line,index);	 	 	 	 
		 	 Label = (char*)calloc(strlen(line)+1, sizeof(char));
		 	 if(Label == NULL)
				{
				 fprintf(stderr,"out of memory\n");
				 exit(1);
				}
		 	 while(isspace(line[index]) == FALSE)
		 	 	{		 	 	 
		 	 	 Label[labelIndex] = line[index];
		 	 	 labelIndex++;
		 	 	 index++;
		 	 	}		 	
			 Label[labelIndex] = '\0';	
			 symbolTable = insert(symbolTable, Label, IC, "external", lineNum); 	 		 		 	 	 	 	 
		 	 free(Label);	 	 	 		 	 
		 	}	
		} 	
 	 else if(notEmpty(line) == TRUE && (numOfIC = checkLine(line,lineNum)) != FALSE)
 	 	{ 
 	 	 if(strchr(line,':'))
 	 	 	 symbolTable = defineLabel(line,symbolTable,IC,lineNum); 			
	         IC = IC + numOfIC;		  	 
		}
	 else	 	
	 	 errorFlag = TRUE;	 		 	 		
	 lineNum++;	 	 				
	}   	
 fclose(fileToReadp);	
 if(errorFlag == FALSE)
 	{
 	 fileToWirtep = fopen(filenameOB,"w"); 
 	 fprintf(fileToWirtep,"  %d\t%d\n", (IC -100- getDC(DataTable)), getDC(DataTable));
 	 fclose(fileToWirtep);
 	} 
 free(filenameOB); 
 update_symbolTable(symbolTable,IC-getDC(DataTable));  
 return symbolTable; 	 		
 END:
 errorFlag = TRUE;
 return symbolTable; 
}
