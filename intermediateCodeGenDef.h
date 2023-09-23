//Deepak Guneja
//2015A7PS0083P

#include "symbolTableAndTypeChecker.c"

int tempVarNo = 1;
int labelNo = 1;

enum operatorSym
{
	ASSIGN, LESSTHAN, LESSTHANEQ, GREATERTHAN, GREATERTHANEQ, NOTEQUAL, ISEQUAL, PLUSOP, MINUSOP, MULOP, DIVOP, VALUEAT, READOP, PRINTOP, LABELOP, GOTO, ADDRESSOFFSET
};

typedef enum operatorSym operatorSym;

struct
{
	operatorSym op;
	char* str;
} convert4[] = { {ASSIGN, "="}, {LESSTHAN, "<"}, {GREATERTHAN, ">"}, {LESSTHANEQ, "<="}, {GREATERTHANEQ, ">="}, {NOTEQUAL, "!="}, {ISEQUAL, "=="}, {PLUSOP, "+"}, {MINUSOP, "-"}, {MULOP, "*"}, {DIVOP, "/"}, {VALUEAT, "VALUEAT"}, {READOP, "READOP"}, {PRINTOP, "PRINTOP"}, {LABELOP, "LABELOP"}, {GOTO, "GOTO"}, {ADDRESSOFFSET, "ADDRESSOFFSET"}
};

struct intCode
{
	ASTnode* result;
	operatorSym op;
	ASTnode* arg1;
	ASTnode* arg2;
	struct intCode* next;
};

typedef struct intCode intCode;

struct retTypeIntCode	//going to be used for checkArithExprsnIntCode()
{
	varType t;
	int sz[2];
	intCode* code;
	ASTnode* addr;
};

typedef struct retTypeIntCode retTypeIntCode;

int offsetForIntCode = 0;	//as only one function so offset can be global