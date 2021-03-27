#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "input.h"

int errorFlag = FALSE;

/*check if given label is a saved word
 *
 *parameters:
 *label - the label that need to be checked
 *
 *Returns - TRUE - if not saved word , FALSE - if it a saved word 
 */
int SavedWord(char label[])
{
 int index=0;
 char Uvalidlabel[28][7] = {{"mov"},{"cmp"},{"add"},{"sub"},{"lea"},{"clr"},{"not"}, {"inc"}, {"dec"}, {"jmp"}, {"bne"}, {"jsr"}, {"red"}, {"prn"}, {"rts"},{"stop"},{"r0"},{"r1"},{"r2"},{"r3"},{"r4"},{"r5"},{"r6"},{"r7"},{"data"},{"string"},{"entry"},{"extern"}}; 
 for(index =0; index<23; index++)
 	{
 	 if(strcmp(Uvalidlabel[index],label) == 0)
 	 	{ 	 	 
 	 	 return FALSE;
 	 	} 
 	}  
 return TRUE; 	
} 

/*In case of error in first or second run delete all files that was made
 *
 *parameters:
 *filename - the file name of the file that failed
 */
void cleanUp(char filename[])
{
 char *filenameEXT = NULL, *filenameENT = NULL, *filenameOB = NULL;
 
 filenameEXT = (char*)calloc(strlen(filename) +END_OF_FILE, sizeof(char));
 filenameENT = (char*)calloc(strlen(filename) +END_OF_FILE, sizeof(char));
 filenameOB = (char*)calloc(strlen(filename) +END_OF_FILE, sizeof(char));
 if(filenameEXT == NULL || filenameENT == NULL || filenameOB == NULL)
  	{
  	 fprintf(stderr,"out of memory\n");
  	 exit(1);
  	} 
 strcpy(filenameEXT,filename);
 strcpy(filenameENT,filename);
 strcpy(filenameOB,filename);
 filenameEXT[strlen(filename) - 3] = '\0';
 filenameENT[strlen(filename) - 3] = '\0'; 
 filenameOB[strlen(filename) - 3] = '\0';   
 /*delete .ext file*/
 strcat(filenameEXT,END_EXT);
 remove(filenameEXT);
 free(filenameEXT);
 /*delete .ent file*/
 strcat(filenameENT,END_ENT);
 remove(filenameENT);
 free(filenameENT);
 /*delete .ob file*/
 strcat(filenameOB,END_OB);
 remove(filenameOB);
 free(filenameOB); 
}

/*checks if line is a comment
 *if it is return TRUE, if not return FALSE
 *
 *parameters:
 *line - currnet line
 */
int isComment(char line[])
{
 int index=0; 
 while(isspace(line[index]) != 0)
 	index++;
 if(line[index] == ';')
 	return TRUE;	
 return FALSE;		
}

/*function that jump to the next non space char
 *
 *parameters:
 *line - The current line 
 *from - From which index to start
 *
 *returns - The index of the next non space char, if reach the end of line return END_OF_LINE 
 */
int jumpSpaces(char line[],int from)
{
 int index = from;
 while(isspace(line[index]) != FALSE)
 	{ 	 
 	 if(line[index] =='\n')
 	 	return END_OF_LINE;
 	 index++; 	 
 	} 
 if(line[index] == '\0')
 	return END_OF_LINE;		
 return index;	
}

/*Print to client the number of the line that was worng
 *
 *parameters:
 *lineNum - Current line number
 */
void errorPrint(int lineNum)
{
 printf("\nError on line (%d):",lineNum);
 errorFlag = TRUE;
}

/*function that get line and save the name of the function in givan "func"
 *
 *parameters:
 *line - The current line 
 *index - From which index to start
 *func - the function name saved in "func"
 *
 *returns - the index of the end of function in line
 */
int getFunc(int i, char line[], char func[])
{
 int index = i, indexFunction = 0;
 while(isspace(line[index]) == FALSE)
 	{
 	 func[indexFunction] = line[index];
 	 indexFunction++;
 	 index++;
 	} 
 func[indexFunction] = '\0'; 
 return index;	
}

