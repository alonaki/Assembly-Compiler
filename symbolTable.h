typedef struct Node
{
 char symbol[32];
 int value;
 char attribute[10];
 char attribute2[10];
 struct Node* next;
}Node;

#if !defined(___SYMBOL___TABLE___HEADER___)
#define ___SYMBOL___TABLE___HEADER___

Node* defineLabel(char line[], Node* head, int IC, int lineNum);
Node* insert(Node* head, char sym[], int val, char att[],int lineNum);
Node* insertEntryLabel(Node *head, char Label[], int lineNum);	
void update_symbolTable(Node *symbolTable,int IC);
int getValue(Node* head, char sym[], int lineNum);
void destroy(Node** head);
#endif
