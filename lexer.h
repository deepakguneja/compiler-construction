//Deepak Guneja
//2015A7PS0083P

#include "hashTableLexer.c"

tokenHead* tokenList(char* sourceCode, hashNode* hashTable);

void insertTknAtLast(tokenHead* allTkns, char* tkn, int lexemeBegin, int forward, char* currBuffptr, char* otherBuffptr, int lineNo, int tknValFlag, int buffFlag, hashNode* hashTable);
//tknValFlag will tell wheter to consider sent value or not
//buffFlag will tell whether lexemeBegin and forward are in the same buffer or not

token* getNextToken(token* currTkn);

void printToken(token* currTkn);

void printTokenList(tokenHead* allTkns);

void freeTkn(tokenHead* tkns);

char* giveErrLexeme(int lexemeBegin, int forward, char* currBuffptr, char* otherBuffptr, int buffFlag);

void insertLexerErr(char* str, int lineNo);

void printLexerErrs();

void removeTheComments(char* sourceCode);