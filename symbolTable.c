#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symbolTable.h"
#include "input.h"
#define EXTERNAL_DEFINED_TWICE -2

/*get value of given symbol for the symbol table
 *
 *parameters:
 *head - pointer to the head of symbol table
 *sym - symbol that need to serch in the list
 *lineNum - current line number
 *
 *Returns - "-1" - if not found in the symbol table , else return the value
 */
int getValue(Node* head, char sym[],int lineNum)
 {
  while(head != NULL)
  {   
   if(strcmp(head->symbol,sym) == 0)
   	{
   	 if(strcmp(head->attribute,"external") == 0)
   	 	return FALSE;
   	 return head->value; 
   	} 	
   head = head->next;   	
  }   
  if(SavedWord(sym) == TRUE)
  	{
  	 errorPrint(lineNum);
  	 printf("\n\t-->Label did not exist %s", sym);  	 
  	} 
  return UNVALID;
 }

/*Creates new Node in linked list (keep the labels of the file) 
 *
 *parameters:
 *sym - given label from file
 *val - Value of the label
 *att - attribute can be (code,entry, external, data)
 *
 *Returns - The created node
 */
Node* newNode(char sym[], int val, char att[])
 {
  Node *tmp = NULL;
  tmp = (Node*)calloc(1,sizeof(Node));
  if(tmp == NULL)
  	{
  	 fprintf(stderr,"out of memory\n");
  	 exit(1);
  	}
  strcpy(tmp->symbol,sym);
  tmp->value = val;
  strcpy(tmp->attribute,att); 
  strcpy(tmp->attribute2,"");
  return tmp;	
 }
 
 /*destroy given linked list
 *
 *parameters:
 *head - the start of linked list
 */
 void destroy(Node** head)
 {
  if(*head == NULL)
  	return;
  destroy(&((*head)->next));
  free(*head);
  *head = NULL;		
 }

/*search if label already exists in the symbol table (if extern label define more then once its ok)
 *
 *parameters:
 *sym - given label to serach
 *head - pointer to the start of symbol table
 *
 *Returns - "UNVALID" - if not found , "EXTERNAL_DEFINED_TWICE" - if the label found but it is external Label defiend twice , else return the posstion of the label in the symbol table
 */
 int search(Node* head, char sym[])
 {
  int pos =1;  
  while(head != NULL)
  {
   if(strcmp(head->symbol,sym) == 0)
   	{
   	 if(strcmp(head->attribute,"external") == 0)
   	 	return EXTERNAL_DEFINED_TWICE;
   	 return pos;
   	} 
   head = head->next;
   pos++;	
  }
  return UNVALID;
 }

/*Insert to end of linked list (symbol table) given Node
 *
 *parameters:
 *head - the head of symbol table
 *sym - the label to enter 
 *val - the value of the label
 *att - the attribute of the label
 *
 *Returns - pointer to head of the symbol table
 */
Node* insertToEnd(Node* head,char sym[], int val, char att[])
 {
  if(head == NULL)
  	return newNode(sym,val,att);
  else
  	{
  	 Node *tmp = head;
  	 while(tmp->next != NULL)
  	 	tmp = tmp->next;
  	 tmp->next = newNode(sym,val,att);
  	}
  return head;	 		
 }

/*Insert to start of linked list (symbol table) given Node
 *
 *parameters:
 *head - the head of symbol table
 *sym - the label to enter 
 *val - the value of the label
 *att - the attribute of the label
 *
 *Returns - pointer to head of the symbol table
 */
Node* insertToStart(Node* head, char sym[], int val, char att[])
{
 Node* tmp = newNode(sym,val,att);
 if(tmp!= NULL)
 {
  tmp->next=head;
  head=tmp;
 }
 return head;
} 

/*Insert to linked list (symbol table) given Node
 *
 *parameters:
 *head - the head of symbol table
 *sym - the label to enter 
 *val - the value of the label
 *att - the attribute of the label
 *lineNum - the current line number
 *
 *Returns - pointer to head of the symbol table
 */