/*function that get line and save the two argument into variables
 *
 *parameters:
 *line - The current line 
 *index - From which index to start
 *argument1 - the first argument save in this variable
 *argument2 - the second argument save in this variable 
 *lineNum - the current line number in file
 *
 *returns - "NON_ARGUMENT" - if non varibales, "ONE_ARGUMENT" - if one variable, "TWO_ARGUMENTS" - if two variables, "UNVALID" - if there is an error
 */
int splitToArguments(char *argument1,char *argument2, char line[],int index, int lineNum)
{
 int i=index,indexArgument =0; 
 i = jumpSpaces(line,i);
 if(i == END_OF_LINE)
 	return NON_ARGUMENT; 			
 while(isspace(line[i]) == FALSE)
 	{
 	 if(line[i] == ',')
 	 	break;
 	 argument1[indexArgument] = line[i];
 	 i++;
 	 indexArgument++;
 	}
 argument1[indexArgument] = '\0';
 i = jumpSpaces(line,i);
 if(i == END_OF_LINE)
 	return ONE_ARGUMENT; 		
 if(line[i] == ',')
 	i++;
 indexArgument =0;	
 i = jumpSpaces(line,i);
 if(i == END_OF_LINE)
 	return UNVALID;
 while(isspace(line[i]) == FALSE)
 	{ 	 
 	 argument2[indexArgument] = line[i]; 	 
 	 i++;
 	 indexArgument++;
 	}
 argument2[indexArgument] = '\0';	
 i = jumpSpaces(line,i); 
 if(i == END_OF_LINE)
 	return TWO_ARGUMENTS;
 else 	
 	 return UNVALID; 			
}



/*Write to file the IC
 *
 *parameters:
 *filename - The name of the file that we want to write the IC
 *IC - The current IC
 *
 *returns - The update IC
 */
int ICCounter(char filename[], int IC)
{
 int index = 0, tmp, limit=0, num;
 char c;
 int tenP = 1000; 
 FILE *fileToWirtep;
 fileToWirtep = fopen(filename,"a");
 num = IC;
 if(IC < THREE_DIGIT)
 	{
 	 putc('0',fileToWirtep);
 	 limit = 1;
 	 tenP = tenP/10;
 	} 
 for(index =3; index>=limit; index--)
 	{
 	 tmp = num/tenP;
 	 num = num %tenP;
 	 c = 0;
 	 c = tmp +ASCCI_VALUE;
 	 putc(c,fileToWirtep);
 	 tenP = tenP/10;
 	} 
 putc('\t',fileToWirtep);	
 fclose(fileToWirtep);
 IC++;
 return IC;
}


/*Checks if given line is empty
 *
 *parameters:
 *line - The line to check
 *
 *returns - FALSE if not empty, and TRUE if empty
 */
int notEmpty(char line[])
{
int index=0;
if(line == NULL)
	return FALSE;	
 while(isspace(line[index]) != FALSE)
 	{
 	 if(line[index] == '\n')
 	 	return FALSE;
 	 index++;
 	}  
 return TRUE;
}

/*Checks if the current line have a valid label
 *
 *parameters:
 *line - The current line 
 *lineNum - number of line in a file
 *
 *returns - FALSE if it unvalid label , TRUE if valid label
 */
int checkLabel(char label[],int lineNum)
{
 int index=0;
 index = jumpSpaces(label,index);
 if(isalpha(label[index]))
 	index++;
 else
 	{
 	 errorPrint(lineNum);
 	 printf("\n\t-->Unvalid label needs to starts with letter --> %s", label);
 	 return FALSE;
 	} 	 
 while(isdigit(label[index]) || isalpha(label[index]))
 	index++;
 if(label[index] == ':')
 	return TRUE;
 errorPrint(lineNum);	
 printf("\n\t-->Unvalid label --> %s", label);	
 return FALSE;		 
}


