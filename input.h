#define MAX 100 /*Line length*/
#define MAX_LABEL_SIZE 32
#define MEMORY_SIZE 4096

#define DATA_LENGTH 5
#define STRING_LENGTH 8
#define EXTERN_LENGTH 8
#define ENTRY_LENGTH 7
#define END_OF_FILE 5

#define END_OF_LINE -1
#define TRUE 1
#define FALSE 0
#define THREE_DIGIT 1000

#define IMMEDIATE 0
#define DIRECT 1
#define RELATIVE 2
#define REGISTER_DIRECT 3

#define TWO_OPERAND_FUNCTION 4
#define ONE_OPERAND_FUNCTION 13
#define TWO_ARGUMENTS 2
#define ONE_ARGUMENT 1
#define NON_ARGUMENT 0

#define UNVALID -1
#define INITIALIZATION -1
#define ASCCI_VALUE 48

#define END_OB ".ob\0"
#define END_ENT ".ent\0"
#define END_AS ".as\0"
#define END_EXT ".ext\0"

#if !defined(___INPUT___HEADER___)
#define ___INPUT___HEADER___

extern int  errorFlag;

char *readMaxChars(char *line, FILE *fp,int lineNum);
int ICCounter(char filename[], int IC);
int jumpSpaces(char line[], int i);

int getFunc(int i, char line[], char func[]);
int checkLabel(char label[],int lineNum);
int SavedWord(char label[]);
int notEmpty(char line[]);
int isComment(char line[]);

int splitToArguments(char *argument1,char *argument2, char line[],int index,int lineNum);
int twoOper(char argument1[], char argument2[], char func[],int lineNum);
int oneOper(char argument[],char func[],int lineNum);
int oneOperRegDir(char argument[]);

void errorPrint(int lineNum);
void cleanUp(char filename[]);
#endif
