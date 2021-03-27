#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dataTable.h"
#include "input.h"

/*Creates new Node in linked list (keep the data of the file) 
 *
 *parameters:
 *DC - Data counter value
 *val - Value of the data
 *
 *Returns - The created node
 */
 DataNode* newNodeD(int DC,int val)
 {
  DataNode *tmp = NULL;
  tmp = (DataNode*)calloc(1,sizeof(DataNode));
  if(tmp == NULL)
  	{
  	 fprintf(stderr,"out of memory\n");
  	 exit(1);
  	}
  tmp->DC=DC;
  tmp->value = val;  
  return tmp;	
 }
 
/*destroy given linked list
 *
 *parameters:
 *head - the start of linked list
 */
void destroyD(DataNode** head)
 {
  if(*head == NULL)
  	return;
  destroyD(&((*head)->next));
  free(*head);
  *head = NULL;		
 }

/*Insert to end of linked list givaen DataNode
 *
 *parameters:
 *head - the head of Data list
 *DC - Data counter 
 *val - value of number / ASSCI char 
 *
 *Returns - pointer to head of the data list 
 */
DataNode* insertToEndD(DataNode* head,int DC, int val)
 {
  if(head == NULL)
  	return newNodeD(DC,val);
  else
  	{
  	 DataNode *tmp = head;
  	 while(tmp->next != NULL)
  	 	tmp = tmp->next;
  	 tmp->next = newNodeD(DC,val);
  	}
  return head;	 		
 }

/*Insert to start of linked list givaen DataNode
 *
 *parameters:
 *head - the head of Data list
 *DC - Data counter 
 *val - value of number / ASSCI char 
 *
 *Returns - pointer to head of the data list 
 */
DataNode* insertToStartD(DataNode* head,int DC, int val)
{
 DataNode* tmp = newNodeD(DC,val);
 if(tmp!= NULL)
 {
  tmp->next=head;
  head=tmp;
 }
 return head;
} 

/*Insert a Node to linked list 
 *
 *parameters:
 *head - the head of Data list
 *DC - Data counter 
 *val - value of number / ASSCI char 
 *
 *Returns - pointer to head of the data list 
 */
DataNode* insertD(DataNode* head,int DC, int val)
{
 if(head == NULL)
 	return insertToStartD(head,DC,val);
 else
 	return insertToEndD(head,DC,val); 	
}

/*get line of data and insert into data list
 *
 *parameters:
 *head - the head of Data list
 *line - current line 
 *lineNum - current line number
 *
 *Returns - pointer to head of the data list 
 */
DataNode* writeDataToList(DataNode *head, char line[],int lineNum)
{
 int index =0, indexNumber =0 ,tmpData, Minous =0;
 unsigned int tmpNumberInt; /*MAX number 4095*/ 
 char *tmpNumber=NULL;   
 static int DC =0; 
 if(lineNum == -1)
 	{
 	 DC =0;
 	 return head;
 	} 
 if((strstr(line,".string ")) != NULL)
	  	{	  	 	  	 	  	 	 
		 while(line[index] != '\"')
		 	index++;			 		 		 		 		
		 index++; 	
		 while(line[index] != '"')
		 	{
		 	 tmpData = (int)(line[index]);		 		 
		  	 head = insertD(head,DC,tmpData);
		  	 index++;
		  	 DC++;
		 	}	
		  head = insertD(head,DC,'\0');	
		  DC++;	 			
		 }		
 else if((strstr(line,".data ")) != NULL)
		{ 	
		 while(line[index] != '.')
	  	 	index++;	 
	  	 index = index + DATA_LENGTH;	 	 	  	 	  	 	 
		 index = jumpSpaces(line,index);		   
		 while(line[index] != '\n')
		  	{	 	 	 		 	 	 
		 	 index = jumpSpaces(line,index);
		 	 if(index == END_OF_LINE)
		 	 	break;
		 	 if(line[index] =='-')
		 	 	{
		 	 	 index++;
		 	 	 Minous =1;
		 	 	} 
		 	 else if(line[index] == '+')
		 	 	index++;		
		 	 tmpNumber = (char*)calloc(6, sizeof(char));
		 	 if(tmpNumber == NULL)
			  	{
			  	 fprintf(stderr,"out of memory\n");
			  	 exit(1);
			  	}
		 	 while(isspace(line[index]) == 0 && line[index] != ',')
		 	 	{
		 	 	 tmpNumber[indexNumber] = line[index];			 	 	 			 	 	 
		 	 	 indexNumber++; 
		 	 	 index++;
		 	 	} 
		 	 tmpNumber[indexNumber] = '\0';				 	 				 	 	
		 	 tmpNumberInt = atoi(tmpNumber);			 	 
		 	 if(Minous ==1)
		 	 	{
		 	 	 Minous = 0;
		 	 	 tmpNumberInt = tmpNumberInt *(-1);
		 	 	} 			 	 					 	 			 	 	
		 	 head = insertD(head,DC,tmpNumberInt);
			 DC++;
			 index = jumpSpaces(line,index);	
		 	 if(index == END_OF_LINE)
		 	 	{
		 	 	 free(tmpNumber);
			 	 break;
			 	} 
		 	 if(line[index] == ',')
		 	 	index++;	
		 	 else
		 	 	{
		 	 	 index = jumpSpaces(line,index);	
		 	 	 if(index == END_OF_LINE)
		 	 	 	{
		 	 	 	 free(tmpNumber);
		 	 	 	 break;
		 	 	 	} 
		 	 	} 			 	  
		 	  indexNumber = 0;			 	  
		 	  free(tmpNumber);
		 	 }
		 }	 	 	  		 	 			 		 	
 return head;
}

/*get data list and return the Data counter
 *
 *parameters:
 *head - the head of Data list
 *
 *Returns - Data counter
 */
int getDC(DataNode *head)
{
 int tmpDC = 0;
 head=head->next;
 if(head == NULL)
 	return tmpDC;
 while(head != NULL)
  {
   tmpDC = head->DC;   
   head = head->next;
  }
  return tmpDC+1;
}