/*Checks if the given argument is a valid immedate operand
 *
 *parameters:
 *argument - argument that need to check 
 *lineNum - number of line in a file
 *
 *returns - FALSE if unvalid, TRUE if valid 
 */
int oneOperImm(char argument[],int lineNum)
{
 int index =0,indexArgument=0,Minous=0,tmpNumInt; 
 char tmpNum[5]; 
 if(argument[index] == '#')
 	{ 	 
 	 index++;
 	 if(argument[index] != '-' && argument[index] != '+' && isdigit(argument[index]) == FALSE)
 	 	 return FALSE; 	 	
 	 else if(argument[index] == '-' || argument[index] == '+')
	  	{
	  	 if(argument[index] == '-')
	  	 	Minous =1;
	  	 index++;
	  	 /*unvalid char after "-"*/
	  	 if(isdigit(argument[index]) == FALSE)
	  	 	return FALSE;	  	 	
	  	}			
	 while(isdigit(argument[index]) != FALSE)		  	 		  	 
		  	 {
		  	  tmpNum[indexArgument] = argument[index];
		  	  indexArgument++;
		  	  index++;
		  	 }
	 tmpNum[indexArgument] = '\0';				 	 				 	 	
	 tmpNumInt = atoi(tmpNum);
	 if(Minous ==1)
	 	tmpNumInt = tmpNumInt *(-1); 			 	 			 			  	  
	  /*check if no unexpect char after command*/	
	  while(isspace(argument[index]) != FALSE || argument[index] == '\0')
	 	{
	 	 if(argument[index] == '\0')
		 	return TRUE;
	 	 index++;
	 	}
 	  return FALSE;
 	  	 	  		
 	}
return FALSE; 	
}


/*Checks if the given argument is a valid relative operand (%next)
 *
 *parameters:
 *argument - argument that need to check 
 *lineNum - number of line in a file
 *
 *returns - FALSE if unvalid, TRUE if valid 
 */
int oneOperRel(char argument[],int lineNum)
{
 int index =0; 
 if(argument[index] == '%')
 	{
 	  index++;
 	  if(isalpha(argument[index]) != FALSE)
 	  	{
 	  	 index++;
 	  	 while(isalpha(argument[index]) != FALSE || isdigit(argument[index]) != FALSE)
 	  	 	index++; 	  	 	
 	  	 while(isspace(argument[index]) != FALSE || argument[index] == '\0')
 	  	 	{
		 	 if(argument[index] == '\0')
			 	return TRUE;
		 	 index++;
		 	}
	 	 return FALSE;	
 	  	} 
 	  else
 	  	return FALSE;
 	}	
return FALSE;
}

/*Checks if the given argument is a valid register operand 
 *
 *parameters:
 *argument - argument that need to check 
 *lineNum - number of line in a file
 *
 *returns - FALSE if unvalid, TRUE if valid 
 */
int oneOperRegDir(char argument[])
{
 int index=0;
 char registers[8][3] = {{"r0"},{"r1"},{"r2"},{"r3"},{"r4"},{"r5"},{"r6"},{"r7"}}; 
 for(index =0; index<8; index++)
 	{
 	 if(strcmp(registers[index],argument) == 0)	 
 	 	 return TRUE;
 	}  
 return FALSE; 	
}

/*Checks if the given argument is a valid label operand 
 *
 *parameters:
 *argument - argument that need to check 
 *lineNum - number of line in a file
 *
 *returns - FALSE if unvalid, TRUE if valid 
 */
int oneOperLabel(char argument[], int lineNum)
{
 int index = 0; 	  
 if(SavedWord(argument) == FALSE)
 	return FALSE; 		
 if(isalpha(argument[index]) != FALSE)
 	{
 	 index++;
 	 while(isalpha(argument[index]) != FALSE || isdigit(argument[index]) != FALSE)
 	 	 index++; 	 	
 	 while(isspace(argument[index]) != FALSE || argument[index] == '\0')
 		{
 	 	if(argument[index] == '\0')
	 		return TRUE;
 	 	index++;
 		}  	 	
 	 return FALSE;
 	}
 else
 	 return FALSE;	
}

