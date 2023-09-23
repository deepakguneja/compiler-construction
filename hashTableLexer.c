//Deepak Guneja
//2015A7PS0083P

#include"hashTableLexer.h"

hashNode* createHashTable(char* keyWrdFile) {
	//This function is to create a Hash Table with the given keywords filename.
	FILE* fp = fopen(keyWrdFile, "r");
	if(fp==NULL)
	{
		printf("Give the file to read keywords for Hash Table\n");
		exit(1);
	}
	//Assuming Keywords in the range 50
	hashNode* hashTable = (hashNode*)malloc(41*sizeof(hashNode));
	for (int i = 0; i < 41; ++i)
	{
		hashTable[i].first = NULL;
		hashTable[i].last = NULL;
	}
	int noOfKeys;
	char keyWithTkn[30];
	fgets(keyWithTkn, 30, fp);
	noOfKeys = atoi(keyWithTkn);
	int i;
	char* key = (char*)malloc(15*sizeof(char));
	char* tkn = (char*)malloc(15*sizeof(char));
	for(i=0; i<noOfKeys; i++) {
		fgets(keyWithTkn, 30, fp);
		key = strtok(keyWithTkn, ",");
		tkn = strtok(NULL, "\n");
		int index = hashIndex(key);
		// printf("%s: %d\n", key, index);
		insertSymbol(hashTable, key, tkn, index);
	}
	fclose(fp);
	//free(key);
	//free(tkn);
	return hashTable;
}

int hashIndex(char* key) {
	//This function calculates tha hash index using Horner's Formula
	int len = strlen(key);
	int i;
	int index = 0;
	for(i=len-1; i>=0; i--) {
		//choosing x = 33;
		index = (index*33 + key[i])%41;
	}
	return index;
}

void insertSymbol(hashNode* hashTable, char* key, char* tkn, int index) {
	//This fucntion inserts symbol in hashTable
	keyword* node = (keyword*)malloc(sizeof(keyword));
	strcpy(node->key, key);
	strcpy(node->tkn, tkn);
	node->next = NULL;
	if(hashTable[index].first==NULL) {
		hashTable[index].first = node;
		hashTable[index].last = node;
		return;
	}
	hashTable[index].last->next = node;
	hashTable[index].last = node;
	return;
}

keyword* lookupHashTable(hashNode* hashTable, char* id) {
	//This function looks for the given id in hahsTable
	int index = hashIndex(id);
	if(hashTable[index].first==NULL) {
		return NULL;
	}
	keyword* node = hashTable[index].first;
	while(node!=NULL) {
		if(strcmp(node->key, id)==0) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

void printHashTable(hashNode* hashTable) {
	//This function prints the hashTable
	int i;
	for(i=0; i<41; i++) {
		if(hashTable[i].first!=NULL) {
			keyword* node = hashTable[i].first;
			while(node!=NULL) {
				printf("%s ", node->key);
				node = node->next;
			}
			printf("\n");
		}
		else {
			printf("Empty Node\n");
		}
	}
}