Node* insert(Node* head, char sym[], int val, char att[],int lineNum)
{
 int value;
 if(SavedWord(sym) == FALSE)
 	{
 	 errorPrint(lineNum);
 	 printf("\nThe label --> '%s' is saved word\n", sym);
 	 return head;
 	}
 else if(head == NULL || head->symbol == NULL)
 	return insertToStart(head,sym,val,att);
 else
 	{
 	 if((value = search(head, sym)) == -1)
 	 	return insertToEnd(head,sym,val,att);
 	 else if(value == EXTERNAL_DEFINED_TWICE && strcmp(att,"external") == 0)
 	 	return head;	
 	 else
 	 	{ 	 	 
 	 	 errorPrint(lineNum);
 	 	 printf("\n\t-->The label --> '%s' already exists", sym);
 	 	 return head;
 	 	} 	
 	} 	
}


/*add to symbol table entry attribute if it apper in the file and was not defiend as externl
 *
 *parameters:
 *lineNum - current line number
 *label - the entry label
 *head - pointer to the stmbol table
 *
 *Returns - pointer to the head of the symbol table
 */ 
Node* insertEntryLabel(Node *head, char Label[],int lineNum)
 { 
  Node *tmp = head;  
  while(tmp != NULL)
  {
   if(strcmp(tmp->symbol,Label) == 0)
   	{
   	 if(strcmp(tmp->attribute,"external") == 0)
   	 	{
   	 	 errorPrint(lineNum);
   	 	 printf("\n\t-->The label '%s' already designated as external", Label); 
   	 	 return head;
   	 	} 
    	 strcpy(tmp->attribute2,"entry");
    	 return head;
    	} 
   tmp = tmp->next;   	
  }  
  errorPrint(lineNum);
  printf("\n\t-->Undefined label '%s'", Label);	
  return head;
 }

/*define label attribute , instraction counter and insert into symbol table 
 *
 *parameters:
 *lineNum - current line number
 *IC - instraction counter
 *head - pointer to the stmbol table
 *line - the current line
 *
 *Returns - pointer to the head of the symbol table
 */ 
Node* defineLabel(char line[], Node* head, int IC, int lineNum)
{
 int index =0, labelIndex =0;
 char *Label= NULL;
 char data[6] = ".data", exte[9] =".extern", string[8] = ".string", entry[7] = ".entry";
 index = jumpSpaces(line,index);	
 Label = (char*)calloc(MAX_LABEL_SIZE, sizeof(char));
 if(Label == NULL)
  	{
  	 fprintf(stderr,"out of memory\n");
  	 exit(1);
  	}			 	 			 	 
 while(line[index] != ':' && isspace(line[index]) == 0)
 	{
 	 if(labelIndex >= MAX_LABEL_SIZE-1)
	 	 {
	 	  labelIndex++;
	 	  errorPrint(lineNum);
	    	  printf("\n\t-->Max size of label is 31");
	    	  break;
	 	 }					 	 
 	 Label[labelIndex] = line[index];			 	 	 
 	 index++;
	 labelIndex++;			 	 	 
 	}			 	 	
 if(labelIndex == MAX_LABEL_SIZE)
		;
  else
  	{					 	 	
 	 index++; /*skip :*/			 	 	
 	 Label[labelIndex] = '\0';			 	 			 	 			 	 
 	 if(strstr(line,data) != NULL || strstr(line,string) != NULL)
		head = insert(head, Label, IC, "data",lineNum);
	 else if(strstr(line,exte) != NULL)
	 	head = insert(head, Label, IC, "external", lineNum);	  	 
	 else if((strstr(line,entry) != NULL))
	 	head = insert(head, Label, IC, "entry", lineNum);
	 else 
	 	head = insert(head, Label, IC, "code", lineNum);
 	}
 free(Label);
 return head;						 	 			 	 		
}

void update_symbolTable(Node* head,int IC)
{
 while(head != NULL)
  	{
  	 if(strcmp(head->attribute,"data") == 0)
  	 	head->value = head->value + IC;
  	 head = head->next;
  	}
}