/*Checks if the given argument is match to function
 *
 *parameters:
 *argument - argument that need to check 
 *func - function name
 *lineNum - number of line in a file
 *
 *returns - FALSE if unvalid, TRUE if valid 
 */
int oneOper(char argument[],char func[],int lineNum)
{
 char prn[4] = "prn", jmp[4] ="jmp", bne[4] = "bne", jsr[4] = "jsr"; 
 if(strcmp(func,prn) == 0)
 	{
 	 if(oneOperImm(argument,lineNum) == TRUE || oneOperRegDir(argument) == TRUE || oneOperLabel(argument,lineNum) == TRUE)
 	 	return TRUE;
 	 else 
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Unvalid operand");
 	 	 return FALSE;
 	 	} 
 	}
 else if (strcmp(func,jmp) == 0 ||  strcmp(func,bne) == 0 || strcmp(func,jsr) == 0)
 	{
 	 if(oneOperRel(argument,lineNum) == TRUE || oneOperLabel(argument,lineNum) == TRUE)
 	 	return TRUE;
 	 else 
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Unvalid operand");
 	 	 return FALSE;
 	 	}
 	}
 else
 	{
 	 if(oneOperRegDir(argument) == TRUE || oneOperLabel(argument,lineNum) == TRUE)
 	 	return TRUE;
 	 else 
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Unvalid operand");
 	 	 return FALSE;
 	 	}
 	}
}

/*Checks if the given arguments are match to function valid operand
 *
 *parameters:
 *argument1 - the first argument that need to check 
 *argument2 - the second argument that need to check 
 *func - function name
 *lineNum - number of line in a file
 *
 *returns - FALSE if unvalid, TRUE if valid 
 */
int twoOper(char argument1[], char argument2[], char func[],int lineNum)
{
 if(strcmp(func,"cmp") == 0)
 	{
 	 if((oneOperImm(argument1,lineNum) == TRUE || oneOperRegDir(argument1) == TRUE || oneOperLabel(argument1,lineNum) == TRUE) && (oneOperRegDir(argument2) == TRUE || oneOperLabel(argument2,lineNum) == TRUE || oneOperImm(argument2,lineNum) == TRUE))
        	 return TRUE; 		
 	 else
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Unvalid operand");   	 	 
 	 	 return FALSE;
 	 	} 
 	 }
 else if(strcmp(func,"lea") == 0)
 	{
 	 if((oneOperLabel(argument1,lineNum) == TRUE) && ((oneOperRegDir(argument2) == TRUE || oneOperLabel(argument2,lineNum) == TRUE)))
        	return TRUE; 		
 	 else
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Unvalid operand");   	 	 
 	 	 return FALSE;
 	 	} 
 	 }	 		
 if((oneOperRegDir(argument1) == TRUE || oneOperImm(argument1,lineNum) == TRUE || oneOperLabel(argument1,lineNum) == TRUE) && (oneOperRegDir(argument2) == TRUE || oneOperLabel(argument2,lineNum) == TRUE))
        return TRUE; 	
 else
 	 	{
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->Unvalid operand");  	 	 
 	 	 return FALSE;
 	 	}
}

/*read the next line - max line 100 chars
 *
 *parameters:
 *line - the line save in this argument
 *fp - pointer to the file
 *lineNum - the number of line
 *
 *Returns -NULL if the line cant be read, else return pointer to the line
 */
char *readMaxChars(char *line, FILE *fp,int lineNum)
{
    int ch;
    char *newline = NULL;

    newline = fgets(line, MAX, fp);     
    if(strlen(line) == MAX-1 && line[MAX-2] != '\n')
    	{
	    do
	    {
		ch = fgetc(fp);
	    }while(ch != '\n' && ch != EOF);
	  errorPrint(lineNum);
 	  printf("\n\t-->Max size of line is 100 chars");
 	  line[0] = ';';	   
	}    
    return newline;
}

