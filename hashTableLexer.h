//Deepak Guneja
//2015A7PS0083P

#include "header.h"

struct keyword
{
	char key[15];
	char tkn[15];
	struct keyword* next;
};

typedef struct keyword keyword;

struct hashNode
{
	keyword* first;
	keyword* last;
};

typedef struct hashNode hashNode;

hashNode* createHashTable(char* keyWrdFile);

int hashIndex(char* key);

void insertSymbol(hashNode* hashTable, char* key, char* tkn, int index);

keyword* lookupHashTable(hashNode* hashTable, char* id);

void printHashTable(hashNode* hashTable);
