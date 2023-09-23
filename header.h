//Deepak Guneja
//2015A7PS0083P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

union tokenVal {
	int intVal;
	double doubleVal;
};

typedef union tokenVal tokenVal;

struct token {
	char tkn[15];
	char* lexeme;
	tokenVal value;
	int lineNo;
	/********************/ //change below later
	struct token* next; //change it later
};

typedef struct token token;

struct tokenHead {
	token* first;
	token* last;
};

typedef struct tokenHead tokenHead;

struct errors
{
	int lineNo;
	char errStr[2048];
	struct errors* next;
};

typedef struct errors errors;

struct lexerErrHead
{
	errors* first;
	errors* last;
};

typedef struct lexerErrHead lexerErrHead;

lexerErrHead* lErr;

struct parserErrHead
{
	errors* first;
	errors* last;
};

typedef struct parserErrHead parserErrHead;

parserErrHead* pErr;

struct semanticErrHead
{
	errors* first;
	errors* last;
};

typedef struct semanticErrHead semanticErrHead;

semanticErrHead* symTableErr;

int noOfParseTreeNodes = 0;
int noOfASTnodes = 0;