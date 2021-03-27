#if !defined(___WRITE___TO___FILES___HEADER___)
#define ___WRITE___TO___FILES___HEADER___

void printToEnt(char filename[], Node* head);
void printToFileExt(char filename[],char label[], int IC, Node* head);
void printToFile(DataNode* head, char filename[], int IC);
void writeTofile(char argument[], char filename[], Node* symbolTable,int IC, int lineNum);
void writeFuncTofile(char filename[], int funcIndex);
void writeMionTofile(char filename[], char argument1[], char argument2[]);
#endif
