//***********************************************************************************//
// This header file is used to define all functions prototypes and global variables. //
//  This header file being included in the general.h file.			     //
//***********************************************************************************//

#define COMMANDS_AMOUNT 16

#define REGISTERS_AMOUNT 16

#define ADDRESS_TO_BEGIN 100

#define MAX_STRING_LEN 256

enum {
		SYMBOL_ON,SYMBOL_OFF,
		ERROR_ON,ERROR_OFF
	};

static char* commandsArr[]={"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};

static char* registersArr[]={"r0","r1","r2","r3","r4","r5","r6","r7","R0","R1","R2","R3","R4","R5","R6","R7"};

static int errorFlag=ERROR_OFF;

static int symbolFlag=SYMBOL_OFF;

static int DC=0,IC=0,numExtern=0,numEntry=0;


int GetCommandBin(char* command);
char* NewFileNameToOpen(char*);
void SkipSpacesInLine(char**);
char* GetExtFileName(char *);
char* GetEntFileName(char*);
int GetRegisterIndex(char*);
int OneOperandHandler(char*);
int GetEntryAddress(char* ,char** ,char** );
int TwoOperandsHandler(char* );
int CharsInLine(char* );
int LinesAllocator(char* );
void GetOperandBinCode(char* , char* , int ,char** ,char** ,char** ,unsigned long* ,char* ,char** ,FILE* , int [], int , int , int* ,char**,int*,int );
int DataCounter(char* );
void  MemoryAllocator(FILE* );
void FirstRead(FILE* ,char** ,char** ,char** ,char** );
int SecondRead(FILE* ,char** ,char** ,char** ,char** ,unsigned long* ,char* ,char*,int*,char**,int*,char**,int* );
int GetSymbolIndex(char* ,char** ,char** );
int IsExtern(char* ,char** );
int GetOperandIndex(char* ,char** ,char** );
char* GetObjFileName(char* );
int IsEntry(char* ,char** );
char *GetParameterIndex(char* );
int BinToDecimalConvertor(int );
unsigned long BinToOctalConvertor(int []);
long DecToOctalConvertor(int );
int IsContainIndex(char* );
char *GetVariable(char* );
void VariableHandler(char* , unsigned long* , char** , char** , int , int* , int , char* );
unsigned long BinaryNegativeValuesHandler(int );
