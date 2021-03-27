typedef struct DataNode
{
 int DC;
 int value; 
 struct DataNode* next;
}DataNode;

#if !defined(___DATA___TABLE___HEADER___)
#define ___DATA___TABLE___HEADER___

DataNode* writeDataToList(DataNode *head, char line[],int lineNum);
DataNode* insertD(DataNode* head,int DC, int val);
void destroyD(DataNode** head);
int getDC(DataNode *head);
#endif
