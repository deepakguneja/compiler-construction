//Deepak Guneja
//2015A7PS0083P

#include "abstractSyntaxTree.c"

int semanticErr = 0;	
int semanticErrExist = 0;//If Semantic Error then no need to do Code Generation, i.e. semanticErrExist = 1;

struct functionField
{
	void* childSymTablePtr;	//pointer to child Symbol Table 	//will be typecasted to symTableNode
	int inpParamListSz;
	int outParamListSz;
	int inpIndices[20];	//indices of input parameteres in childSymTable, maximum number of input paramters possible = 20
	int outIndices[20];	//indices of output parameteres in childSymTable, maximum number of output paramters possible = 20
	varType inParamType[20];	//type of input output parameters
	varType outParamType[20];
};

typedef struct functionField functionField;

struct idField
{
	int offset;
	int sz[2];	//if matrix then it would be nxm, otherwise in sz[0] = size, even for strings
	int outParamInitOrNot;	//If the id is output parameter list, so this will be set to 0, and will remain 0 till it is initialised
};

typedef struct idField idField;

struct field
{
	token* tkn;	//ptr to lexeme in token
	char* funcScopeName;	//ptr to lexeme in parent function's token
	varType t;
	functionField* funcPtr;
	idField* idPtr;
	struct field* next;	//if collison in symbolTable, then handled by Chaining
};

typedef struct field field;

struct symTableNode
{
	field* first;
	field* last;
};

typedef struct symTableNode* symTable;	//symbol Tabnle is an array of symTableNode	

struct symTableStackNode
{
	symTable st;
	char* functionName;	//FUNID whose symbol table is this
	struct symTableStackNode* next;
	// struct symTableStackNode* prev;
};

typedef struct symTableStackNode symTableStackNode;

struct symTableStack
{
	symTableStackNode* first;
};

typedef struct symTableStack symTableStack;

struct retType	//going to be used for checkArithExprsn()
{
	varType t;
	int sz[2];
};

typedef struct retType retType;